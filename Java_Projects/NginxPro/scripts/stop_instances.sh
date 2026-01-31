#!/bin/bash

# Скрипт остановки всех инстансов Spring Boot приложения
# Останавливает инстансы на портах 8080, 8081 и 8082

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}🛑 Остановка инстансов Spring Boot приложения${NC}\n"

# Порты для остановки (все инстансы)
PORTS=(8080 8081 8082)

for port in "${PORTS[@]}"; do
    # Находим PID процесса на указанном порту
    pid=$(lsof -ti:${port} 2>/dev/null)
    
    if [ -n "$pid" ]; then
        echo -e "${YELLOW}Остановка процесса на порту ${port} (PID: ${pid})...${NC}"
        kill "$pid" 2>/dev/null
        
        # Ждем завершения процесса
        sleep 2
        
        # Проверяем, завершился ли процесс
        if kill -0 "$pid" 2>/dev/null; then
            echo -e "${RED}Процесс не завершился, принудительная остановка...${NC}"
            kill -9 "$pid" 2>/dev/null
        fi
        
        echo -e "${GREEN}✅ Инстанс на порту ${port} остановлен${NC}"
    else
        echo -e "${YELLOW}⚠️  Инстанс на порту ${port} не найден${NC}"
    fi
done

echo -e "\n${GREEN}🎉 Все инстансы остановлены${NC}"
