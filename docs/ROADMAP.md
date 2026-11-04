# Development Roadmap

This roadmap tracks planned work. Code deliverables may be marked complete when added; deployment, runtime, and hardware verification remain separate checklist items.

## Phase 1 — Project Foundation

- [x] Create repository and initial documentation.
- [x] Select Node-RED as the flow runtime.
- [x] Select Node-RED dashboard and MQTT topic contract.
- [x] Set hardware architecture to one ESP32 controlling two lanes.
- [ ] Confirm exact board, pin mapping, power supply, and wiring with the team.

## Phase 2 — Node-RED and Database

- [x] Add SQLite schema and Node-RED flow.
- [x] Add RFID registration, validation, and parking session logic (runtime test pending).
- [x] Add summary, history, and gate status endpoints.
- [x] Add manual gate command endpoint and MQTT publishing (device acknowledgement verification pending).
- [ ] Configure and verify the flow in the team's Node-RED runtime.
- [ ] Add authentication and authorization.

## Phase 3 — Dashboard

- [x] Add responsive overview dashboard.
- [x] Add separate entry and exit status panels.
- [x] Add RFID registration and manual gate controls.
- [ ] Test dashboard on desktop and mobile.
- [ ] Add robust pending, error, and offline states.

## Phase 4 — Single ESP32 Hardware Integration

- [x] Add one combined ESP32 starter sketch for both RFID readers and both servos.
- [x] Define lane-specific MQTT topics and shared device identity.
- [ ] Compile firmware in Arduino IDE.
- [ ] Verify two RC522 readers sharing SPI with separate SS pins.
- [ ] Verify two servo outputs and external servo power.
- [ ] Test entry RFID scan and entry servo command.
- [ ] Test exit RFID scan and exit servo command.
- [ ] Test both lanes operating without topic or pin conflicts.
- [ ] Verify MQTT reconnect and command acknowledgement behavior.

## Phase 5 — Validation and Demonstration

- [ ] Test valid and invalid RFID cards.
- [ ] Test repeated scans and duplicate network requests.
- [ ] Test entry/exit session consistency.
- [ ] Test dashboard commands for both gates.
- [ ] Test device/network failure scenarios.
- [ ] Document known limitations and prepare demonstration.

## Completion Criteria

The prototype is ready for demonstration when one ESP32 can independently read both lane RFID readers, operate the corresponding servo for each lane, exchange events and commands with Node-RED, and show transaction history and gate status in the dashboard.
