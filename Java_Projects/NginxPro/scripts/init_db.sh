#!/bin/bash

# Инициализация скрипта для работы с базой данных PostgreSQL
# Использует переменные окружения из .env файла (без вывода секретов)

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}🗄️  Скрипт инициализации базы данных PostgreSQL${NC}"

# ============================================================================
# 1. БЕЗОПАСНАЯ ЗАГРУЗКА ПЕРЕМЕННЫХ ОКРУЖЕНИЯ
# ============================================================================

# Функция безопасной загрузки переменных из .env файла
load_env_variables() {
    local env_file="$(dirname "$0")/../.env"

    if [ -f "$env_file" ]; then
        echo -e "${GREEN}📄 Загружаю переменные из .env файла${NC}"

        # Безопасная загрузка без вывода значений
        while IFS='=' read -r key value || [ -n "$key" ]; do
            # Пропускаем комментарии и пустые строки
            if [[ ! $key =~ ^[[:space:]]*# ]] && [[ -n $key ]] && [[ $key =~ ^[A-Z_]+$ ]]; then
                key=$(echo "$key" | xargs)
                value=$(echo "$value" | sed 's/^"//; s/"$//; s/^'"'"'//; s/'"'"'$//')
                # Удаляем кавычки если есть
                value="${value%\"}"
                value="${value#\"}"
                value="${value%\'}"
                value="${value#\'}"
                export "$key"="$value"
                # Выводим только название переменной (без значения)
                echo -e "   ${GREEN}✓${NC} ${key}"
            fi
        done < "$env_file"
        echo -e "${GREEN}✅ Все переменные загружены${NC}"
    else
        echo -e "${YELLOW}⚠️  Файл .env не найден${NC}"
        echo -e "   Использую значения по умолчанию"
    fi
}

# Загружаем переменные
load_env_variables

# ============================================================================
# 2. УСТАНОВКА ПЕРЕМЕННЫХ С ЗНАЧЕНИЯМИ ПО УМОЛЧАНИЮ
# ============================================================================

# Основные переменные для подключения к приложению
DATABASE_HOST="${DATABASE_HOST:-localhost}"
DATABASE_PORT="${DATABASE_PORT:-5432}"
DATABASE_NAME="${DATABASE_NAME:-myshop_db}"
DATABASE_USERNAME="${DATABASE_USERNAME}"
DATABASE_PASSWORD="${DATABASE_PASSWORD}"

# Переменные для административного подключения
PG_HOST="${PG_HOST:-$DATABASE_HOST}"
PG_PORT="${PG_PORT:-$DATABASE_PORT}"
PG_USER="${PG_USER:-postgres}"
PG_PASSWORD="${PG_PASSWORD}"

# Путь к SQL скриптам (относительно проекта)
PROJECT_DIR="$(dirname "$0")/.."
SCHEMA_FILE="${PROJECT_DIR}/src/main/resources/schema.sql"
DATA_FILE="${PROJECT_DIR}/src/main/resources/data.sql"

# Устанавливаем пароль для psql (не выводим в консоль)
if [ -n "$PG_PASSWORD" ]; then
    export PGPASSWORD="$PG_PASSWORD"
fi

# ============================================================================
# 3. БЕЗОПАСНЫЙ ВЫВОД КОНФИГУРАЦИИ
# ============================================================================

echo -e "\n${BLUE}📊 Конфигурация подключения (безопасный режим):${NC}"
echo -e "   ${YELLOW}Для приложения:${NC}"
echo -e "     📍 Хост: ${DATABASE_HOST}:${DATABASE_PORT}"
echo -e "     📦 База: ${DATABASE_NAME}"
echo -e "     👤 Пользователь: ${DATABASE_USERNAME}"
echo -e "     🔑 Пароль: ${GREEN}[скрыто]${NC}"
echo -e "   ${YELLOW}Для администрирования:${NC}"
echo -e "     📍 Хост: ${PG_HOST}:${PG_PORT}"
echo -e "     👤 Пользователь: ${PG_USER}"
echo -e "     🔑 Пароль: ${GREEN}[скрыто]${NC}"

# ============================================================================
# 4. ФУНКЦИИ ПРОВЕРКИ (БЕЗОПАСНЫЕ)
# ============================================================================

# Проверка установки PostgreSQL
check_postgres_installed() {
    if ! command -v psql &> /dev/null; then
        echo -e "${RED}❌ PostgreSQL не установлен${NC}"
        echo -e "   Установите: sudo apt-get install postgresql"
        exit 1
    fi
    echo -e "${GREEN}✅ PostgreSQL установлен${NC}"
}

# Проверка запуска PostgreSQL
check_postgres_running() {
    echo -e "\n🔍 Проверка доступности PostgreSQL..."

    if ! pg_isready -h "$PG_HOST" -p "$PG_PORT" -q; then
        echo -e "${RED}❌ PostgreSQL не запущен${NC}"
        echo -e "   Запустите: sudo systemctl start postgresql"
        exit 1
    fi
    echo -e "${GREEN}✅ PostgreSQL доступен${NC}"
}

# Проверка подключения с административными правами (без вывода пароля)
check_admin_connection() {
    echo -e "\n🔑 Проверка административного подключения..."

    # Пытаемся подключиться без вывода пароля
    if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres -c "SELECT 1;" &>/dev/null; then
        echo -e "${GREEN}✅ Административное подключение успешно${NC}"
        return 0
    else
        echo -e "${YELLOW}⚠️  Не удалось подключиться${NC}"

        # Пробуем использовать sudo для peer auth
        if sudo -u postgres psql -c "SELECT 1;" &>/dev/null; then
            echo -e "${GREEN}✅ Подключение через sudo успешно${NC}"
            PG_USER="postgres"
            unset PGPASSWORD
            return 0
        fi

        echo -e "${RED}❌ Не удалось подключиться к PostgreSQL${NC}"
        echo -e "   Проверьте:"
        echo -e "   1. Правильность пароля в .env файле"
        echo -e "   2. Настройки аутентификации в pg_hba.conf"
        exit 1
    fi
}

# Проверка существования SQL файлов
check_sql_files() {
    echo -e "\n📋 Проверка SQL файлов..."

    if [ ! -f "$SCHEMA_FILE" ]; then
        echo -e "${YELLOW}⚠️  Файл схемы не найден: $SCHEMA_FILE${NC}"
        return 1
    else
        echo -e "${GREEN}✅ Файл схемы найден${NC}"
    fi

    if [ ! -f "$DATA_FILE" ]; then
        echo -e "${YELLOW}⚠️  Файл данных не найден: $DATA_FILE${NC}"
        return 1
    else
        echo -e "${GREEN}✅ Файл данных найден${NC}"
    fi

    return 0
}

# ============================================================================
# 5. ОСНОВНЫЕ ФУНКЦИИ (БЕЗОПАСНЫЕ)
# ============================================================================

# Инициализация базы данных
init_db() {
    echo -e "\n${BLUE}🚀 Начинаю инициализацию базы данных...${NC}"

    check_postgres_installed
    check_postgres_running
    check_admin_connection
    check_sql_files

    # Проверка существования базы данных
    echo -e "\n📦 Проверка базы данных..."
    if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres -tAc "SELECT 1 FROM pg_database WHERE datname = '$DATABASE_NAME'" | grep -q 1; then
        echo -e "${YELLOW}⚠️  База данных уже существует${NC}"
        read -p "Пересоздать базу данных? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            echo -e "Отключение активных подключений..."
            # Отключаем ВСЕ активные подключения к базе
            psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres \
                -c "SELECT pg_terminate_backend(pid) FROM pg_stat_activity WHERE datname = '$DATABASE_NAME' AND pid <> pg_backend_pid();" 2>/dev/null || true

            echo -e "Удаление существующей базы..."
            if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres -c "DROP DATABASE IF EXISTS $DATABASE_NAME;" 2>&1; then
                echo -e "${GREEN}✅ База данных удалена${NC}"
            else
                echo -e "${RED}❌ Ошибка при удалении базы данных${NC}"
                echo -e "   Попробуйте удалить базу вручную:"
                echo -e "   sudo -u postgres psql -c \"DROP DATABASE IF EXISTS $DATABASE_NAME;\""
                exit 1
            fi

            echo -e "Создание новой базы данных..."
            if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres -c "CREATE DATABASE $DATABASE_NAME WITH ENCODING='UTF8' LC_COLLATE='C.UTF-8' LC_CTYPE='C.UTF-8';" 2>&1; then
                echo -e "${GREEN}✅ База данных создана${NC}"
            else
                echo -e "${RED}❌ Ошибка при создании базы данных${NC}"
                exit 1
            fi
        else
            echo -e "${YELLOW}⚠️  Используем существующую базу данных${NC}"
            # Проверяем, что можем подключиться к существующей базе
            if ! psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d "$DATABASE_NAME" -c "SELECT 1;" &>/dev/null; then
                echo -e "${RED}❌ Не могу подключиться к существующей базе $DATABASE_NAME${NC}"
                exit 1
            fi
        fi
    else
        echo -e "Создание базы данных..."
        if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres -c "CREATE DATABASE $DATABASE_NAME WITH ENCODING='UTF8' LC_COLLATE='C.UTF-8' LC_CTYPE='C.UTF-8';" 2>&1; then
            echo -e "${GREEN}✅ База данных создана${NC}"
        else
            echo -e "${RED}❌ Ошибка при создании базы данных${NC}"
            exit 1
        fi
    fi
    # Проверка существования пользователя
    echo -e "\n👤 Проверка пользователя..."
    if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres -tAc "SELECT 1 FROM pg_roles WHERE rolname = '$DATABASE_USERNAME'" | grep -q 1; then
        echo -e "${YELLOW}⚠️  Пользователь уже существует${NC}"
    else
        echo -e "Создание пользователя..."
        if [ -n "$DATABASE_PASSWORD" ]; then
            # Создаем пользователя с паролем (пароль не выводится)
            if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres -c "CREATE USER $DATABASE_USERNAME WITH PASSWORD '$DATABASE_PASSWORD';" &>/dev/null; then
                echo -e "${GREEN}✅ Пользователь создан${NC}"
            else
                echo -e "${RED}❌ Ошибка при создании пользователя${NC}"
                exit 1
            fi
        else
            echo -e "${YELLOW}⚠️  Создание пользователя без пароля${NC}"
            if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres -c "CREATE USER $DATABASE_USERNAME;" &>/dev/null; then
                echo -e "${GREEN}✅ Пользователь создан${NC}"
            else
                echo -e "${RED}❌ Ошибка при создании пользователя${NC}"
                exit 1
            fi
        fi
    fi

    # БАЗОВЫЕ ПРАВА (безопасный минимум)
    echo -e "\n🔑 Настройка базовых прав доступа..."

    # Даем права на базу данных
    psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres \
        -c "GRANT CONNECT, TEMPORARY ON DATABASE $DATABASE_NAME TO $DATABASE_USERNAME;" &>/dev/null || {
        echo -e "${YELLOW}⚠️  Не удалось установить права на базу${NC}"
    }

    # СОЗДАНИЕ СХЕМЫ И ТАБЛИЦ
    echo -e "\n🏗️  Создание схемы и таблиц..."

    if [ -f "$SCHEMA_FILE" ]; then
        echo -e "Выполнение schema.sql..."
        if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d "$DATABASE_NAME" -f "$SCHEMA_FILE" &>/dev/null; then
            echo -e "${GREEN}✅ Схема создана${NC}"
        else
            echo -e "${YELLOW}⚠️  Предупреждение при создании схемы${NC}"
            echo -e "   (Некоторые объекты могли уже существовать)"
        fi
    else
        echo -e "${RED}❌ Файл схемы не найден: $SCHEMA_FILE${NC}"
        exit 1
    fi

    # ПОЛНЫЕ ПРАВА НА СУЩЕСТВУЮЩИЕ ТАБЛИЦЫ
    echo -e "\n🔐 Настройка полных прав на существующие таблицы..."

    # Даем права на схему public
    psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d "$DATABASE_NAME" \
        -c "GRANT USAGE ON SCHEMA public TO $DATABASE_USERNAME;" &>/dev/null || true

    # Даем права на ВСЕ существующие таблицы
    psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d "$DATABASE_NAME" << EOF &>/dev/null || true
GRANT SELECT, INSERT, UPDATE, DELETE ON ALL TABLES IN SCHEMA public TO $DATABASE_USERNAME;
GRANT USAGE ON ALL SEQUENCES IN SCHEMA public TO $DATABASE_USERNAME;
GRANT EXECUTE ON ALL FUNCTIONS IN SCHEMA public TO $DATABASE_USERNAME;
EOF

    # Настраиваем права для БУДУЩИХ таблиц
    psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d "$DATABASE_NAME" << EOF &>/dev/null || true
ALTER DEFAULT PRIVILEGES IN SCHEMA public
GRANT SELECT, INSERT, UPDATE, DELETE ON TABLES TO $DATABASE_USERNAME;

ALTER DEFAULT PRIVILEGES IN SCHEMA public
GRANT USAGE ON SEQUENCES TO $DATABASE_USERNAME;

ALTER DEFAULT PRIVILEGES IN SCHEMA public
GRANT EXECUTE ON FUNCTIONS TO $DATABASE_USERNAME;
EOF

    # ЗАГРУЗКА ТЕСТОВЫХ ДАННЫХ
    echo -e "\n📊 Загрузка тестовых данных..."

    if [ -f "$DATA_FILE" ]; then
        echo -e "Выполнение data.sql..."
        if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d "$DATABASE_NAME" -f "$DATA_FILE" &>/dev/null; then
            echo -e "${GREEN}✅ Тестовые данные загружены${NC}"
        else
            echo -e "${YELLOW}⚠️  Предупреждение при загрузке данных${NC}"
            echo -e "   (Некоторые данные могли уже существовать)"
        fi
    else
        echo -e "${YELLOW}⚠️  Файл данных не найден, пропускаем${NC}"
    fi

    echo -e "\n${GREEN}🎉 База данных успешно инициализирована!${NC}"
    echo -e "   📊 Имя базы: ${DATABASE_NAME}"
    echo -e "   👤 Пользователь: ${DATABASE_USERNAME}"
    echo -e "   📍 Хост: ${DATABASE_HOST}:${DATABASE_PORT}"
    echo -e "   🔐 Права: SELECT, INSERT, UPDATE, DELETE"
    echo -e "\n${YELLOW}Теперь можно запускать Spring Boot приложение:${NC}"
    echo -e "   ${BLUE}./gradlew bootRun${NC}"
    echo -e "\n${YELLOW}Не забудьте в application.properties установить:${NC}"
    echo -e "   ${BLUE}spring.sql.init.mode=never${NC}"
}

# Очистка базы данных
clean_db() {
    echo -e "\n${RED}🗑️  Начинаю очистку базы данных...${NC}"

    check_postgres_installed
    check_postgres_running
    check_admin_connection

    # Запрос подтверждения для безопасности
    echo -e "${YELLOW}⚠️  ВНИМАНИЕ: Будут удалены все данные!${NC}"
    read -p "Вы уверены, что хотите удалить базу '$DATABASE_NAME'? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo -e "${GREEN}✅ Отмена операции${NC}"
        exit 0
    fi

    # Отключение всех активных подключений
    echo -e "\n🔌 Отключение активных подключений..."
    psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres \
        -c "SELECT pg_terminate_backend(pid) FROM pg_stat_activity WHERE datname = '$DATABASE_NAME' AND pid <> pg_backend_pid();" &>/dev/null || true

    # Удаление базы данных
    echo -e "\nУдаление базы данных..."
    if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres -c "DROP DATABASE IF EXISTS $DATABASE_NAME;" &>/dev/null; then
        echo -e "${GREEN}✅ База данных удалена${NC}"
    else
        echo -e "${RED}❌ Ошибка при удалении базы данных${NC}"
        exit 1
    fi

    # Удаление пользователя (если не postgres и не используется другими БД)
    if [ "$DATABASE_USERNAME" != "postgres" ]; then
        echo -e "\n👤 Проверка использования пользователя..."
        USER_DBS=$(psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres \
            -tAc "SELECT COUNT(*) FROM pg_database WHERE datdba = (SELECT oid FROM pg_roles WHERE rolname = '$DATABASE_USERNAME');")

        if [ "$USER_DBS" -eq "0" ]; then
            echo -e "Удаление пользователя..."
            if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres -c "DROP USER IF EXISTS $DATABASE_USERNAME;" &>/dev/null; then
                echo -e "${GREEN}✅ Пользователь удален${NC}"
            else
                echo -e "${YELLOW}⚠️  Не удалось удалить пользователя${NC}"
            fi
        else
            echo -e "${YELLOW}⚠️  Пользователь используется другими БД, не удаляем${NC}"
        fi
    fi

    echo -e "\n${GREEN}✅ Очистка завершена${NC}"
}

# Проверка подключения (безопасная)
check_connection() {
    echo -e "\n${BLUE}🔗 Проверка подключения к базе данных...${NC}"

    check_postgres_installed
    check_postgres_running

    # Устанавливаем пароль для проверки
    if [ -n "$DATABASE_PASSWORD" ]; then
        export PGPASSWORD="$DATABASE_PASSWORD"
    fi

    echo -e "\n🔍 Проверка подключения приложения..."
    if psql -h "$DATABASE_HOST" -p "$DATABASE_PORT" -U "$DATABASE_USERNAME" -d "$DATABASE_NAME" -c "SELECT '✅' AS status;" &>/dev/null; then
        echo -e "${GREEN}✅ Подключение к БД успешно!${NC}"

        # Проверка прав
        echo -e "\n🔐 Проверка прав доступа..."
        if psql -h "$DATABASE_HOST" -p "$DATABASE_PORT" -U "$DATABASE_USERNAME" -d "$DATABASE_NAME" \
            -c "SELECT '✅' AS status FROM information_schema.tables WHERE table_schema = 'public' LIMIT 1;" &>/dev/null; then
            echo -e "${GREEN}✅ Права доступа корректны${NC}"
        else
            echo -e "${YELLOW}⚠️  Возможны проблемы с правами доступа${NC}"
        fi
    else
        echo -e "\n${RED}❌ Не удалось подключиться к БД${NC}"
        echo -e "   Возможные причины:"
        echo -e "   1. База данных не существует"
        echo -e "   2. Пользователь не существует"
        echo -e "   3. Неправильный пароль"
        echo -e "   4. Хост недоступен"
        echo -e "\n${YELLOW}Попробуйте выполнить:${NC}"
        echo -e "   ${BLUE}$0 init${NC} - для создания БД"
        exit 1
    fi
}

# ============================================================================
# 6. ОБРАБОТКА АРГУМЕНТОВ
# ============================================================================

case "$1" in
    init)
        init_db
        ;;
    clean)
        clean_db
        ;;
    check)
        check_connection
        ;;
    *)
        echo -e "${BLUE}Использование: $0 {init|clean|check}${NC}"
        echo -e "  ${GREEN}init${NC}   - создать/настроить БД (включая схему и данные)"
        echo -e "  ${RED}clean${NC}  - удалить БД (с подтверждением)"
        echo -e "  ${BLUE}check${NC}  - проверить подключение и права"
        echo -e "\n${YELLOW}Переменные окружения загружаются из .env файла${NC}"
        echo -e "${YELLOW}Требуется установленный и запущенный PostgreSQL${NC}"
        exit 1
        ;;
esac