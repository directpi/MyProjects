# Инструкции по установке и проверке служб

## Nginx

### Проверка установки Nginx

```bash
# Проверка наличия Nginx
which nginx

# Проверка версии
nginx -v

# Проверка статуса процесса
ps aux | grep nginx | grep -v grep
```

### Установка Nginx (если не установлен)

#### Ubuntu/Debian:
```bash
sudo apt update
sudo apt install -y nginx
```

#### CentOS/RHEL:
```bash
sudo yum install -y nginx
# или для новых версий
sudo dnf install -y nginx
```

### Управление службой Nginx

#### В системах с systemd:
```bash
# Проверка статуса
sudo systemctl status nginx

# Запуск
sudo systemctl start nginx

# Остановка
sudo systemctl stop nginx

# Перезапуск
sudo systemctl restart nginx

# Перезагрузка конфигурации (без остановки)
sudo systemctl reload nginx

# Автозапуск при загрузке системы
sudo systemctl enable nginx
```

#### В WSL или системах без systemd:
```bash
# Запуск
sudo nginx

# Остановка
sudo nginx -s stop

# Перезагрузка конфигурации
sudo nginx -s reload

# Проверка конфигурации
sudo nginx -t
```

### Проверка конфигурации Nginx

```bash
# Проверка синтаксиса конфигурации
sudo nginx -t

# Проверка с выводом путей к конфигурационным файлам
sudo nginx -T
```

### Основные пути конфигурации Nginx

- **Основной конфиг**: `/etc/nginx/nginx.conf`
- **Конфиги сайтов**: `/etc/nginx/sites-available/` и `/etc/nginx/sites-enabled/`
- **Дополнительные конфиги**: `/etc/nginx/conf.d/`
- **Логи**: 
  - Access log: `/var/log/nginx/access.log`
  - Error log: `/var/log/nginx/error.log`

### Проверка работы Nginx

```bash
# Проверка, что Nginx слушает порты
sudo netstat -tlnp | grep nginx
# или
sudo ss -tlnp | grep nginx

# Проверка HTTP ответа
curl -I http://localhost
```

---

## PostgreSQL

### Проверка установки PostgreSQL

```bash
# Проверка наличия PostgreSQL
which psql

# Проверка версии
psql --version

# Проверка статуса службы
sudo systemctl status postgresql
```

### Установка PostgreSQL

#### Ubuntu/Debian:
```bash
sudo apt update
sudo apt install -y postgresql postgresql-contrib
```

#### CentOS/RHEL:
```bash
sudo yum install -y postgresql-server postgresql-contrib
sudo postgresql-setup --initdb
```

### Управление службой PostgreSQL

```bash
# Запуск
sudo systemctl start postgresql

# Остановка
sudo systemctl stop postgresql

# Перезапуск
sudo systemctl restart postgresql

# Автозапуск
sudo systemctl enable postgresql
```

### Подключение к PostgreSQL

```bash
# Подключение от имени пользователя postgres
sudo -u postgres psql

# Подключение к конкретной базе данных
psql -h localhost -U postgres -d myshop_db

# Подключение с указанием порта
psql -h localhost -p 5432 -U postgres -d myshop_db
```

### Проверка существующих пользователей БД

Перед созданием read-only пользователя рекомендуется проверить, какие пользователи уже существуют:

```bash
# Автоматическая проверка через скрипт
./scripts/check_users.sh

# Или вручную через psql
psql -U postgres -d postgres -c "\du"
```

### Создание read-only пользователя

**Учетные данные read-only пользователя:**
- Имя: `user`
- Пароль: `user123`

*!!! Это демо‑учётка для локальной разработки. Для реальных окружений: генерировать случайный пароль и хранить его в .env/секрет‑хранилище.*

**Автоматическое создание (рекомендуется):**

```bash
# Убедитесь, что PostgreSQL запущен
sudo systemctl start postgresql

# Запустите скрипт создания read-only пользователя
./scripts/create_readonly_user.sh
```

**Ручное создание через SQL:**

```sql
-- Подключиться к PostgreSQL как суперпользователь
sudo -u postgres psql

-- Проверить существование пользователя
SELECT * FROM pg_user WHERE usename = 'user';

-- Если пользователь не существует, создать его
CREATE USER "user" WITH PASSWORD 'user123';

-- Предоставить права на подключение к базе данных
GRANT CONNECT ON DATABASE myshop_db TO "user";

-- Предоставить права на использование схемы
\c myshop_db
GRANT USAGE ON SCHEMA public TO "user";

-- Предоставить права на чтение всех таблиц
GRANT SELECT ON ALL TABLES IN SCHEMA public TO "user";

-- Установить права по умолчанию для будущих таблиц
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT SELECT ON TABLES TO "user";

-- Предоставить права на использование последовательностей
GRANT USAGE ON ALL SEQUENCES IN SCHEMA public TO "user";
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT USAGE ON SEQUENCES TO "user";

-- Выйти
\q
```

### Проверка подключения к БД

```bash
# Проверка подключения с основным пользователем
psql -h localhost -U java_user -d myshop_db -c "SELECT version();"

# Проверка подключения с read-only пользователем
psql -h localhost -U user -d myshop_db -c "SELECT version();"

# Попытка записи (должна завершиться ошибкой для read-only пользователя)
psql -h localhost -U user -d myshop_db -c "INSERT INTO test VALUES (1);"
```

---

## pgAdmin

### Проверка установки pgAdmin

```bash
# Проверка наличия pgAdmin
which pgadmin4
# или для Docker
docker ps | grep pgadmin
```

### Установка pgAdmin

#### Через Docker (рекомендуется):
```bash
docker run -p 5050:80 \
  -e PGADMIN_DEFAULT_EMAIL=admin@example.com \
  -e PGADMIN_DEFAULT_PASSWORD=admin \
  -d dpage/pgadmin4
```

#### Через apt (Ubuntu/Debian):
```bash
sudo apt update
sudo apt install -y pgadmin4
```

### Проверка работы pgAdmin

```bash
# Проверка, что pgAdmin слушает порт 5050
curl -I http://localhost:5050

# Или открыть в браузере
# http://localhost:5050
```

---

## Spring Boot приложение

### Проверка Java

```bash
# Проверка версии Java
java -version

# Проверка версии Java компилятора
javac -version
```

### Запуск приложения

```bash
# Перейти в директорию проекта
cd "/home/directpi/21school/BE3_Nginx_Pro.ID_1269323-1/src/NginxPro"

# Запуск основного инстанса (порт 8080)
./gradlew bootRun

# Запуск с указанием порта через переменную окружения
SERVER_PORT=8081 ./gradlew bootRun

# Или через .env файл (создать копии для каждого инстанса)
```

### Запуск нескольких инстансов

#### Вариант 1: Автоматический запуск через скрипт (рекомендуется)

```bash
# Перейти в директорию проекта
cd "/home/directpi/21school/BE3_Nginx_Pro.ID_1269323-1/src/NginxPro"

# Запустить все инстансы (8080, 8081, 8082)
./scripts/start_instances.sh

# Остановить все инстансы
./scripts/stop_instances.sh
```

**Примечание:** Перед запуском убедитесь, что:
1. PostgreSQL запущен
2. База данных `myshop_db` создана
3. Read-only пользователь `user` создан (выполните `./scripts/create_readonly_user.sh`)

#### Вариант 2: Через разные терминалы
```bash
# Терминал 1 - основной инстанс
cd "/home/directpi/21school/BE3_Nginx_Pro.ID_1269323-1/src/NginxPro"
SERVER_PORT=8080 ./gradlew bootRun

# Терминал 2 - read-only инстанс 1
cd "/home/directpi/21school/BE3_Nginx_Pro.ID_1269323-1/src/NginxPro"
SERVER_PORT=8081 DATABASE_USERNAME=user DATABASE_PASSWORD=user123 ./gradlew bootRun

# Терминал 3 - read-only инстанс 2
cd "/home/directpi/21school/BE3_Nginx_Pro.ID_1269323-1/src/NginxPro"
SERVER_PORT=8082 DATABASE_USERNAME=user DATABASE_PASSWORD=user123 ./gradlew bootRun
```

#### Вариант 2: В фоновом режиме
```bash
# Основной инстанс
cd "/home/directpi/21school/BE3_Nginx_Pro.ID_1269323-1/src/NginxPro"
nohup ./gradlew bootRun > logs/app_8080.log 2>&1 &

# Read-only инстанс 1
cd "/home/directpi/21school/BE3_Nginx_Pro.ID_1269323-1/src/NginxPro"
nohup env SERVER_PORT=8081 DATABASE_USERNAME=user DATABASE_PASSWORD=user123 ./gradlew bootRun > logs/app_8081.log 2>&1 &

# Read-only инстанс 2
cd "/home/directpi/21school/BE3_Nginx_Pro.ID_1269323-1/src/NginxPro"
nohup env SERVER_PORT=8082 DATABASE_USERNAME=user DATABASE_PASSWORD=user123 ./gradlew bootRun > logs/app_8082.log 2>&1 &
```

### Проверка работы приложения

```bash
# Проверка основного инстанса
curl http://localhost:8080/api/v1/health

# Проверка read-only инстанса 1
curl http://localhost:8081/api/v1/health

# Проверка read-only инстанса 2
curl http://localhost:8082/api/v1/health

# Проверка Swagger
curl http://localhost:8080/swagger/index.html
```

---

## OpenSSL (для создания SSL сертификатов)

### Проверка установки OpenSSL

```bash
openssl version
```

### Установка OpenSSL

#### Ubuntu/Debian:
```bash
sudo apt install -y openssl
```

#### CentOS/RHEL:
```bash
sudo yum install -y openssl
```

### Создание самоподписанного SSL сертификата

```bash
# Создать директорию для сертификатов
sudo mkdir -p /etc/nginx/ssl

# Создать приватный ключ
sudo openssl genrsa -out /etc/nginx/ssl/ecommerce.local.key 2048

# Создать самоподписанный сертификат
sudo openssl req -new -x509 -key /etc/nginx/ssl/ecommerce.local.key \
  -out /etc/nginx/ssl/ecommerce.local.crt \
  -days 365 \
  -subj "/C=RU/ST=State/L=City/O=Organization/CN=ecommerce.local"

# Установить правильные права доступа
sudo chmod 600 /etc/nginx/ssl/ecommerce.local.key
sudo chmod 644 /etc/nginx/ssl/ecommerce.local.crt
```

---

## Локальный DNS (/etc/hosts)

### Добавление записи в /etc/hosts

```bash
# Быстрый способ через echo (WSL)
echo "127.0.0.1 ecommerce.local" | sudo tee -a /etc/hosts

# Или вручную открыть файл для редактирования
sudo nano /etc/hosts   # или sudo vi /etc/hosts
# и добавить строку:
# 127.0.0.1 ecommerce.local
```

### Проверка резолвинга домена

```bash
# Проверка резолвинга в WSL
getent hosts ecommerce.local

# Дополнительно (опционально)
ping -c 1 ecommerce.local
host ecommerce.local
nslookup ecommerce.local
```

> **Важно (Windows + WSL):**  
> Чтобы браузер в Windows тоже видел `ecommerce.local`, нужно добавить строку  
> `127.0.0.1 ecommerce.local` в файл  
> `C:\Windows\System32\drivers\etc\hosts` от имени администратора.

---

## Итоговый чек‑лист для HTTPS на ecommerce.local

1. **Убедиться, что сертификаты существуют**
   - Директория и файлы:

   ```bash
   sudo mkdir -p /etc/nginx/ssl

   # приватный ключ
   sudo openssl genrsa -out /etc/nginx/ssl/ecommerce.local.key 2048

   # самоподписанный сертификат (365 дней, CN = ecommerce.local)
   sudo openssl req -new -x509 \
     -key /etc/nginx/ssl/ecommerce.local.key \
     -out /etc/nginx/ssl/ecommerce.local.crt \
     -days 365 \
     -subj "/CN=ecommerce.local"

   # права доступа
   sudo chmod 600 /etc/nginx/ssl/ecommerce.local.key
   sudo chmod 644 /etc/nginx/ssl/ecommerce.local.crt
   ```

2. **Прописать домен `ecommerce.local`**
   - В WSL: добавить запись в `/etc/hosts` (см. выше).
   - В Windows: добавить запись `127.0.0.1 ecommerce.local` в  
     `C:\Windows\System32\drivers\etc\hosts`.

3. **Убедиться, что Nginx использует конфиги проекта**

   ```bash
   # в каталоге проекта
   cd "/home/directpi/21school/BE3_Nginx_Pro.ID_1269323-1/src/NginxPro"

   # скопировать конфиг виртуального хоста
   sudo cp nginx/conf.d/ecommerce.conf /etc/nginx/conf.d/ecommerce.conf
   ```

4. **Проверка и перезапуск Nginx**

   ```bash
   sudo nginx -t
   sudo systemctl reload nginx   # или sudo nginx -s reload
   ```

5. **Проверка в браузере**
   - Открыть:
     - `https://ecommerce.local/` — главная страница (`index.html`).
     - `https://ecommerce.local/api/v1` — Swagger UI (через редирект на `/swagger/swagger-ui/index.html`).
     - `https://ecommerce.local/admin` — pgAdmin (через Nginx).

---

## Общая проверка всех служб

### Скрипт проверки базовых служб

```bash
#!/bin/bash
echo "=== Проверка служб ==="

echo -n "Nginx: "
if command -v nginx &> /dev/null; then
    echo "✓ Установлен ($(nginx -v 2>&1))"
else
    echo "✗ Не установлен"
fi

echo -n "PostgreSQL: "
if command -v psql &> /dev/null; then
    echo "✓ Установлен ($(psql --version))"
else
    echo "✗ Не установлен"
fi

echo -n "Java: "
if command -v java &> /dev/null; then
    echo "✓ Установлен ($(java -version 2>&1 | head -1))"
else
    echo "✗ Не установлен"
fi

echo -n "OpenSSL: "
if command -v openssl &> /dev/null; then
    echo "✓ Установлен ($(openssl version))"
else
    echo "✗ Не установлен"
fi

echo -n "Nginx процесс: "
if pgrep nginx > /dev/null; then
    echo "✓ Запущен"
else
    echo "✗ Не запущен"
fi

echo -n "PostgreSQL процесс: "
if pgrep postgres > /dev/null; then
    echo "✓ Запущен"
else
    echo "✗ Не запущен"
fi

echo -n "Приложение на 8080: "
if curl -s http://localhost:8080/api/v1/health > /dev/null; then
    echo "✓ Работает"
else
    echo "✗ Не отвечает"
fi

echo -n "pgAdmin на 5050: "
if curl -s http://localhost:5050 > /dev/null; then
    echo "✓ Работает"
else
    echo "✗ Не отвечает"
fi
```
---
Сохранить как `check-services.sh` и сделать исполняемым:
```bash
chmod +x check-services.sh
./check-services.sh
```

---

### Полная проверка задания (скрипт `scripts/test_full_assignment.sh`)

Для автоматизированной проверки всего задания (маршрутизация, балансировка, кеширование, gzip, HTTPS, `/admin`, Swagger и т.д.) в проекте есть скрипт:

- `scripts/test_full_assignment.sh`

**Как использовать:**

1. Убедиться, что:
   - PostgreSQL запущен, БД `myshop_db` создана;
   - read-only пользователь `user/user123` создан (`./scripts/create_readonly_user.sh`);
   - все инстансы backend’а запущены (`./scripts/start_instances.sh`);
   - Nginx запущен и использует конфиги из проекта (`nginx/nginx.conf`, `nginx/conf.d/ecommerce.conf`);
   - домен `ecommerce.local` прописан в `/etc/hosts` (WSL) и в `C:\Windows\System32\drivers\etc\hosts` (Windows);
   - SSL‑сертификаты `ecommerce.local.crt/key` созданы в `/etc/nginx/ssl`.

2. Запустить скрипт из корня проекта:

```bash
cd "/home/directpi/21school/BE3_Nginx_Pro.ID_1269323-1/src/NginxPro"
./scripts/test_full_assignment.sh
```

3. Скрипт **не пишет в терминал подробный вывод**, а сохраняет его в лог‑файл:

- Директория логов: `logs/`
- Имя файла: `logs/test_full_assignment_YYYYMMDD_HHMMSS.log`

Например:

```text
logs/test_full_assignment_20260127_190941.log
```

4. Открыть лог и проанализировать результаты:

- Блоки в логе:
  - Блок 0 — health всех backend‑инстансов (8080/8081/8082)
  - Блок 1/2 — маршрутизация HTTP/HTTPS (`/`, `/api`, `/api/v1`, `/admin`, `/status` и т.д.)
  - Блок 3 — редирект `/api/v1` → Swagger UI
  - Блок 4 — балансировка по `X-Upstream` (2:1:1)
  - Блок 5 — write‑запросы и `X-Upstream: 127.0.0.1:8080`
  - Блок 6 — поведение кеша по `X-Cache-Status`
  - Блок 7 — gzip (`Content-Encoding: gzip` для HTML/JSON, отсутствие gzip для PNG)

Это основной инструмент для быстрой перепроверки, что всё задание по Nginx (маршрутизация, балансировка, кеш, gzip, HTTPS, `/admin`, Swagger) работает корректно после любых изменений.



## Устранение проблем

### Nginx не запускается

1. Проверить конфигурацию:
```bash
sudo nginx -t
```

2. Проверить логи:
```bash
sudo tail -f /var/log/nginx/error.log
```

3. Проверить, не занят ли порт:
```bash
sudo netstat -tlnp | grep :80
sudo netstat -tlnp | grep :443
```

### PostgreSQL не подключается

1. Проверить статус службы:
```bash
sudo systemctl status postgresql
```

2. Проверить логи:
```bash
sudo tail -f /var/log/postgresql/postgresql-*.log
```

3. Проверить настройки подключения в `pg_hba.conf`:
```bash
sudo cat /etc/postgresql/*/main/pg_hba.conf
```

### Приложение не запускается

1. Проверить логи:
```bash
tail -f logs/application.log
```

2. Проверить, не занят ли порт:
```bash
netstat -tlnp | grep :8080
```

3. Проверить переменные окружения:
```bash
cat .env
```

---

## Полезные команды

### Просмотр логов в реальном времени

```bash
# Логи Nginx
sudo tail -f /var/log/nginx/access.log
sudo tail -f /var/log/nginx/error.log

# Логи приложения
tail -f logs/application.log

# Логи PostgreSQL
sudo tail -f /var/log/postgresql/postgresql-*.log
```

### Очистка логов

```bash
# Очистить логи Nginx (создать пустой файл)
sudo truncate -s 0 /var/log/nginx/access.log
sudo truncate -s 0 /var/log/nginx/error.log

# Очистить логи приложения
> logs/application.log
```

### Проверка портов

```bash
# Все открытые порты
sudo netstat -tlnp

# Конкретный порт
sudo netstat -tlnp | grep :8080

# Или через ss
sudo ss -tlnp | grep :8080
```
