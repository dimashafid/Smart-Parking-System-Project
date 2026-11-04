# Node-RED integration

Node-RED provides HTTP API, SQLite access and MQTT transport. Import `node-red/flows/parking-system.json`, configure the SQLite path and broker.

Endpoints: GET /api/health, /api/cards, /api/summary, /api/history, /api/gates; POST /api/cards/register, /api/rfid/scan, /api/gates/command; dashboard GET /parking.

MQTT: parking/{entry,exit}/rfid (device → runtime), parking/{entry,exit}/command (runtime → device), parking/{entry,exit}/status (device → runtime).

SQLite triggers handle registered-card validation and entry/exit session transitions. Dashboard is served by a Node-RED template node. Restrict access to a trusted LAN; add authentication/TLS before deployment.
