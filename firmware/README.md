# ESP32 firmware — single controller

The current hardware design uses **one ESP32 board for both parking gates**.

## Connected devices

- RFID Reader ENTRY (RC522)
- RFID Reader EXIT (RC522)
- Servo ENTRY
- Servo EXIT
- Buzzer (shared feedback output)

Open `smart-parking/smart-parking.ino` in Arduino IDE.

## Required Arduino libraries

Install these libraries using Arduino IDE Library Manager:

- PubSubClient
- MFRC522
- ESP32Servo
- ArduinoJson

## Default pin assignment

| Component | ESP32 GPIO |
|---|---:|
| RC522 ENTRY SS/SDA | 5 |
| RC522 EXIT SS/SDA | 17 |
| Both RC522 RST | 22 |
| SPI SCK (shared) | 18 |
| SPI MISO (shared) | 19 |
| SPI MOSI (shared) | 23 |
| Servo ENTRY signal | 13 |
| Servo EXIT signal | 14 |
| Buzzer | 27 |

Both RC522 modules share the SPI bus and reset line, but use separate SS pins. Check wiring and GPIO availability on the exact ESP32 board before powering the circuit.

## Configuration

Edit `WIFI_SSID`, `WIFI_PASSWORD`, `MQTT_HOST`, and `MQTT_PORT` in the sketch.

The ESP32 subscribes to:

- `parking/entry/command`
- `parking/exit/command`

It publishes RFID events and status to the matching gate topics. The same board identifies the lane using `gate_id` and uses one device identity: `ESP32_PARKING_01`.

## Power and safety

- RC522 modules use 3.3 V logic and supply.
- Do not power two servos from the ESP32 3.3 V pin.
- Use a suitable regulated external servo supply and connect its GND to ESP32 GND.
- Confirm servo current, voltage, travel angle, and mechanical linkage before installation.
- This prototype has no gate-position limit switch, vehicle-presence sensor, or obstruction detection. An MQTT acknowledgement confirms that firmware issued a servo command; it does not independently verify the physical gate position or safe passage.
- Add authentication/TLS to MQTT and authorization to dashboard commands before deployment.

The sketch is starter firmware and has not yet been compiled or tested on the team's physical hardware.
