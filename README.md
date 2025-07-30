# BayMaxTools BLE + Clock ⏰📡

BayMaxTools is a lightweight ESP32-based project designed to configure device parameters and time settings via Bluetooth Low Energy (BLE). It allows for seamless runtime configuration changes using a custom BLE characteristic and stores settings persistently using SPIFFS in JSON format.

---

## 🚀 Features

- 📡 Two-way BLE communication with custom UUID.
- ⏱️ Set device date and time via BLE (`SETTIME` command).
- ⚙️ Modify configuration parameters on the fly (`SETCFG` command).
- 💾 Saves configuration to SPIFFS (`/config.json`) for persistent use.
- 🔁 Auto restart on config update for immediate effect.
- 🧩 Simple JSON structure with `ArduinoJson`.

---

## 🧰 Tech Stack & Libraries

- Microcontroller: **ESP32**
- Platform: **Arduino Framework**
- BLE: [`BLEDevice.h`](https://github.com/espressif/arduino-esp32/tree/master/libraries/BLE)
- Filesystem: SPIFFS
- JSON Parser: [`ArduinoJson`](https://arduinojson.org/)

---

## 🧠 BLE Command Structure

### ✅ Set Time
Send the following command over BLE to set the current date and time:

```
SETTIME,DD,MM,YYYY,HH,MM,SS
```

**Example:**
```
SETTIME,29,07,2025,14,30,00
```

---

### ✅ Set Configuration
Update configuration like BLE device name or display format:

```
SETCFG,BLE_NAME,NewDeviceName
SETCFG,is12hFormat,true
```

---

## 📁 File Structure

```
baymaxtools-ble-clock/
├── src/
│   └── main.cpp           # Main application logic
├── data/
│   └── config.json        # Persistent configuration
├── README.md              # Project documentation
└── platformio.ini         # Optional: for PlatformIO build
```

---

### 📝 Sample `config.json`
```json
{
  "BLE_NAME": "BayMaxTools",
  "is12hFormat": false
}
```

---

## ⚙️ How to Use

1. Upload the code to your ESP32 using Arduino IDE or PlatformIO.
2. Use a BLE tool such as **nRF Connect** or **Serial Bluetooth Terminal**.
3. Connect to the device advertised as `BayMaxTools`.
4. Write commands (`SETTIME`, `SETCFG`) to the writable characteristic.
5. Configuration will be saved to SPIFFS and used on the next boot.

---

## 📦 Future Development Ideas

This project is intended as a minimal base system. Future expansions may include:

- Displaying time on OLED/TFT.
- Time synchronization via NTP.
- OTA (Over-The-Air) updates via BLE or Wi-Fi.
- Sensor integration (temperature, humidity, motion, etc.).
- Logging system for BLE data.

---

## 🛠 Recommended Tools

- [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/)
- [nRF Connect (Android/iOS)](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-mobile)
- [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal)

---

## 🤝 Contributing

Pull requests and feature suggestions are welcome. For major changes, please open an issue first to discuss what you would like to change.

---

## 🔗 Related Resources

- [ESP32 BLE Library Docs](https://github.com/espressif/arduino-esp32/tree/master/libraries/BLE)
- [ArduinoJson Documentation](https://arduinojson.org/v6/doc/)
- [SPIFFS on ESP32](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/spiffs.html)
