# Development Roadmap

This roadmap tracks the planned work for the Smart Parking System. Code deliverables may be marked complete when added; deployment, runtime, and hardware verification remain separate checklist items.

## Phase 1 — Project Foundation

- [x] Create public GitHub repository.
- [x] Write initial README and project documentation.
- [ ] Confirm team members and responsibilities.
- [ ] Confirm final hardware list and wiring plan.
- [x] Select Node-RED as the initial flow runtime.\n- [x] Select Node-RED HTTP template dashboard and MQTT topic contract.
- [ ] Agree on naming conventions and API payload format.

## Phase 2 — Node-RED and Database

- [x] Add initial SQLite schema.\n- [x] Add starter Node-RED HTTP and SQLite flow.\n- [ ] Configure and verify the flow in the team's Node-RED runtime.
- [x] Add RFID card registration endpoint and dashboard form.
- [x] Implement active-card validation and entry session trigger (runtime test pending).
- [x] Implement exit validation and session closure trigger (runtime test pending).
- [ ] Prevent duplicate event processing.
- [x] Add parking summary and history endpoints.
- [x] Add manual/automatic command recording and MQTT publishing (device acknowledgement verification pending).
- [ ] Add authentication and authorization for dashboard controls.

## Phase 3 — Virtual Device Simulation

- [ ] Simulate entry and exit ESP32 devices.
- [ ] Simulate registered and unknown RFID cards.
- [ ] Simulate gate open and close acknowledgements.
- [ ] Simulate device disconnection and command timeout.
- [ ] Verify that simulator events are stored in SQLite.

## Phase 4 — Web Dashboard

- [x] Add responsive overview dashboard page.
- [x] Add entry and exit gate monitoring panels.
- [ ] Build parking history with date/time filters.
- [x] Add RFID card registration and list view.
- [x] Add manual open/close controls (authorization and safety hardening pending).
- [ ] Display device connectivity and last-seen timestamps.
- [ ] Add loading, error, pending, and offline states.

## Phase 5 — ESP32 and Hardware Integration

- [x] Add starter entry-gate ESP32 sketch (hardware test pending).
- [x] Add starter exit-gate ESP32 sketch (hardware test pending).
- [ ] Connect and test RFID readers.
- [ ] Connect and test actuators.
- [ ] Add buzzer feedback for access results.
- [x] Define MQTT topic and payload contract; physical integration pending.
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
