# ESP32 ネットワークスキャナー

ESP32 C3を使用したWi-FiとBluetoothデバイスのスキャナーです。

## 🌟 主な機能
![4](https://github.com/user-attachments/assets/5f2013af-7ab2-4ecf-998d-3d6e72516e4f)
![3](https://github.com/user-attachments/assets/1ddf6459-7c2a-4e2e-bde4-1958a0865a11)

- Wi-Fiネットワークスキャン機能：               
  - SSID検出
  - 電波強度（RSSI）測定
  - 暗号化方式の特定
  - チャンネル情報
  - BSSID検出
  
    ![5](https://github.com/user-attachments/assets/aa5f4b51-0670-446c-8570-885a35e97f2a)

- Bluetoothデバイススキャン機能：
  - デバイス名検出
  - MACアドレス特定
  - 電波強度測定
  - ベンダー情報
- Wi-FiとBluetoothスキャン結果のJSONエクスポート

## 📋 必要環境

### ハードウェア
- ESP32 C3開発ボード
- プログラミング用USBケーブル

### ソフトウェア
- Arduino IDE
- 必要なライブラリ：
  ```
  - WiFi.h
  - WebServer.h
  - ArduinoJson.h
  - BLEDevice.h
  - BLEScan.h
  - BLEAdvertisedDevice.h
  ```

## 📱 使用方法

1. ESP32の電源を入れる
2. ESP32のWi-Fiアクセスポイントに接続
3. Chromiumベースのブラウザで`192.168.4.1`にアクセス
![2](https://github.com/user-attachments/assets/3bfdd83c-c2e8-492d-b2c9-f4b818de0655)

   - 注意：Safariは非対応
5. 設定したパスワードでログイン
6. タブでWi-FiとBluetoothスキャンを切り替え
7. スキャンボタンでネットワーク探索を開始
8. 保存ボタンで結果をエクスポート

## ⚠️ セキュリティ注意事項

- デフォルトの認証情報を必ず変更すること
- パスワードは平文で保存

## 📄 ライセンス
Copyright 2024 Leucophyllous (Leila_9136)
Licensed under the Apache License, Version 2.0 (the "License");
you may freely use this software except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
