# Development Roadmap

This roadmap tracks the planned work for the Smart Parking System. Items should only be marked complete after implementation and verification.

## Phase 1 — Project Foundation

- [x] Create public GitHub repository.
- [x] Write initial README and project documentation.
- [ ] Confirm team members and responsibilities.
- [ ] Confirm final hardware list and wiring plan.
- [x] Select Node-RED as the initial flow runtime.\n- [ ] Confirm Node-RED dashboard package and ESP32 communication protocol.
- [ ] Agree on naming conventions and API payload format.

## Phase 2 — Node-RED and Database

- [x] Add initial SQLite schema.\n- [x] Add starter Node-RED HTTP and SQLite flow.\n- [ ] Configure and verify the flow in the team's Node-RED runtime.
- [ ] Add RFID card registration and validation.
- [ ] Implement registered-card validation and entry event processing.
- [ ] Implement exit event processing and parking-session closure.
- [ ] Prevent duplicate event processing.
- [ ] Implement parking summary and history endpoints.
- [ ] Implement gate command lifecycle.
- [ ] Add authentication and authorization for dashboard controls.

## Phase 3 — Virtual Device Simulation

- [ ] Simulate entry and exit ESP32 devices.
- [ ] Simulate registered and unknown RFID cards.
- [ ] Simulate gate open and close acknowledgements.
- [ ] Simulate device disconnection and command timeout.
- [ ] Verify that simulator events are stored in SQLite.

## Phase 4 — Web Dashboard

- [ ] Build Node-RED dashboard overview.
- [ ] Build separate entry and exit gate monitoring panels in the selected Node-RED dashboard.
- [ ] Build parking history with date/time filters.
- [ ] Build RFID card management.
- [ ] Build emergency control interface.
- [ ] Display device connectivity and last-seen timestamps.
- [ ] Add loading, error, pending, and offline states.

## Phase 5 — ESP32 and Hardware Integration

- [ ] Implement entry-gate firmware.
- [ ] Implement exit-gate firmware.
- [ ] Connect and test RFID readers.
- [ ] Connect and test actuators.
- [ ] Add buzzer feedback for access results.
- [ ] Connect devices to Node-RED using the agreed MQTT or HTTP protocol.
- [ ] Test power stability and restart behavior.
- [ ] Verify reported gate state against the available hardware feedback.

## Phase 6 — Validation and Demonstration

- [ ] Test valid and invalid RFID cards.
- [ ] Test repeated scans and duplicate network requests.
- [ ] Test entry/exit session consistency.
- [ ] Test simultaneous events at both gates.
- [ ] Test dashboard control acknowledgement and timeout.
- [ ] Test device and network failure scenarios.
- [ ] Document known limitations.
- [ ] Prepare screenshots, demo scenario, and final report.

## Completion Criteria

The project can be considered ready for demonstration when a user can register or identify an RFID card, simulate or perform entry and exit events, view accurate transaction history and parking counts, monitor both gate devices, and issue authorized gate commands with visible acknowledgement and failure handling.
