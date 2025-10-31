-- V1__Create_tables.sql

-- Подключаем расширение pgcrypto для генерации UUID
CREATE EXTENSION IF NOT EXISTS "pgcrypto";

CREATE TABLE IF NOT EXISTS users (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    login VARCHAR(255) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    -- Одна роль USER на пользователя (соответствует требованию задания)
    role VARCHAR(50) NOT NULL DEFAULT 'USER'
);

CREATE TABLE IF NOT EXISTS games (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    player1_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    player2_id UUID REFERENCES users(id) ON DELETE SET NULL,
    state VARCHAR(50) NOT NULL,
    board TEXT NOT NULL DEFAULT '[[0,0,0],[0,0,0],[0,0,0]]',
    previous_board TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    version INTEGER DEFAULT 1,
    vs_computer BOOLEAN NOT NULL DEFAULT FALSE,

    -- Проверка условий для игроков
    CONSTRAINT check_players_logic CHECK (
        (vs_computer = true AND player2_id IS NULL) OR
        (vs_computer = false)
    )
);

-- Индекс для быстрого поиска доступных игр
CREATE INDEX IF NOT EXISTS idx_games_waiting ON games(state)
WHERE state = 'WAITING_FOR_PLAYERS' AND vs_computer = false;

-- Индекс для оптимизации поиска игр по игрокам
CREATE INDEX IF NOT EXISTS idx_games_player1 ON games(player1_id);
CREATE INDEX IF NOT EXISTS idx_games_player2 ON games(player2_id)
WHERE player2_id IS NOT NULL;

-- Индекс для поиска активных игр по игрокам
CREATE INDEX IF NOT EXISTS idx_games_active ON games(state)
WHERE state IN ('WAITING_FOR_PLAYERS', 'PLAYER_1_TURN', 'PLAYER_2_TURN');

CREATE TABLE IF NOT EXISTS moves (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    game_id UUID NOT NULL REFERENCES games(id) ON DELETE CASCADE,
    player_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    row INT NOT NULL CHECK (row BETWEEN 0 AND 2),
    col INT NOT NULL CHECK (col BETWEEN 0 AND 2),
    symbol INT NOT NULL CHECK (symbol IN (1, 2)),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    -- Уникальность хода в игре
    UNIQUE(game_id, row, col)
);

-- Создаем системного пользователя для ИИ
INSERT INTO users (id, login, password, role)
VALUES ('00000000-0000-0000-0000-000000000000', 'COMPUTER', 'unused_password', 'USER')
ON CONFLICT (id) DO NOTHING;

-- Создаем пользователя по умолчанию
INSERT INTO users (id, login, password, role)
VALUES (gen_random_uuid(), 'default_user', '$2a$10$defaultEncodedPassword', 'USER')
ON CONFLICT (login) DO NOTHING;

-- Предоставление прав пользователю java_user на все таблицы
GRANT SELECT, INSERT, UPDATE, DELETE ON ALL TABLES IN SCHEMA public TO java_user;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO java_user;

-- Комментарии к таблицам
COMMENT ON TABLE users IS 'Таблица пользователей системы';
COMMENT ON TABLE games IS 'Таблица игр в крестики-нолики';
COMMENT ON TABLE moves IS 'Таблица ходов в играх (история ходов)';