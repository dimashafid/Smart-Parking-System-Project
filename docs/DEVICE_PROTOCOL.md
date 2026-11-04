# MQTT device protocol

| Topic | Direction | Payload |
|---|---|---|
| parking/entry/rfid, parking/exit/rfid | ESP32 → Node-RED | {uid,gate_id,device_id} |
| parking/entry/command, parking/exit/command | Node-RED → ESP32 | {command_id,command,gate_id} |
| parking/entry/status, parking/exit/status | ESP32 → Node-RED | {device_id,gate_id,state,result} |

Use QoS 1 for important events, do not retain gate commands, authenticate clients and use TLS where possible. Add duplicate/stale command-ID protection and heartbeat monitoring before deployment.
