# System Architecture

## 1. Purpose

This document describes the planned architecture of the Smart Parking System. The system has two lanes (entry and exit) controlled by **one shared ESP32 board**. The board is connected to two RFID readers and two gate servos. Node-RED is the application-flow runtime and connects the ESP32, SQLite database, and dashboard.

This is a design document. Runtime and physical hardware verification remain pending.

## 2. High-Level Components

| Component | Responsibility |
|---|---|
| One ESP32 controller | Reads both RFID readers, controls both servos and shared buzzer, reports device events |
| RFID Reader ENTRY | Reads cards presented at the entry lane |
| RFID Reader EXIT | Reads cards presented at the exit lane |
| Servo ENTRY | Moves the entry barrier |
| Servo EXIT | Moves the exit barrier |
| Node-RED | Receives RFID events, applies parking rules, coordinates commands |
| MQTT broker | Carries RFID events, commands, and device status |
| SQLite | Stores cards, parking sessions, gate telemetry, commands, and audit events |
| Node-RED Dashboard | Presents statistics, transaction history, device state, and operator controls |

## 3. Communication Flow

### RFID access

1. A card is presented to either the entry or exit RFID reader.
2. The single ESP32 reads the UID and assigns `gate_id` as `entry` or `exit` according to the reader used.
3. The ESP32 publishes the event to the corresponding MQTT RFID topic.
4. Node-RED validates the card and checks parking-session rules.
5. Node-RED publishes an OPEN command to the matching gate command topic.
6. The ESP32 receives the command and operates only the servo associated with that gate.
7. The ESP32 publishes a status acknowledgement containing the gate ID and command ID.
8. Node-RED stores the event in SQLite and updates the dashboard.

### Manual command

1. An operator requests OPEN or CLOSE for the entry or exit gate in the dashboard.
2. Node-RED records the command and publishes it to the corresponding MQTT topic.
3. The single ESP32 receives the command and routes it to the matching servo using `gate_id`.
4. The ESP32 returns an acknowledgement and reported state.
5. Node-RED updates the command result and dashboard.

A submitted command or firmware acknowledgement is not proof that the physical gate has reached the requested position. The UI must distinguish pending, acknowledged, failed, and timed-out commands.

## 4. MQTT Device Protocol

The one ESP32 uses two lane-specific topic groups:

- `parking/entry/rfid`, `parking/exit/rfid`
- `parking/entry/command`, `parking/exit/command`
- `parking/entry/status`, `parking/exit/status`

Both gate groups use the same device ID: `ESP32_PARKING_01`. The `gate_id` field identifies which reader or servo is involved.

## 5. Gate and Device Identity

Stable identifiers:

- `entry` — entry lane.
- `exit` — exit lane.
- `ESP32_PARKING_01` — the single shared controller.

## 6. Gate State Model

Recommended logical states:

- `open` — firmware reports an OPEN command was issued.
- `closed` — firmware reports a CLOSE command was issued.
- `unknown` — no reliable state is available.
- `offline` — device communication is unavailable.

Without a position sensor, the application can only report the last commanded/reported state. It must not claim that the physical position has been independently verified.

## 7. Operational and Safety Notes

- RFID authentication alone cannot detect whether a vehicle has passed the barrier.
- Without vehicle-presence or position sensors, the prototype cannot provide automatic anti-crush or obstruction detection.
- Remote open/close control must be protected by authentication and authorization.
- Network loss must not be represented as a successful gate operation.
- Two servos require a suitable external power supply; do not power them from the ESP32 3.3 V output. Connect supply ground to ESP32 ground.
- Store timestamps consistently and display them in the Asia/Jakarta timezone.

## 8. Verification Status

The single-controller firmware is a starter implementation. It still requires Arduino compilation, pin/wiring verification, MQTT integration testing, and physical tests with both RFID readers and both servos.
