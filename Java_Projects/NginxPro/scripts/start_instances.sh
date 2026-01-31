#!/bin/bash

# Скрипт запуска нескольких инстансов Spring Boot приложения для балансировки нагрузки
# Основной инстанс (8080) - полный доступ к БД
# Read-only инстансы (8081, 8082) - только чтение из БД

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

SCRIPT_DIR="$(dirname "$0")"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
LOG_DIR="${PROJECT_DIR}/logs"
ENV_FILE="${PROJECT_DIR}/.env"

# Загрузка переменных из .env файла
if [ -f "$ENV_FILE" ]; then
    set -a
    source "$ENV_FILE"
    set +a
fi

# Проверка обязательных переменных
if [ -z "$DATABASE_USERNAME" ]; then
    echo -e "${RED}❌ Ошибка: переменная DATABASE_USERNAME не задана в .env файле${NC}"
    exit 1
fi

if [ -z "$DATABASE_PASSWORD" ]; then
    echo -e "${RED}❌ Ошибка: переменная DATABASE_PASSWORD не задана в .env файле${NC}"
    exit 1
fi

if [ -z "$READONLY_USER" ]; then
    echo -e "${RED}❌ Ошибка: переменная READONLY_USER не задана в .env файле${NC}"
    exit 1
fi

if [ -z "$READONLY_USER_PASSWORD" ]; then
    echo -e "${RED}❌ Ошибка: переменная READONLY_USER_PASSWORD не задана в .env файле${NC}"
    exit 1
fi

# Создаем директорию для логов если её нет
mkdir -p "$LOG_DIR"

echo -e "${BLUE}🚀 Запуск инстансов Spring Boot приложения${NC}\n"

# Проверка наличия Java
if ! command -v java &> /dev/null; then
    echo -e "${RED}❌ Java не установлена${NC}"
    exit 1
fi

# Проверка наличия Gradle wrapper
if [ ! -f "${PROJECT_DIR}/gradlew" ]; then
    echo -e "${RED}❌ Gradle wrapper не найден${NC}"
    exit 1
fi

# Функция проверки занятости порта
is_port_in_use() {
    local port=$1
    if command -v lsof &> /dev/null; then
        lsof -ti:${port} > /dev/null 2>&1
    elif command -v netstat &> /dev/null; then
        netstat -tlnp 2>/dev/null | grep -q ":${port} "
    elif command -v ss &> /dev/null; then
        ss -tlnp 2>/dev/null | grep -q ":${port} "
    else
        # Если нет доступных утилит, пробуем подключиться
        timeout 1 bash -c "echo > /dev/tcp/localhost/${port}" 2>/dev/null
    fi
}

# Функция запуска инстанса
start_instance() {
    local port=$1
    local db_user=$2
    local db_password=$3
    local log_file="${LOG_DIR}/app_${port}.log"
    
    # Проверка занятости порта
    if is_port_in_use "$port"; then
        local existing_pid=$(lsof -ti:${port} 2>/dev/null || echo "")
        echo -e "${YELLOW}⚠️  Порт ${port} уже занят${NC}"
        if [ -n "$existing_pid" ]; then
            echo -e "   Процесс уже запущен (PID: ${existing_pid})"
        fi
        echo -e "   ${BLUE}Пропускаю запуск инстанса на порту ${port}${NC}"
        return 0
    fi
    
    echo -e "${YELLOW}📦 Запуск инстанса на порту ${port}...${NC}"
    
    cd "$PROJECT_DIR"
    
    # Доп. переменные приложения (чтобы имя/версия были одинаковыми на всех инстансах)
    # ВАЖНО: НЕ прокидываем APP_NAME, т.к. ./gradlew переопределяет APP_NAME=Gradle.
    # Вместо этого прокидываем SPRING_APPLICATION_NAME, которое читает Spring Boot.
    # Передаем только если они реально заданы (иначе можем затереть значения из .env файла).
    local extra_env=()
    if [ -n "${APP_NAME:-}" ]; then
        extra_env+=(SPRING_APPLICATION_NAME="${APP_NAME}")
    fi
    if [ -n "${APP_VERSION:-}" ]; then
        extra_env+=(APP_VERSION="${APP_VERSION}")
    fi
    
    # Запуск в фоновом режиме
    # ВАЖНО: пропускаем Gradle-задачу checkPort (-x checkPort),
    # т.к. она всегда проверяет только порт 8080 и ломает запуск read-only инстансов,
    # когда основной сервис уже работает на 8080.
    nohup env SERVER_PORT="${port}" \
            DATABASE_USERNAME="${db_user}" \
            DATABASE_PASSWORD="${db_password}" \
            "${extra_env[@]}" \
            ./gradlew bootRun -x checkPort > "${log_file}" 2>&1 &
    
    local pid=$!
    echo -e "${GREEN}✅ Инстанс запущен (PID: ${pid}, порт: ${port})${NC}"
    echo -e "   Логи: ${log_file}"
    
    # Небольшая задержка перед запуском следующего инстанса
    sleep 3
}

# Запуск основного инстанса (8080) с полным доступом к БД
echo -e "${BLUE}1. Основной инстанс (порт 8080, полный доступ к БД)${NC}"
start_instance 8080 "${DATABASE_USERNAME}" "${DATABASE_PASSWORD}"

# Запуск read-only инстансов (8081, 8082)
echo -e "\n${BLUE}2. Read-only инстансы (порты 8081, 8082)${NC}"
start_instance 8081 "${READONLY_USER}" "${READONLY_USER_PASSWORD}"
start_instance 8082 "${READONLY_USER}" "${READONLY_USER_PASSWORD}"

echo -e "\n${GREEN}🎉 Все инстансы запущены!${NC}"
echo -e "\n${YELLOW}Проверка работы:${NC}"
echo -e "   Основной инстанс: ${BLUE}curl http://localhost:8080/api/v1/health${NC}"
echo -e "   Read-only инстанс 1: ${BLUE}curl http://localhost:8081/api/v1/health${NC}"
echo -e "   Read-only инстанс 2: ${BLUE}curl http://localhost:8082/api/v1/health${NC}"
echo -e "\n${YELLOW}Просмотр логов:${NC}"
echo -e "   tail -f ${LOG_DIR}/app_8080.log"
echo -e "   tail -f ${LOG_DIR}/app_8081.log"
echo -e "   tail -f ${LOG_DIR}/app_8082.log"
echo -e "\n${YELLOW}Остановка инстансов:${NC}"
echo -e "   ${BLUE}./scripts/stop_instances.sh${NC}"
