#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

const char* AP_SSID = "ESP32_AP"; //change
const char* AP_PASSWORD = "12345678";　//change
const IPAddress AP_IP(192, 168, 4, 1);
const int AP_CHANNEL = 1;
const int SCAN_TIME = 5;

WebServer server(80);
BLEScan* pBLEScan;

const char LOGIN_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Login</title>
    <style>
        .login-container { 
            max-width: 300px; 
            margin: 100px auto; 
            padding: 20px;
            border: 1px solid #ccc;
            border-radius: 5px;
        }
        .input-group {
            margin-bottom: 15px;
        }
        input {
            width: 100%;
            padding: 8px;
            margin-top: 5px;
        }
        button {
            width: 100%;
            padding: 10px;
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 4px;
        }
    </style>
</head>
<body>
    <div class="login-container">
        <h2>ログイン</h2>
        <form onsubmit="login(event)">
            <div class="input-group">
                <label>パスワード:</label>
                <input type="password" id="password" required>
            </div>
            <button type="submit">ログイン</button>
        </form>
    </div>
    <script>
        function login(event) {
            event.preventDefault();
            const password = document.getElementById('password').value;
            if (password === '12345678') { //change
                localStorage.setItem('authenticated', 'true');
                window.location.href = '/scan-page';
            } else {
                alert('パスワードが違います');
            }
        }
    </script>
</body>
</html>
)rawliteral";

const char SCAN_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Network Scanner</title>
    <style>
        .container { padding: 20px; }
        .tabs { display: flex; gap: 10px; margin-bottom: 20px; }
        .tab-button {
            padding: 10px 20px;
            border: none;
            background: #ddd;
            cursor: pointer;
        }
        .tab-button.active {
            background: #4CAF50;
            color: white;
        }
        table { 
            border-collapse: collapse; 
            width: 100%; 
            margin-top: 20px; 
        }
        th, td { 
            border: 1px solid black; 
            padding: 8px; 
            text-align: left; 
        }
        .button-container { 
            display: flex; 
            gap: 10px; 
            margin: 20px 0; 
        }
        .scan-button { 
            background-color: #4CAF50; 
            color: white; 
            border: none; 
            border-radius: 4px;
            padding: 10px 20px;
        }
        .save-button { 
            background-color: #008CBA; 
            color: white; 
            border: none; 
            border-radius: 4px;
            padding: 10px 20px;
        }
        button:disabled { 
            background-color: #cccccc; 
            cursor: not-allowed; 
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="tabs">
            <button class="tab-button active" onclick="switchTab('wifi')">Wi-Fi</button>
            <button class="tab-button" onclick="switchTab('bluetooth')">Bluetooth</button>
        </div>
        
        <div id="wifi-tab" class="tab-content">
            <h2>Wi-Fi Networks</h2>
            <div class="button-container">
                <button class="scan-button" onclick="scanWiFi()">Wi-Fiスキャン</button>
                <button class="save-button" id="wifiSaveButton" onclick="downloadData('wifi')" disabled>保存</button>
            </div>
            <div id="wifiList"></div>
        </div>
        
        <div id="bluetooth-tab" class="tab-content" style="display: none;">
            <h2>Bluetooth Devices</h2>
            <div class="button-container">
                <button class="scan-button" onclick="scanBluetooth()">Bluetoothスキャン</button>
                <button class="save-button" id="btSaveButton" onclick="downloadData('bluetooth')" disabled>保存</button>
            </div>
            <div id="bluetoothList"></div>
        </div>
    </div>

    <script>
        if (!localStorage.getItem('authenticated')) {
            window.location.href = '/';
        }

        let currentWiFiData = null;
        let currentBTData = null;

        function switchTab(tab) {
            document.querySelectorAll('.tab-content').forEach(content => {
                content.style.display = 'none';
            });
            document.querySelectorAll('.tab-button').forEach(button => {
                button.classList.remove('active');
            });
            
            document.getElementById(`${tab}-tab`).style.display = 'block';
            document.querySelector(`[onclick="switchTab('${tab}')"]`).classList.add('active');
        }

        async function scanWiFi() {
            const scanButton = document.querySelector('#wifi-tab .scan-button');
            const saveButton = document.getElementById('wifiSaveButton');
            
            try {
                scanButton.disabled = true;
                scanButton.textContent = 'スキャン中...';
                
                const response = await fetch('/scan-wifi');
                currentWiFiData = await response.json();
                displayWiFiNetworks(currentWiFiData);
                
                saveButton.disabled = false;
            } catch (error) {
                console.error('Error:', error);
                alert('Wi-Fiスキャンエラーが発生しました');
            } finally {
                scanButton.disabled = false;
                scanButton.textContent = 'Wi-Fiスキャン';
            }
        }

        async function scanBluetooth() {
            const scanButton = document.querySelector('#bluetooth-tab .scan-button');
            const saveButton = document.getElementById('btSaveButton');
            
            try {
                scanButton.disabled = true;
                scanButton.textContent = 'スキャン中...';
                
                const response = await fetch('/scan-bluetooth');
                currentBTData = await response.json();
                displayBluetoothDevices(currentBTData);
                
                saveButton.disabled = false;
            } catch (error) {
                console.error('Error:', error);
                alert('Bluetoothスキャンエラーが発生しました');
            } finally {
                scanButton.disabled = false;
                scanButton.textContent = 'Bluetoothスキャン';
            }
        }

        function displayWiFiNetworks(data) {
            const networks = data.networks;
            let table = '<table><tr><th>SSID</th><th>RSSI</th><th>暗号化</th><th>チャンネル</th><th>BSSID</th></tr>';
            
            networks.forEach(network => {
                table += `<tr>
                    <td>${network.ssid}</td>
                    <td>${network.rssi} dBm</td>
                    <td>${network.encryption}</td>
                    <td>${network.channel}</td>
                    <td>${network.bssid}</td>
                </tr>`;
            });
            
            table += '</table>';
            document.getElementById('wifiList').innerHTML = table;
        }

        function displayBluetoothDevices(data) {
            const devices = data.devices;
            let table = '<table><tr><th>デバイス名</th><th>アドレス</th><th>RSSI</th><th>ベンダーコード</th></tr>';
            
            devices.forEach(device => {
                table += `<tr>
                    <td>${device.name || '不明'}</td>
                    <td>${device.address}</td>
                    <td>${device.rssi} dBm</td>
                    <td>${device.manufacturer || '不明'}</td>
                </tr>`;
            });
            
            table += '</table>';
            document.getElementById('bluetoothList').innerHTML = table;
        }

        function downloadData(type) {
            const data = type === 'wifi' ? currentWiFiData : currentBTData;
            if (!data) return;
            
            const blob = new Blob([JSON.stringify(data, null, 2)], { type: 'application/json' });
            const url = window.URL.createObjectURL(blob);
            const a = document.createElement('a');
            const timestamp = new Date().toISOString().replace(/[:.]/g, '-');
            
            a.href = url;
            a.download = `${type}_scan_${timestamp}.json`;
            document.body.appendChild(a);
            a.click();
            window.URL.revokeObjectURL(url);
            document.body.removeChild(a);
        }
    </script>
</body>
</html>
)rawliteral";

String scanWiFiNetworks() {
    JsonDocument doc;
    JsonArray networks = doc["networks"].to<JsonArray>();

    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++) {
        JsonObject network = networks.add<JsonObject>();
        network["ssid"] = WiFi.SSID(i);
        network["rssi"] = WiFi.RSSI(i);
        network["encryption"] = WiFi.encryptionType(i);
        network["channel"] = WiFi.channel(i);
        network["bssid"] = WiFi.BSSIDstr(i);
    }

    String result;
    serializeJson(doc, result);
    return result;
}

String scanBluetoothDevices() {
    JsonDocument doc;
    JsonArray devices = doc["devices"].to<JsonArray>();

    BLEScanResults* results = pBLEScan->start(SCAN_TIME, false);
    
    for (int i = 0; i < results->getCount(); i++) {
        BLEAdvertisedDevice device = results->getDevice(i);
        JsonObject deviceObj = devices.add<JsonObject>();
        
        if (device.haveName()) {
            deviceObj["name"] = device.getName().c_str();
        } else {
            deviceObj["name"] = "Unknown";
        }
        deviceObj["address"] = device.getAddress().toString().c_str();
        deviceObj["rssi"] = device.getRSSI();
        
        if (device.haveManufacturerData()) {
            char manufacturerData[100];
            uint8_t* data = (uint8_t*)device.getManufacturerData().c_str();
            int dataLen = device.getManufacturerData().length();
            for(int j = 0; j < dataLen && j < 49; j++) {
                sprintf(&manufacturerData[j*2], "%02X", data[j]);
            }
            manufacturerData[min(dataLen * 2, 98)] = '\0';
            deviceObj["manufacturer"] = manufacturerData;
        } else {
            deviceObj["manufacturer"] = "Unknown";
        }
    }

    pBLEScan->clearResults();
    String result;
    serializeJson(doc, result);
    return result;
}

void handleRoot() {
    server.send(200, "text/html", LOGIN_HTML);
}

void handleScanPage() {
    server.send(200, "text/html", SCAN_HTML);
}

void handleWiFiScan() {
    String networkData = scanWiFiNetworks();
    server.send(200, "application/json", networkData);
}

void handleBluetoothScan() {
    String deviceData = scanBluetoothDevices();
    server.send(200, "application/json", deviceData);
}

void setup() {
    Serial.begin(115200);
    
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(AP_SSID, AP_PASSWORD, AP_CHANNEL);
    
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setActiveScan(true);
    
    server.on("/", handleRoot);
    server.on("/scan-page", handleScanPage);
    server.on("/scan-wifi", handleWiFiScan);
    server.on("/scan-bluetooth", handleBluetoothScan);
    server.begin();
    
    Serial.println("Server started at " + WiFi.softAPIP().toString());
}

void loop() {
    server.handleClient();
    delay(10);
}
