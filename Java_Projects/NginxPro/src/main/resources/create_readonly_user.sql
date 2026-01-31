-- Скрипт создания read-only пользователя для балансировки нагрузки
-- ВАЖНО: Этот файл является шаблоном. Значения будут заменены скриптом create_readonly_user.sh
-- Пользователь и пароль задаются через переменные READONLY_USER и READONLY_USER_PASSWORD из .env
-- Права: только SELECT на все таблицы в базе данных

-- Создание пользователя (если не существует)
DO $$
BEGIN
    IF NOT EXISTS (SELECT FROM pg_user WHERE usename = 'user') THEN
        CREATE USER "user" WITH PASSWORD 'user123';
        RAISE NOTICE 'Пользователь "user" создан';
    ELSE
        RAISE NOTICE 'Пользователь "user" уже существует';
    END IF;
END
$$;

-- Предоставление прав на подключение к базе данных
GRANT CONNECT ON DATABASE myshop_db TO "user";

-- Предоставление прав на использование схемы public
GRANT USAGE ON SCHEMA public TO "user";

-- Предоставление прав SELECT на все существующие таблицы
GRANT SELECT ON ALL TABLES IN SCHEMA public TO "user";

-- Предоставление прав SELECT на все будущие таблицы
ALTER DEFAULT PRIVILEGES IN SCHEMA public
    GRANT SELECT ON TABLES TO "user";

-- Предоставление прав на использование последовательностей (для чтения)
GRANT USAGE ON ALL SEQUENCES IN SCHEMA public TO "user";

-- Предоставление прав на использование будущих последовательностей
ALTER DEFAULT PRIVILEGES IN SCHEMA public
    GRANT USAGE ON SEQUENCES TO "user";

-- Вывод информации о созданном пользователе
SELECT 
    'Пользователь "user" успешно создан с read-only правами' AS status,
    'Права: CONNECT, USAGE, SELECT на все таблицы' AS permissions;
