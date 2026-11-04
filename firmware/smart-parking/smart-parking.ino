/*
 * Smart Parking - Single ESP32 Controller
 * One ESP32 controls BOTH entry and exit lanes:
 * - RFID reader ENTRY + servo ENTRY
 * - RFID reader EXIT + servo EXIT
 *
 * Libraries:
 * - PubSubClient
 * - MFRC522
 * - ESP32Servo
 * - ArduinoJson
 *
 * Default ESP32 VSPI pins: SCK=18, MISO=19, MOSI=23.
 * RC522 modules share SPI and RST; each reader has a separate SS pin.
 * Servo motors must use a suitable external supply and common GND.
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>

// ---------- Network configuration ----------
const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";
const char* MQTT_HOST = "192.168.1.10";
const uint16_t MQTT_PORT = 1883;

const char* DEVICE_ID = "ESP32_PARKING_01";

// ---------- Pin configuration ----------
// RC522 ENTRY and EXIT share SPI bus and reset pin.
constexpr uint8_t RFID_ENTRY_SS = 5;
constexpr uint8_t RFID_EXIT_SS  = 17;
constexpr uint8_t RFID_RST      = 22;

// Two independent servo outputs.
constexpr uint8_t SERVO_ENTRY_PIN = 13;
constexpr uint8_t SERVO_EXIT_PIN  = 14;
constexpr uint8_t BUZZER_PIN      = 27;

constexpr int GATE_CLOSED_ANGLE = 0;
constexpr int GATE_OPEN_ANGLE = 90;
constexpr unsigned long SCAN_COOLDOWN_MS = 1200;

// ---------- Objects ----------
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
MFRC522 readerEntry(RFID_ENTRY_SS, RFID_RST);
MFRC522 readerExit(RFID_EXIT_SS, RFID_RST);
Servo servoEntry;
Servo servoExit;

unsigned long lastEntryScan = 0;
unsigned long lastExitScan = 0;

String topicFor(const char* gate, const char* suffix) {
  return String("parking/") + gate + "/" + suffix;
}

void publishStatus(const char* gate, const char* state,
                   const char* result, const char* commandId = "") {
  if (!mqtt.connected()) return;

  StaticJsonDocument<256> doc;
  doc["gate_id"] = gate;
  doc["device_id"] = DEVICE_ID;
  doc["state"] = state;
  doc["result"] = result;
  if (commandId && commandId[0] != '\0') {
    doc["command_id"] = commandId;
  }

  char payload[256];
  const size_t length = serializeJson(doc, payload, sizeof(payload));
  const String topic = topicFor(gate, "status");
  mqtt.publish(topic.c_str(), payload, length, false);
}

void publishRfid(const char* gate, MFRC522& reader) {
  String uid;
  for (byte i = 0; i < reader.uid.size; i++) {
    if (reader.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(reader.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  StaticJsonDocument<192> doc;
  doc["uid"] = uid;
  doc["gate_id"] = gate;
  doc["device_id"] = DEVICE_ID;

  char payload[192];
  const size_t length = serializeJson(doc, payload, sizeof(payload));
  const String topic = topicFor(gate, "rfid");
  mqtt.publish(topic.c_str(), payload, length, false);

  tone(BUZZER_PIN, 1800, 100);
  Serial.printf("RFID %s: %s\n", gate, uid.c_str());
}

void handleGateCommand(const char* gate, const char* command,
                       const char* commandId) {
  Servo& targetServo = (strcmp(gate, "entry") == 0) ? servoEntry : servoExit;

  if (strcmp(command, "OPEN") == 0) {
    targetServo.write(GATE_OPEN_ANGLE);
    publishStatus(gate, "open", "EXECUTED", commandId);
    tone(BUZZER_PIN, 2000, 120);
  } else if (strcmp(command, "CLOSE") == 0) {
    targetServo.write(GATE_CLOSED_ANGLE);
    publishStatus(gate, "closed", "EXECUTED", commandId);
    tone(BUZZER_PIN, 1200, 120);
  } else {
    publishStatus(gate, "unknown", "FAILED", commandId);
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<256> doc;
  const DeserializationError error = deserializeJson(doc, payload, length);
  if (error) {
    Serial.println("MQTT command JSON invalid");
    return;
  }

  const String topicName(topic);
  String gateFromTopic;
  if (topicName == topicFor("entry", "command")) {
    gateFromTopic = "entry";
  } else if (topicName == topicFor("exit", "command")) {
    gateFromTopic = "exit";
  } else {
    return;
  }

  const char* gate = doc["gate_id"] | "";
  const char* command = doc["command"] | "";
  const char* commandId = doc["command_id"] | "";

  // Ensure the command topic and payload refer to the same gate.
  if (gateFromTopic != gate || commandId[0] == '\0') {
    Serial.println("MQTT command rejected: gate_id or command_id mismatch");
    return;
  }

  handleGateCommand(gate, command, commandId);
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Wi-Fi connected, IP: ");
  Serial.println(WiFi.localIP());
}

void connectMqtt() {
  while (!mqtt.connected()) {
    const String clientId = String(DEVICE_ID) + "-" +
                            String((uint32_t)ESP.getEfuseMac(), HEX);
    if (mqtt.connect(clientId.c_str())) {
      mqtt.subscribe("parking/+/command", 1);
      publishStatus("entry", "unknown", "ONLINE");
      publishStatus("exit", "unknown", "ONLINE");
      Serial.println("MQTT connected; subscribed to parking/+/command");
    } else {
      Serial.printf("MQTT connection failed, state=%d\n", mqtt.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);

  servoEntry.setPeriodHertz(50);
  servoExit.setPeriodHertz(50);
  servoEntry.attach(SERVO_ENTRY_PIN, 500, 2400);
  servoExit.attach(SERVO_EXIT_PIN, 500, 2400);
  servoEntry.write(GATE_CLOSED_ANGLE);
  servoExit.write(GATE_CLOSED_ANGLE);

  SPI.begin(18, 19, 23);
  readerEntry.PCD_Init();
  readerExit.PCD_Init();

  connectWiFi();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  mqtt.setBufferSize(512);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  if (!mqtt.connected()) {
    connectMqtt();
  }
  mqtt.loop();

  const unsigned long now = millis();

  if (now - lastEntryScan >= SCAN_COOLDOWN_MS &&
      readerEntry.PICC_IsNewCardPresent() &&
      readerEntry.PICC_ReadCardSerial()) {
    publishRfid("entry", readerEntry);
    readerEntry.PICC_HaltA();
    readerEntry.PCD_StopCrypto1();
    lastEntryScan = now;
  }

  if (now - lastExitScan >= SCAN_COOLDOWN_MS &&
      readerExit.PICC_IsNewCardPresent() &&
      readerExit.PICC_ReadCardSerial()) {
    publishRfid("exit", readerExit);
    readerExit.PICC_HaltA();
    readerExit.PCD_StopCrypto1();
    lastExitScan = now;
  }
}
