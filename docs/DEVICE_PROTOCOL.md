# MQTT device protocol — one ESP32, two gates

One physical ESP32 controller (`ESP32_PARKING_01`) handles both entry and exit lanes. The lane is identified by `gate_id`, not by a separate microcontroller.

| Topic | Direction | Payload fields |
|---|---|---|
| `parking/entry/rfid` | ESP32 → Node-RED | `uid, gate_id=entry, device_id=ESP32_PARKING_01` |
| `parking/exit/rfid` | ESP32 → Node-RED | `uid, gate_id=exit, device_id=ESP32_PARKING_01` |
| `parking/entry/command` | Node-RED → ESP32 | `command_id, command, gate_id=entry` |
| `parking/exit/command` | Node-RED → ESP32 | `command_id, command, gate_id=exit` |
| `parking/entry/status` | ESP32 → Node-RED | `device_id, gate_id=entry, state, result, command_id` |
| `parking/exit/status` | ESP32 → Node-RED | `device_id, gate_id=exit, state, result, command_id` |

The ESP32 subscribes to `parking/+/command`, validates that the topic lane matches the payload `gate_id`, and routes the command to the matching servo.

Use QoS 1 for important events, do not retain gate commands, authenticate clients and use TLS where possible. Duplicate/stale command-ID protection and heartbeat monitoring should be completed before deployment.

An `EXECUTED` acknowledgement means the firmware issued the servo command; without position feedback it does not prove the physical gate reached its target.
