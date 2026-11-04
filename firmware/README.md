# ESP32 firmware

Install Arduino libraries: PubSubClient, MFRC522, ESP32Servo. Open the matching sketch for each gate. Configure Wi-Fi credentials and MQTT broker IP. Default pins: RC522 SS=5/RST=22, servo=13, buzzer=27; verify your board pinout. RC522 uses 3.3V. Power servo from a suitable external supply and share GND; do not power a high-current servo from ESP32 3V3.

Prototype only: current sketch does not parse/deduplicate command IDs, authenticate MQTT, detect obstructions, or read a gate limit switch. Do not use as a real vehicle barrier without independent safety hardware.
