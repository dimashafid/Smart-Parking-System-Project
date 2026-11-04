# System Architecture

## 1. Purpose

This document describes the planned architecture of the Smart Parking System. The prototype has two independent access points: an entry gate and an exit gate. Node-RED is the current application-flow runtime and connects the ESP32 devices, SQLite database, and dashboard.

This is a design document. Only the initial Node-RED HTTP health-check and RFID scan logging flow is currently provided; the remaining behavior is planned.

## 2. High-Level Components

| Component | Responsibility |
|---|---|
| ESP32 Entry | Reads entry RFID events, controls its actuator and buzzer, reports device state |
| ESP32 Exit | Reads exit RFID events, controls its actuator and buzzer, reports device state |
| Node-RED | Receives events, applies parking rules, exposes HTTP endpoints, coordinates device commands |
| SQLite | Stores cards, parking sessions, gate telemetry, commands, and audit events |
| Node-RED Dashboard | Presents statistics, transaction history, device state, and authorized controls |
| Virtual Device Simulator | Emulates RFID scans and gate devices before physical hardware is available |

## 3. Communication Flow

### RFID access (target behavior)

1. A card is presented to the RFID reader.
2. The ESP32 reads the card UID.
3. The device sends the UID and gate identifier to Node-RED.
4. Node-RED validates the card and checks parking-session rules.
5. Node-RED returns an access decision.
6. The ESP32 activates the actuator and buzzer according to the decision.
7. The device reports the resulting gate state.
8. Node-RED stores the event in SQLite and updates the dashboard.

The current starter flow implements only receipt and storage of RFID scan events. Card authorization and session processing are not implemented yet.

### Emergency command (target behavior)

1. An authorized dashboard user requests an open or close operation.
2. Node-RED validates authorization and the requested gate.
3. Node-RED stores the command as pending.
4. The command is delivered to the corresponding ESP32 using the transport selected by the team.
5. The ESP32 executes the command if it can do so safely.
6. The ESP32 returns an acknowledgement and reported state.
7. Node-RED updates the command result and dashboard.

A submitted command is not proof that the physical gate has moved. The UI must distinguish pending, acknowledged, failed, and timed-out commands.

## 4. Initial HTTP API

| Method | Endpoint | Current status |
|---|---|---|
| GET | /api/health | Implemented in starter flow |
| POST | /api/rfid/scan | Implemented: logs scan event |
| GET | /api/gates | Planned |
| GET | /api/parking/summary | Planned |
| GET | /api/parking/records | Planned |
| GET | /api/cards | Planned |
| POST | /api/cards | Planned |
| POST | /api/gates/:gateId/commands | Planned |
| POST | /api/devices/:deviceId/telemetry | Planned |

The current RFID request body is:

```json
{
  "gate_id": "entry",
  "uid": "A1B2C3D4"
}
```

The gate_id value must be entry or exit. Authentication and authorization must be added before exposing control endpoints beyond a trusted development environment.

## 5. Gate and Device Identity

Use stable identifiers:

- entry — entry lane.
- exit — exit lane.
- ESP32_ENTRY_01 — entry controller.
- ESP32_EXIT_01 — exit controller.

Every device event should eventually include a unique event identifier to help reject duplicate submissions.

## 6. Gate State Model

Recommended logical states:

- open — device reports that the gate is open.
- closed — device reports that the gate is closed.
- moving — movement is in progress, if hardware can report it.
- unknown — no reliable state is available.
- offline — device communication is unavailable.

If the selected actuator has no position feedback, the application can only report the last commanded or estimated state. It must not claim that the physical position has been independently verified.

## 7. Operational and Safety Notes

- RFID authentication alone cannot detect whether a vehicle has passed the barrier.
- Without a vehicle-presence or position sensor, the prototype cannot provide automatic anti-crush or obstruction detection.
- Remote open/close control must be protected by authentication and authorization.
- Network loss must not be represented as a successful gate operation.
- Physical emergency stop and safe actuator behavior must be considered separately from dashboard controls.
- Store timestamps consistently and display them in the Asia/Jakarta timezone.

## 8. Current Technology Direction

Node-RED is the application-flow runtime. SQLite is the persistence layer, accessed through the Node-RED SQLite node. The dashboard package/version and ESP32 transport (MQTT or HTTP) remain to be finalized by the team.
