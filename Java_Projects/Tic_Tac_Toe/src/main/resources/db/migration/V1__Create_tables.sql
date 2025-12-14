-- V1__Create_tables.sql
CREATE TABLE IF NOT EXISTS users (
    id UUID PRIMARY KEY,
    login VARCHAR(255) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL
);

CREATE TABLE IF NOT EXISTS games (
    id UUID PRIMARY KEY,
    player1_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    player2_id UUID REFERENCES users(id) ON DELETE SET NULL,
    state VARCHAR(50) NOT NULL,
    board VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    version INTEGER,
    vs_computer BOOLEAN NOT NULL DEFAULT FALSE

    -- Проверка условий для игроков
    CONSTRAINT check_players_logic CHECK (
        (vs_computer = true AND player2_id IS NULL) OR
        (vs_computer = false)
    )
);
-- Индекс для быстрого поиска доступных игр
--CREATE INDEX IF NOT EXISTS idx_games_waiting ON games(state)
--WHERE state = 'WAITING_FOR_PLAYERS' AND vs_computer = false;

-- Индекс для оптимизации поиска игр по игрокам
--CREATE INDEX IF NOT EXISTS idx_games_player1 ON games(player1_id);
--CREATE INDEX IF NOT EXISTS idx_games_player2 ON games(player2_id)
--WHERE player2_id IS NOT NULL;

CREATE TABLE IF NOT EXISTS moves (
    id UUID PRIMARY KEY,
    game_id UUID REFERENCES games(id),
    player_id UUID REFERENCES users(id),
    row INT NOT NULL,
    col INT NOT NULL,
    symbol VARCHAR(1) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Создаем системного пользователя для ИИ (если не существует)
INSERT INTO users (id, login, password)
VALUES ('00000000-0000-0000-0000-000000000000', 'COMPUTER', 'unused_password')
ON CONFLICT (id) DO NOTHING;