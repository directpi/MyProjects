# Ручная установка конфигурации Nginx

## Варианты установки

У вас есть два способа настроить Nginx:

- **Вариант A (автоматический)** – запустить скрипт `configure_nginx_from_project.sh` и выбрать режим `1`.  
  Скрипт:
  - сделает бэкап `/etc/nginx/nginx.conf`;
  - перезапишет его на основе `src/NginxPro/nginx/nginx.conf`, подставив актуальный `PROJECT_ROOT`;
  - скопирует `nginx/conf.d/ecommerce.conf` в `/etc/nginx/conf.d/ecommerce.conf`;
  - выполнит `sudo nginx -t` и `sudo systemctl reload nginx`.

- **Вариант B (ручной)** – ничего не менять скриптом и внести изменения вручную по инструкции ниже.

Дальше описан **ручной вариант (B)**.

## Что нужно сконфигурировать вручную

### 1. В `/etc/nginx/nginx.conf` (блок `http {}`)

Внутри блока `http {}` должны быть:

1. **Переменная `$project_root`**:

```nginx
http {
    # ... существующие настройки ...

    map $host $project_root {
        default /путь/к/вашему/проекту/src/NginxPro;
    }

    # ... остальные настройки ...
}
```

Замените `/путь/к/вашему/проекту` на реальный путь. Пример:

```nginx
map $host $project_root {
    default /home/directpi/21school/BE3_Nginx_Pro.ID_1269323-1/src/NginxPro;
}
```

2. **Upstream-блоки для балансировки**:

```nginx
upstream backend_primary {
    server localhost:8080 weight=2;
    server localhost:8081 weight=1;
    server localhost:8082 weight=1;
}

upstream backend_write {
    server localhost:8080;
}
```

3. **Подключение конфигов из `conf.d`**:

```nginx
include /etc/nginx/conf.d/*.conf;
```

### 2. Файл `ecommerce.conf`

**Скопировать весь файл** в `/etc/nginx/conf.d/ecommerce.conf`:

```bash
sudo cp /путь/к/проекту/src/NginxPro/nginx/conf.d/ecommerce.conf /etc/nginx/conf.d/ecommerce.conf
```

**Важно:** 
- Файл копируется в `/etc/nginx/conf.d/`, а НЕ в `/etc/nginx/nginx.conf`
- Nginx автоматически подхватит все `.conf` файлы из `/etc/nginx/conf.d/`

## Пошаговая инструкция

### Шаг 1: Обновить системный nginx.conf

```bash
# 1. Создать резервную копию
sudo cp /etc/nginx/nginx.conf /etc/nginx/nginx.conf.backup

# 2. Открыть для редактирования
sudo nano /etc/nginx/nginx.conf

# 3. Найти блок http { и добавить внутри:
#    - map $host $project_root { default /путь/к/вашему/проекту/src/NginxPro; }
#    - upstream backend_primary {...}
#    - upstream backend_write {...}
#    - include /etc/nginx/conf.d/*.conf;

# 4. Сохранить и закрыть (Ctrl+O, Enter, Ctrl+X)
```

### Шаг 2: Скопировать ecommerce.conf

```bash
# Определить путь к проекту (замените на ваш)
PROJECT_PATH="/home/directpi/21school/BE3_Nginx_Pro.ID_1269323-1/src/NginxPro"

# Скопировать конфигурацию
sudo cp "$PROJECT_PATH/nginx/conf.d/ecommerce.conf" /etc/nginx/conf.d/ecommerce.conf
```

### Шаг 3: Проверить синтаксис

```bash
sudo nginx -t
```

Должно вывести: `nginx: configuration file /etc/nginx/nginx.conf test is successful`

### Шаг 4: Перезагрузить Nginx

```bash
sudo nginx -s reload
# или
sudo systemctl reload nginx
```

## Примечание про автоматический режим

Если вы выбираете автоматический режим (через `configure_nginx_from_project.sh`, вариант `1`), скрипт сам:
- создаст резервную копию `/etc/nginx/nginx.conf`;
- перезапишет его содержимым `src/NginxPro/nginx/nginx.conf`, подставив ваш `PROJECT_ROOT`;
- скопирует `nginx/conf.d/ecommerce.conf` в `/etc/nginx/conf.d/ecommerce.conf`;
- выполнит `sudo nginx -t` и `sudo systemctl reload nginx`.

## Проверка установки

После установки проверьте:

```bash
# 1. Проверить, что переменная определена
sudo nginx -T | grep -A 3 "project_root"

# 2. Проверить, что конфиг загружен
sudo nginx -T | grep -A 5 "ecommerce.local"

# 3. Проверить работу endpoints
curl http://localhost/
curl http://localhost/api/v1
curl http://localhost/status
```

## Структура файлов после установки

```
/etc/nginx/
├── nginx.conf              # Системный конфиг (добавлен map $project_root)
└── conf.d/
    └── ecommerce.conf      # Конфигурация проекта (скопирован из проекта)
```

## Важные замечания

1. **Путь к проекту:** Обязательно замените путь в `map $host $project_root` на реальный путь к вашему проекту
2. **Резервная копия:** Всегда создавайте резервную копию перед изменением системных файлов
3. **Проверка синтаксиса:** Всегда проверяйте синтаксис перед перезагрузкой Nginx
4. **Права доступа:** Убедитесь, что у пользователя `www-data` есть права на чтение статических файлов

---

## Этап 5: HTTPS (локальный сертификат)

Далее — ручная настройка HTTPS для `ecommerce.local`.

### 1. Локальный DNS

Добавьте запись в `/etc/hosts`:

```bash
sudo nano /etc/hosts
```

В конец файла:

```text
127.0.0.1    ecommerce.local
```

Проверьте резолвинг:

```bash
ping -c 1 ecommerce.local
```

### 2. Создание самоподписанного сертификата

Создайте директорию для сертификатов и ключа:

```bash
sudo mkdir -p /etc/nginx/ssl
cd /etc/nginx/ssl
```

Сгенерируйте приватный ключ и сертификат (срок действия 365 дней):

```bash
sudo openssl req -x509 -nodes -days 365 \
  -newkey rsa:2048 \
  -keyout ecommerce.local.key \
  -out ecommerce.local.crt \
  -subj "/C=RU/ST=Local/L=Local/O=Ecommerce/OU=Dev/CN=ecommerce.local"
```

Выставьте права:

```bash
sudo chmod 600 /etc/nginx/ssl/ecommerce.local.key
sudo chmod 644 /etc/nginx/ssl/ecommerce.local.crt
```

### 3. Включение HTTPS в конфигурации

В конфигурации `ecommerce.conf` добавлен `server`‑блок для HTTPS:

```nginx
server {
    listen 443 ssl;
    server_name ecommerce.local;

    ssl_certificate     /etc/nginx/ssl/ecommerce.local.crt;
    ssl_certificate_key /etc/nginx/ssl/ecommerce.local.key;

    # ... остальные location, аналогичные HTTP-серверу ...
}
```

По желанию можно настроить редирект с HTTP на HTTPS, добавив в HTTP‑сервере (порт 80) в начало блока:

```nginx
server {
    listen 80;
    server_name ecommerce.local localhost;

    return 301 https://$host$request_uri;

    # ... остальная конфигурация ...
}
```

После изменения конфига не забудьте:

```bash
sudo nginx -t
sudo systemctl reload nginx
```

### 4. Проверка HTTPS

```bash
curl -vk https://ecommerce.local/
curl -vk https://ecommerce.local/api/v1/health
curl -vk https://ecommerce.local/status
```

Браузер будет предупреждать о самоподписанном сертификате — это ожидаемо для учебного проекта.
