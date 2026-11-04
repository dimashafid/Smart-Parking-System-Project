# Node-RED Smart Parking

## Install
From this folder run:
```bash
npm install
mkdir -p data
sqlite3 data/parking.db < sql/schema.sql
npm start
``
Import `flows/parking-system.json` from Node-RED Menu → Import. Set SQLite database path to the absolute path of `node-red/data/parking.db`. Configure MQTT broker node (default localhost:1883). Deploy. Dashboard: http://localhost:1880/parking.

## Hardware architecture
The current design uses **one ESP32 board** connected to two RFID readers and two servo motors. Node-RED still treats the entry and exit lanes separately using `gate_id` values `entry` and `exit`. Both MQTT topic groups are handled by the same physical controller, `ESP32_PARKING_01`.

## API
GET /api/health, /api/cards, /api/summary, /api/history, /api/gates. POST /api/cards/register with `{uid,owner_name,vehicle_plate}`. POST /api/rfid/scan with `{gate_id,uid}`. POST /api/gates/command with `{gate_id,command,source}`.

## Rules
SQLite trigger validates registered active cards, prevents duplicate open sessions, creates entry sessions, closes exit sessions, rejects invalid exits and records an OPEN command for accepted scans. The Node-RED flow publishes accepted commands via MQTT. Device status is ingested from `parking/+/status`.

## MQTT
The single ESP32 publishes to `parking/entry/rfid` or `parking/exit/rfid`; subscribes to `parking/+/command`; and publishes status to `parking/entry/status` and `parking/exit/status`. The `gate_id` in each message identifies which reader or servo is involved. Broker defaults localhost:1883.

## Limitations
No dashboard authentication/TLS. UID is not cryptographic authentication. RFID scan does not prove vehicle passage. Firmware acknowledgement confirms that a servo command was issued, not physical position feedback. Install suitable limit switches, obstruction detection, and a physical emergency stop before real barrier use. Restrict Node-RED/MQTT to a trusted LAN.

The unified firmware is located at `firmware/smart-parking/smart-parking.ino`. It has not yet been compiled or tested on the team's physical hardware.
