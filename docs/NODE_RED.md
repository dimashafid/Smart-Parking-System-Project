# Node-RED integration

Node-RED provides the HTTP API, SQLite access, MQTT transport, and dashboard. Import `node-red/flows/parking-system.json`, configure the SQLite path and Mosquitto broker.

The hardware design uses one ESP32 controller for both lanes. Node-RED keeps separate lane topics and gate IDs so entry and exit logic remain independent.

## HTTP endpoints

GET: `/api/health`, `/api/cards`, `/api/summary`, `/api/history`, `/api/gates`, `/parking`

POST: `/api/cards/register`, `/api/rfid/scan`, `/api/gates/command`

## MQTT topics

- `parking/entry/rfid` and `parking/exit/rfid` (ESP32 → Node-RED)
- `parking/entry/command` and `parking/exit/command` (Node-RED → ESP32)
- `parking/entry/status` and `parking/exit/status` (ESP32 → Node-RED)

Both topic groups are handled by the same physical ESP32, identified as `ESP32_PARKING_01`. The `gate_id` selects the entry or exit reader/servo.

SQLite triggers handle registered-card validation and entry/exit session transitions. Dashboard is served by a Node-RED template node. Restrict access to a trusted LAN; add authentication/TLS before deployment.
