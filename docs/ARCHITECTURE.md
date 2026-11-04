# System Architecture

## 1. Purpose

This document describes the planned architecture of the Smart Parking System. The system has two independent access points: an entry gate and an exit gate. Both devices communicate with a central backend that serves the web dashboard and persists application data in SQLite.

This is a design document. It does not imply that all components have been implemented or tested.

## 2. High-Level Components

| Component | Responsibility |
|---|---|
| ESP32 Entry | Reads entry RFID events, controls its actuator and buzzer, reports device state |
| ESP32 Exit | Reads exit RFID events, controls its actuator and buzzer, reports device state |
| Backend API | Validates requests, applies parking rules, records events, dispatches gate commands |
| SQLite | Stores cards, parking sessions, gate telemetry, commands, and audit events |
| Web Dashboard | Presents statistics, transaction history, device state, and authorized controls |
| Virtual Simulator | Emulates RFID scans and gate devices before physical hardware is available |

## 3. Communication Flow

### RFID access

1. A card is presented to the RFID reader.
2. The ESP32 reads the card UID.
3. The device sends the UID, gate identifier, and event identifier to the backend.
4. The backend validates the card and checks the parking-session rules.
5. The backend returns an access decision.
6. The ESP32 activates the actuator and buzzer according to the decision.
7. The device reports the resulting gate state.
8. The backend stores the event and updates the dashboard.

### Emergency command

1. An authorized dashboard user requests an open or close operation.
2. The backend validates the user's authorization and the requested gate.
3. The backend stores the command as pending.
4. The command is delivered to the corresponding ESP32.
5. The ESP32 executes the command if it can do so safely.
6. The ESP32 returns an acknowledgement and reported state.
7. The backend updates the command result and the dashboard.

A command being submitted is not equivalent to the physical gate having moved. The UI must distinguish pending, acknowledged, failed, and timed-out commands.

## 4. Suggested API Contract

The following endpoints are proposed and may change during implementation.

| Method | Endpoint | Purpose |
|---|---|---|
| POST | `/api/access-events` | Submit an RFID access event |
| GET | `/api/gates` | Retrieve the latest state of both gates |
| GET | `/api/parking/summary` | Retrieve parking statistics |
| GET | `/api/parking/records` | Retrieve transaction history |
| GET | `/api/cards` | List registered RFID cards |
| POST | `/api/cards` | Register an RFID card |
| POST | `/api/gates/:gateId/commands` | Request a gate operation |
| POST | `/api/devices/:deviceId/telemetry` | Receive device telemetry |

Authentication and authorization must be added before exposing control endpoints beyond a trusted development environment.

## 5. Gate and Device Identity

Use stable identifiers:

- `ENTRY_GATE` — entry lane.
- `EXIT_GATE` — exit lane.
- `ESP32_ENTRY_01` — entry controller.
- `ESP32_EXIT_01` — exit controller.

Every event should include a unique event identifier to help the backend reject accidental duplicate submissions.

## 6. Gate State Model

Recommended logical states:

- `OPEN` — device reports that the gate is open.
- `CLOSED` — device reports that the gate is closed.
- `MOVING` — movement is in progress, if the hardware can report it.
- `UNKNOWN` — no reliable state is available.
- `OFFLINE` — device communication is unavailable.

If the selected actuator has no position feedback, the application can only report the last commanded or estimated state. It must not claim that the physical position has been independently verified.

## 7. Operational and Safety Notes

- RFID authentication alone cannot detect whether a vehicle has passed the barrier.
- Without a vehicle-presence or position sensor, the prototype cannot provide automatic anti-crush or obstruction detection.
- Remote open/close control must be protected by authentication and authorization.
- Network loss must not be represented as a successful gate operation.
- Physical emergency stop and safe actuator behavior must be considered separately from dashboard controls.
- Store timestamps consistently and display them in the Asia/Jakarta timezone.

## 8. Proposed Technology Choices

The current proposal is React and Tailwind CSS for the frontend, Node.js and Express for the backend, SQLite for persistence, and REST API with WebSocket updates. The final choices should be confirmed before implementation begins.
