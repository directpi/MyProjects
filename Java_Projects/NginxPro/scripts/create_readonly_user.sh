#!/bin/bash

# Скрипт создания read-only пользователя БД для балансировки нагрузки
# Использует переменные окружения из .env файла (без вывода секретов)

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}👤 Скрипт создания read-only пользователя PostgreSQL${NC}"

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

# Переменные для административного подключения
PG_HOST="${PG_HOST:-$DATABASE_HOST}"
PG_PORT="${PG_PORT:-$DATABASE_PORT}"
PG_USER="${PG_USER:-postgres}"
PG_PASSWORD="${PG_PASSWORD}"

# Учетные данные read-only пользователя из .env (обязательные)
if [ -z "$READONLY_USER" ]; then
    echo -e "${RED}❌ Ошибка: переменная READONLY_USER не задана в .env файле${NC}"
    echo -e "   Добавьте в .env: ${BLUE}READONLY_USER=user${NC}"
    exit 1
fi

if [ -z "$READONLY_USER_PASSWORD" ]; then
    echo -e "${RED}❌ Ошибка: переменная READONLY_USER_PASSWORD не задана в .env файле${NC}"
    echo -e "   Добавьте в .env: ${BLUE}READONLY_USER_PASSWORD=your_password${NC}"
    exit 1
fi

# Путь к SQL скрипту
SCRIPT_DIR="$(dirname "$0")"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
SQL_FILE="${PROJECT_DIR}/src/main/resources/create_readonly_user.sql"

# Устанавливаем пароль для psql (не выводим в консоль)
if [ -n "$PG_PASSWORD" ]; then
    export PGPASSWORD="$PG_PASSWORD"
fi

# ============================================================================
# 3. БЕЗОПАСНЫЙ ВЫВОД КОНФИГУРАЦИИ
# ============================================================================

echo -e "\n${BLUE}📊 Конфигурация подключения (безопасный режим):${NC}"
echo -e "   ${YELLOW}Для администрирования:${NC}"
echo -e "     📍 Хост: ${PG_HOST}:${PG_PORT}"
echo -e "     👤 Пользователь: ${PG_USER}"
echo -e "     🔑 Пароль: ${GREEN}[скрыто]${NC}"
echo -e "   ${YELLOW}Создаваемый read-only пользователь:${NC}"
echo -e "     👤 Имя: ${READONLY_USER}"
echo -e "     🔑 Пароль: ${GREEN}[скрыто]${NC}"
echo -e "     📦 База данных: ${DATABASE_NAME}"
echo -e "     🔐 Права: SELECT (только чтение)"

# ============================================================================
# 4. ФУНКЦИИ ПРОВЕРКИ
# ============================================================================

# Проверка установки PostgreSQL
check_postgres_installed() {
    if ! command -v psql &> /dev/null; then
        echo -e "${RED}❌ PostgreSQL не установлен${NC}"
        echo -e "   Установите PostgreSQL: sudo apt install postgresql"
        exit 1
    else
        echo -e "${GREEN}✅ PostgreSQL установлен${NC}"
    fi
}

# Проверка запуска PostgreSQL
check_postgres_running() {
    if ! pg_isready -h "$PG_HOST" -p "$PG_PORT" &>/dev/null; then
        echo -e "${RED}❌ PostgreSQL не запущен${NC}"
        echo -e "   Запустите PostgreSQL: sudo systemctl start postgresql"
        exit 1
    else
        echo -e "${GREEN}✅ PostgreSQL запущен${NC}"
    fi
}

# Проверка административного подключения
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

# Проверка существования SQL файла
check_sql_file() {
    echo -e "\n📋 Проверка SQL файла..."

    if [ ! -f "$SQL_FILE" ]; then
        echo -e "${RED}❌ Файл SQL не найден: $SQL_FILE${NC}"
        exit 1
    else
        echo -e "${GREEN}✅ SQL файл найден${NC}"
    fi
}

# Проверка существования базы данных
check_database_exists() {
    echo -e "\n📦 Проверка базы данных..."
    if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d postgres -tAc "SELECT 1 FROM pg_database WHERE datname = '$DATABASE_NAME'" | grep -q 1; then
        echo -e "${GREEN}✅ База данных существует${NC}"
        return 0
    else
        echo -e "${RED}❌ База данных не найдена: $DATABASE_NAME${NC}"
        echo -e "   Сначала выполните: ./scripts/init_db.sh"
        exit 1
    fi
}

# ============================================================================
# 5. ОСНОВНАЯ ФУНКЦИЯ
# ============================================================================

# Создание read-only пользователя
create_readonly_user() {
    echo -e "\n${BLUE}🚀 Начинаю создание read-only пользователя...${NC}"

    check_postgres_installed
    check_postgres_running
    check_admin_connection
    check_sql_file
    check_database_exists

    # Создание временного SQL файла с подстановкой переменных
    TEMP_SQL_FILE=$(mktemp)
    # Экранируем специальные символы для sed
    READONLY_USER_ESC=$(printf '%s\n' "$READONLY_USER" | sed 's/[[\.*^$()+?{|]/\\&/g')
    READONLY_PASSWORD_ESC=$(printf '%s\n' "$READONLY_USER_PASSWORD" | sed 's/[[\.*^$()+?{|]/\\&/g')
    DATABASE_NAME_ESC=$(printf '%s\n' "$DATABASE_NAME" | sed 's/[[\.*^$()+?{|]/\\&/g')
    
    sed -e "s/'user'/'${READONLY_USER_ESC}'/g" \
        -e "s/\"user\"/\"${READONLY_USER_ESC}\"/g" \
        -e "s/user123/${READONLY_PASSWORD_ESC}/g" \
        -e "s/myshop_db/${DATABASE_NAME_ESC}/g" \
        "$SQL_FILE" > "$TEMP_SQL_FILE"
    
    # Выполнение SQL скрипта
    echo -e "\n📝 Выполнение SQL скрипта..."
    
    if psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d "$DATABASE_NAME" -f "$TEMP_SQL_FILE"; then
        echo -e "${GREEN}✅ Read-only пользователь успешно создан${NC}"
        rm -f "$TEMP_SQL_FILE"
    else
        echo -e "${RED}❌ Ошибка при создании пользователя${NC}"
        rm -f "$TEMP_SQL_FILE"
        exit 1
    fi

    # Проверка прав пользователя
    echo -e "\n🔍 Проверка прав пользователя..."
    psql -h "$PG_HOST" -p "$PG_PORT" -U "$PG_USER" -d "$DATABASE_NAME" << EOF
SELECT 
    'Пользователь: ' || usename AS user_info,
    'Права на базу: ' || string_agg(privilege_type, ', ') AS db_privileges
FROM information_schema.role_table_grants
WHERE grantee = '${READONLY_USER}' AND table_schema = 'public'
GROUP BY usename;
EOF

    echo -e "\n${GREEN}🎉 Read-only пользователь готов к использованию!${NC}"
    echo -e "   👤 Имя: ${READONLY_USER}"
    echo -e "   📦 База данных: ${DATABASE_NAME}"
    echo -e "   🔐 Права: SELECT (только чтение)"
    echo -e "\n${YELLOW}Теперь можно настроить инстансы приложения на портах 8081 и 8082${NC}"
    echo -e "   Используйте эти учетные данные в .env файлах для read-only инстансов:"
    echo -e "   ${BLUE}DATABASE_USERNAME=${READONLY_USER}${NC}"
    echo -e "   ${BLUE}DATABASE_PASSWORD=${READONLY_USER_PASSWORD}${NC}"
}

# ============================================================================
# 6. ЗАПУСК
# ============================================================================

create_readonly_user
