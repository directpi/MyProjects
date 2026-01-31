#!/bin/bash
# Скрипт установки конфигурации Nginx для E-Commerce проекта
# Режимы:
# 1) Перезаписать /etc/nginx/nginx.conf (с бэкапом) и скопировать ecommerce.conf в /etc/nginx/conf.d/
# 2) Ручной режим: ничего не менять, только вывести краткую инструкцию

set -e

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Установка конфигурации Nginx для E-Commerce проекта ===${NC}"

# Определяем путь к директории скрипта (nginx/)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Определяем путь к корню проекта (src/NginxPro)
# Скрипт находится в nginx/, поэтому корень проекта на уровень выше
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo -e "${YELLOW}Обнаружен путь к проекту: ${PROJECT_ROOT}${NC}"

# Проверяем, что мы в правильной директории
if [ ! -f "$SCRIPT_DIR/conf.d/ecommerce.conf" ]; then
    echo -e "${RED}Ошибка: Не найден файл conf.d/ecommerce.conf${NC}"
    echo "Убедитесь, что скрипт запущен из директории nginx/"
    exit 1
fi

print_manual_instructions() {
    echo ""
    echo -e "${YELLOW}Ручной режим (скрипт ничего не меняет в системе)${NC}"
    echo ""
    echo "1) Скопировать конфиг виртуального хоста:"
    echo "   sudo cp \"$SCRIPT_DIR/conf.d/ecommerce.conf\" /etc/nginx/conf.d/ecommerce.conf"
    echo ""
    echo "2) В /etc/nginx/nginx.conf в блок http {} добавить:"
    echo "   - map \$host \$project_root { default $PROJECT_ROOT; }"
    echo "   - upstream backend_primary (8080 weight=2, 8081/8082 weight=1)"
    echo "   - upstream backend_write (8080)"
    echo "   - include /etc/nginx/conf.d/*.conf;"
    echo ""
    echo "3) Проверить и применить:"
    echo "   sudo nginx -t && sudo systemctl reload nginx"
    echo ""
    echo "4) Если / отдаёт 403/404 и в error.log Permission denied:"
    echo "   Nginx (www-data) нужен проход по /home/<user>/... (лучше через setfacl)."
    echo ""
}

write_nginx_conf_overwrite() {
    local target="/etc/nginx/nginx.conf"
    local backup="/etc/nginx/nginx.conf.backup.$(date +%Y%m%d_%H%M%S)"
    local template="$SCRIPT_DIR/nginx.conf"

    if [ ! -f "$template" ]; then
        echo -e "${RED}Ошибка: не найден шаблон nginx.conf в ${template}${NC}"
        exit 1
    fi

    echo -e "${YELLOW}Перезапись ${target} содержимым из шаблона (с бэкапом: ${backup})...${NC}"
    sudo cp "$target" "$backup"

    # Используем шаблон nginx.conf из проекта, меняя только default-путь в map $project_root.
    # Это позволяет заранее проверять и ревьюить конфигурацию прямо в репозитории.
    sed "s|default .*/src/NginxPro;|default $PROJECT_ROOT;|g" "$template" | sudo tee "$target" >/dev/null
}

echo ""
echo -e "${YELLOW}Выберите режим установки:${NC}"
echo "  1) Перезаписать /etc/nginx/nginx.conf (с бэкапом) + скопировать ecommerce.conf"
echo "  2) Ручной режим (ничего не менять, вывести инструкцию)"
echo ""
read -r -p "Ваш выбор (1/2) [2]: " MODE
MODE="${MODE:-2}"

if [ "$MODE" = "2" ]; then
    print_manual_instructions
    exit 0
fi

if [ "$MODE" != "1" ]; then
    echo -e "${RED}Ошибка: неверный выбор. Допустимо 1 или 2.${NC}"
    exit 1
fi

write_nginx_conf_overwrite

echo -e "${YELLOW}Копирование ecommerce.conf в /etc/nginx/conf.d/...${NC}"
sudo mkdir -p /etc/nginx/conf.d
sudo cp "$SCRIPT_DIR/conf.d/ecommerce.conf" /etc/nginx/conf.d/ecommerce.conf

echo -e "${YELLOW}Проверка и применение конфигурации...${NC}"
sudo nginx -t
sudo systemctl reload nginx

echo -e "${GREEN}=== Установка завершена ===${NC}"
echo ""
echo "Проверка:"
echo "  curl -i http://localhost/"
echo "  curl -i http://localhost/api"
echo "  curl -i http://localhost/api/v1/health"
echo "  curl -i http://localhost/status"
