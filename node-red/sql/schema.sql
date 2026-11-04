PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS rfid_cards (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    uid TEXT NOT NULL UNIQUE COLLATE NOCASE,
    owner_name TEXT,
    vehicle_plate TEXT,
    is_active INTEGER NOT NULL DEFAULT 1 CHECK (is_active IN (0, 1)),
    created_at TEXT NOT NULL DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ', 'now'))
);

CREATE TABLE IF NOT EXISTS access_events (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    gate_id TEXT NOT NULL CHECK (gate_id IN ('entry', 'exit')),
    card_uid TEXT NOT NULL,
    event_type TEXT NOT NULL DEFAULT 'rfid_scan',
    created_at TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS parking_sessions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    card_uid TEXT NOT NULL,
    vehicle_plate TEXT,
    entry_gate TEXT NOT NULL DEFAULT 'entry',
    entry_at TEXT NOT NULL,
    exit_gate TEXT,
    exit_at TEXT,
    status TEXT NOT NULL DEFAULT 'inside'
        CHECK (status IN ('inside', 'completed', 'invalid')),
    CHECK ((status = 'inside' AND exit_at IS NULL) OR
           (status = 'completed' AND exit_at IS NOT NULL) OR
           status = 'invalid')
);

CREATE UNIQUE INDEX IF NOT EXISTS idx_one_open_session_per_card
    ON parking_sessions(card_uid) WHERE status = 'inside';

CREATE INDEX IF NOT EXISTS idx_access_events_created_at
    ON access_events(created_at DESC);

CREATE INDEX IF NOT EXISTS idx_parking_sessions_status
    ON parking_sessions(status);

CREATE TABLE IF NOT EXISTS gate_status (
    gate_id TEXT PRIMARY KEY CHECK (gate_id IN ('entry', 'exit')),
    reported_state TEXT NOT NULL DEFAULT 'unknown'
        CHECK (reported_state IN ('open', 'closed', 'moving', 'unknown')),
    connected INTEGER NOT NULL DEFAULT 0 CHECK (connected IN (0, 1)),
    last_seen_at TEXT
);

INSERT OR IGNORE INTO gate_status (gate_id) VALUES ('entry'), ('exit');

CREATE TABLE IF NOT EXISTS gate_commands (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    gate_id TEXT NOT NULL CHECK (gate_id IN ('entry', 'exit')),
    command TEXT NOT NULL CHECK (command IN ('open', 'close')),
    status TEXT NOT NULL DEFAULT 'pending'
        CHECK (status IN ('pending', 'acknowledged', 'failed', 'timeout')),
    requested_at TEXT NOT NULL,
    acknowledged_at TEXT
);
