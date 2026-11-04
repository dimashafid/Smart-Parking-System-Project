# Node-RED Smart Parking

## Install
From this folder run:
```bash
npm install
mkdir -p data
sqlite3 data/parking.db < sql/schema.sql
npm start
```
Import `flows/parking-system.json` from Node-RED Menu → Import. Set SQLite database path to the absolute path of `node-red/data/parking.db`. Configure MQTT broker node (default localhost:1883). Deploy. Dashboard: http://localhost:1880/parking.

## API
GET /api/health, /api/cards, /api/summary, /api/history, /api/gates. POST /api/cards/register with `{uid,owner_name,vehicle_plate}`. POST /api/rfid/scan with `{gate_id,uid}`. POST /api/gates/command with `{gate_id,command,source}`.

## Rules
SQLite trigger validates registered active cards, prevents duplicate open sessions, creates entry sessions, closes exit sessions, rejects invalid exits and records an OPEN command for accepted scans. The Node-RED flow publishes accepted commands via MQTT. Device status is ingested from parking/+/status.

## MQTT
ESP32 publishes to parking/entry/rfid or parking/exit/rfid; subscribes to parking/{entry,exit}/command; publishes status to parking/{entry,exit}/status. Broker defaults localhost:1883.

## Limitations
No dashboard authentication/TLS. UID is not cryptographic authentication. RFID scan does not prove vehicle passage. Firmware status is not physical position feedback; install limit switches and obstruction detection and a physical emergency stop before real barrier use. Restrict Node-RED/MQTT to a trusted LAN.
