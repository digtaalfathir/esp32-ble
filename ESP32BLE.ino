// ===========================
// BayMaxTools BLE + Clock
// GitHub Release 1.0.0
// ===========================

#include <FS.h>
#include <SPIFFS.h>
#include <BLE2902.h>
#include <BLEUtils.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <ArduinoJson.h>

// === Global Settings ===
bool receivedSetTime = false;
String setTimePayload = "";
bool receivedSetting = false;
String settingPayload = "";
String BLE_Stat = "Bluetooth Disconnected";

// === Clock Data ===
int day = 1, month = 1, year = 2025;
int hour = 0, minute = 0, second = 0;
bool is12hFormat = false;

// === BLE UUIDs ===
String BLE_NAME = "BayMaxTools";
BLEServer *pServer;
BLEAdvertising *pAdvertising;
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// === BLE Callbacks ===
class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      String data = String(pCharacteristic->getValue().c_str());
      data.trim();
      Serial.print("Data diterima via BLE: ");
      Serial.println(data);

      if (data.startsWith("SETTIME")) {
        setTimePayload = data;
        receivedSetTime = true;
      } else if (data.startsWith("SETCFG")) {
        settingPayload = data;
        receivedSetting = true;
      }
    }
};

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Perangkat terhubung.");
      BLE_Stat = "Bluetooth Connect";
    }

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Perangkat terputus. Mulai advertising ulang...");
      delay(100);
      pAdvertising->start();
      BLE_Stat = "Bluetooth Disconnect";
    }
};

// === Load Configuration ===
void loadConfig() {
  if (!SPIFFS.exists("/config.json")) {
    Serial.println("File config.json tidak ditemukan, menggunakan default.");
    return;
  }

  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Gagal membuka config.json");
    return;
  }

  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, configFile);
  if (error) {
    Serial.println("Gagal parsing config.json");
    return;
  }

  if (doc.containsKey("BLE_NAME")) {
    BLE_NAME = doc["BLE_NAME"].as<String>();
  }
  if (doc.containsKey("is12hFormat")) {
    is12hFormat = doc["is12hFormat"];
  }

  configFile.close();

  Serial.println("Konfigurasi berhasil dimuat dari config.json:");
  Serial.println("BLE_NAME: " + BLE_NAME);
  Serial.print("is12hFormat: "); Serial.println(is12hFormat);
}

// === Update Config Command ===
void handleSettingsCommand(String data) {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) return;

  StaticJsonDocument<512> config;
  if (deserializeJson(config, configFile)) return;
  configFile.close();

  int firstComma = data.indexOf(',');
  int secondComma = data.indexOf(',', firstComma + 1);
  if (firstComma == -1 || secondComma == -1) return;

  String key = data.substring(firstComma + 1, secondComma);
  String value = data.substring(secondComma + 1);

  if (key == "BLE_NAME") config["BLE_NAME"] = value;
  else if (key == "is12hFormat") config["is12hFormat"] = (value == "1" || value == "true");
  else return;

  File configOut = SPIFFS.open("/config.json", "w");
  if (!configOut) return;
  serializeJson(config, configOut);
  configOut.close();
}

// === Setup ===
void setup() {
  Serial.begin(115200);
  if (!SPIFFS.begin(true)) return;

  loadConfig();
  BLEDevice::init(BLE_NAME);
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ
                                       );
  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();
  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();

  Serial.println("BLE aktif. Siap menerima perintah.");
}

// === Main Loop ===
void loop() {
  if (receivedSetTime) {
    int d, m, y, h, min, s;
    sscanf(setTimePayload.c_str(), "SETTIME,%d,%d,%d,%d,%d,%d", &d, &m, &y, &h, &min, &s);
    day = d; month = m; year = y;
    hour = h; minute = min; second = s;
    Serial.printf("Waktu diperbarui: %02d/%02d/%04d %02d:%02d:%02d\n", day, month, year, hour, minute, second);
    receivedSetTime = false;
  }

  if (receivedSetting) {
    handleSettingsCommand(settingPayload);
    loadConfig();
    receivedSetting = false;
    ESP.restart();
  }
}
