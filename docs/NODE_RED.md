# Node-RED Integration Architecture

Node-RED is the application flow runtime for the current prototype. It receives device events, applies application rules, accesses SQLite, and exposes HTTP endpoints for the dashboard or ESP32.

## Initial communication contract

| Direction | Method / endpoint | Purpose |
|---|---|---|
| Dashboard or test client → Node-RED | POST /api/rfid/scan | Submit RFID UID and gate identifier |
| Dashboard or test client → Node-RED | GET /api/health | Check Node-RED API availability |
| ESP32 → Node-RED | To be finalized | Device event transport |
| Node-RED → ESP32 | To be finalized | Gate command transport |

The first flow currently implements the two HTTP endpoints above. MQTT topics and device acknowledgement messages are intentionally not fixed until the team confirms the ESP32 communication method.

## RFID scan payload

```json
{
  "gate_id": "entry",
  "uid": "A1B2C3D4"
}
```

Allowed gate identifiers are entry and exit. The initial flow normalizes the UID and records a row in access_events. This stage does not yet check rfid_cards.is_active and does not create a parking_sessions row.

## Database

The initial SQLite schema is stored in node-red/sql/schema.sql. The Node-RED SQLite node uses a parameterized INSERT for RFID scan values.

## Planned next stages

1. Add card registration and active-card validation.
2. Define whether ESP32 communicates using MQTT or HTTP.
3. Add entry/exit session rules with duplicate-scan protection.
4. Add device heartbeat and acknowledged gate status.
5. Add authenticated dashboard controls and command audit logging.
6. Add dashboard widgets after the team confirms the dashboard package/version.

## Safety and state reporting

A scan event is not proof that a vehicle crossed the barrier. A command to open or close a gate is not proof that the physical barrier reached that position. The dashboard must distinguish pending commands, last reported state, and confirmed device state. Physical obstruction detection and emergency-stop mechanisms require appropriate hardware and are outside this initial flow.
