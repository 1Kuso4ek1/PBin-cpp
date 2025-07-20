CREATE TABLE pastes (
    id TEXT PRIMARY KEY,
    text TEXT NOT NULL,
    language TEXT DEFAULT 'plaintext',
    created TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    expires TIMESTAMPTZ,
    viewCount INTEGER NOT NULL DEFAULT 0,
    ipAddress INET,
    deleteToken TEXT
);

CREATE INDEX idx_expiration ON pastes (expires);
