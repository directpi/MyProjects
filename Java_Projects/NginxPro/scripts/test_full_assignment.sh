#!/usr/bin/env bash
set -e

BASE_HTTP="http://localhost"
BASE_HTTPS="https://ecommerce.local"
API_V1_HTTP="$BASE_HTTP/api/v1"
API_V1_HTTPS="$BASE_HTTPS/api/v1"

LOG_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)/logs"
mkdir -p "$LOG_DIR"
LOG_FILE="$LOG_DIR/test_full_assignment_$(date +%Y%m%d_%H%M%S).log"

echo "Лог тестирования будет сохранён в: $LOG_FILE"
echo

{
echo "============================================================"
echo " БЛОК 0. Проверка backend-инстансов напрямую (8080/8081/8082)"
echo "============================================================"
for port in 8080 8081 8082; do
  echo "--- http://localhost:${port}/api/v1/health ---"
  curl -sS -i "http://localhost:${port}/api/v1/health" | head -n 10 || echo "!! FAILED ${port}"
  echo
done

echo
echo "============================================================"
echo " БЛОК 1. Маршрутизация и базовые endpoint'ы через Nginx (HTTP)"
echo "============================================================"

for url in / /image.png /api /api/v1 /api/v1/health /api/v2/health /admin /status; do
  echo "===== $BASE_HTTP$url ====="
  curl -k -sS -i "$BASE_HTTP$url" | head -n 12 || echo "!! FAILED $url"
  echo
done

echo
echo "============================================================"
echo " БЛОК 2. Маршрутизация и базовые endpoint'ы через Nginx (HTTPS)"
echo "============================================================"

for url in / /image.png /api /api/v1 /api/v1/health /api/v2/health /admin /status; do
  echo "===== $BASE_HTTPS$url ====="
  curl -k -sS -i "$BASE_HTTPS$url" | head -n 12 || echo "!! FAILED $url"
  echo
done

echo
echo "============================================================"
echo " БЛОК 3. Swagger UI по /api/v1"
echo "============================================================"

echo "--- Ожидаем 302 на /swagger/swagger-ui/index.html ---"
curl -k -sS -i "$API_V1_HTTPS" | head -n 12
echo

echo
echo "============================================================"
echo " БЛОК 4. Балансировка GET-запросов (2:1:1) по X-Upstream"
echo "============================================================"

# Эндпоинт для теста GET (подставь, если у тебя другой, важно чтобы был GET список)
GET_ENDPOINT="$API_V1_HTTPS/clients?limit=1"

echo "Тестируем 15 запросов к $GET_ENDPOINT"
declare -A upstreams
for i in $(seq 1 15); do
  up=$(curl -k -sS -o /dev/null -D - "$GET_ENDPOINT" | awk '/^X-Upstream:/ {print $2}')
  upstreams["$up"]=$(( upstreams["$up"] + 1 ))
  echo "[$i] X-Upstream: $up"
done

echo
echo "Сводка по X-Upstream:"
for k in "${!upstreams[@]}"; do
  clean_key=$(echo "$k" | tr -d '\r')
  echo "  $clean_key : ${upstreams[$k]} раз"
done
echo "Ожидание: адрес для 8080 примерно в 2 раза чаще, чем 8081/8082."
echo

echo
echo "============================================================"
echo " БЛОК 5. Write-запросы: всегда на основной backend (8080)"
echo "============================================================"

# ВНИМАНИЕ: ниже пример. Если другой POST-эндпоинт и тело, замените.
WRITE_ENDPOINT="$API_V1_HTTPS/clients"
WRITE_BODY='{"name":"TestReadonly","email":"test@example.com"}'

echo "--- POST $WRITE_ENDPOINT ---"
curl -k -sS -i "$WRITE_ENDPOINT" \
  -H "Content-Type: application/json" \
  -d "$WRITE_BODY" \
  | sed -n '1,20p'

echo
echo "Проверь в заголовках выше строку X-Upstream: должен быть только порт 8080."
echo "Read-only инстансы (8081/8082) для write-запросов использоваться не должны."
echo

echo
echo "============================================================"
echo " БЛОК 6. Кеширование: X-Cache-Status (MISS -> HIT)"
echo "============================================================"

CACHE_TEST_ENDPOINT="$API_V1_HTTPS/health"

for i in 1 2 3; do
  echo "--- запрос #$i к $CACHE_TEST_ENDPOINT ---"
  curl -k -sS -o /dev/null -D - "$CACHE_TEST_ENDPOINT" \
    | awk '/^X-Cache-Status:/ {print $0}'
  sleep 1
done

echo
echo "Ожидание: первый запрос обычно MISS, последующие чаще HIT (если TTL не истёк)."
echo

echo
echo "============================================================"
echo " БЛОК 7. Gzip: HTML/JSON сжат, PNG — нет"
echo "============================================================"

echo "--- HTML (/) ---"
curl -k -sS -o /dev/null -D - "$BASE_HTTPS/" \
  -H "Accept-Encoding: gzip" \
  | awk '/^Content-Type:/ {print}; /^Content-Encoding:/ {print}'
echo

echo "--- JSON (/api/v1/health) ---"
curl -k -sS -o /dev/null -D - "$API_V1_HTTPS/health" \
  -H "Accept-Encoding: gzip" \
  | awk '/^Content-Type:/ {print}; /^Content-Encoding:/ {print}'
echo

echo "--- PNG (/image.png) ---"
curl -k -sS -o /dev/null -D - "$BASE_HTTPS/image.png" \
  -H "Accept-Encoding: gzip" \
  | awk '/^Content-Type:/ {print}; /^Content-Encoding:/ {print}'
echo

echo "Ожидание:"
echo " - для HTML/JSON есть Content-Encoding: gzip"
echo " - для PNG НЕТ Content-Encoding: gzip (только Content-Type: image/png)"
echo

echo "============================================================"
echo " Тестовый прогон завершён."
echo "Смотри на X-Upstream, X-Cache-Status, Content-Encoding и статусы HTTP."
echo "============================================================"
} > "$LOG_FILE" 2>&1