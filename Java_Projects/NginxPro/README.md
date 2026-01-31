[К оглавлению](../README.md)

# Проект: NginxPro

В этом проекте реализовано **E-Commerce REST API** на **Java (Spring Boot)** с **Nginx** в качестве reverse proxy: балансировка нагрузки между несколькими инстансами приложения, кеширование, gzip, HTTPS. Основной инстанс (порт 8080) имеет полный доступ к БД; инстансы на портах 8081 и 8082 работают с read-only пользователем БД для распределения GET-запросов.

## Содержание
- [Общая информация](#общая-информация)
- [Технологический стек](#технологический-стек)
- [Функциональность](#функциональность)
- [Структура проекта](#структура-проекта)
- [Архитектура](#архитектура)
- [Безопасность и креды](#безопасность-и-креды)
- [Запуск проекта](#запуск-проекта)
- [Nginx](#nginx)
- [Тестирование](#тестирование)
- [Автор проекта](#автор-проекта)

## Общая информация

Проект объединяет:

* **Spring Boot** приложение — тот же E-Commerce API (клиенты, товары, поставщики, изображения), что и в RESTfull_API_MyShop
* **Nginx** — reverse proxy, маршрутизация по путям (`/`, `/api`, `/admin`), балансировка нагрузки по upstream (8080 / 8081 / 8082), кеширование ответов, gzip, HTTPS для `ecommerce.local`
* **PostgreSQL** — основное приложение подключается с полными правами; отдельный read-only пользователь для инстансов 8081 и 8082
* **Скрипты** — инициализация БД, создание read-only пользователя, запуск/остановка инстансов, полная проверка задания

## Технологический стек

* **Java 21** – язык приложения  
* **Spring Boot 3.4.3** – фреймворк  
* **Spring Security** – HTTP Basic Auth  
* **Spring Data JDBC** – работа с PostgreSQL  
* **PostgreSQL** – база данных  
* **Nginx** – reverse proxy, балансировка, кеш, gzip, HTTPS  
* **Gradle** – сборка  
* **SpringDoc OpenAPI** – Swagger UI  

## Функциональность

* ✅ E-Commerce API (CRUD: клиенты, товары, поставщики, изображения)  
* ✅ Несколько инстансов приложения: 8080 (запись в БД), 8081 и 8082 (только чтение)  
* ✅ Nginx: маршрутизация `/`, `/api`, `/admin`, редирект `/api/v1` → Swagger  
* ✅ Балансировка нагрузки по GET-запросам между 8080, 8081, 8082  
* ✅ Write-запросы только на 8080  
* ✅ Кеширование и gzip в Nginx  
* ✅ HTTPS на `ecommerce.local` (самоподписанный сертификат)  
* ✅ Учетные данные через переменные окружения  

## Структура проекта

```plaintext
NginxPro/
├── build.gradle.kts
├── settings.gradle.kts
├── gradlew, gradlew.bat
├── example.env                 # Шаблон переменных (копировать в .env)
├── howToRun.md                 # Подробные инструкции по службам и запуску
├── nginx/
│   ├── nginx.conf              # Основной конфиг  
│   ├── conf.d/
│   │   └── ecommerce.conf      # Виртуальный хост (маршрутизация, upstream, кеш, gzip)  
│   ├── CONFIGURE_NGINX_MANUAL.md
│   └── static/                 # Статика (index.html, image.png)
├── Postman_collection/
│   └── E-Commerce API.postman_collection.json
├── scripts/
│   ├── init_db.sh              # Инициализация БД (из .env)
│   ├── create_readonly_user.sh # Создание read-only пользователя БД
│   ├── start_instances.sh      # Запуск 8080, 8081, 8082
│   ├── stop_instances.sh       # Остановка инстансов
│   ├── configure_nginx_from_project.sh
│   └── test_full_assignment.sh # Проверка маршрутизации, балансировки, кеша, gzip, HTTPS
└── src/
    └── main/
        ├── java/example/       # Как в RESTfull_API_MyShop (config, datasource, domain, web)
        └── resources/
            ├── application.properties
            ├── schema.sql
            ├── data.sql
            └── create_readonly_user.sql
```

## Архитектура

* **Порт 8080** — основной инстанс: полный доступ к БД (SELECT, INSERT, UPDATE, DELETE). Используется для всех write-запросов и по умолчанию для части GET.
* **Порты 8081, 8082** — инстансы с подключением к БД под read-only пользователем (только SELECT). Nginx направляет сюда часть GET-запросов для балансировки.
* **Nginx** — принимает запросы на 80/443, проксирует на `backend_primary` (балансировка 8080/8081/8082) или на `backend_write` (только 8080) для write и Swagger.

## Безопасность и креды

* Рекомендуется: скопировать `example.env` в `.env`, подставить свои значения и **никогда не коммитить `.env`**.

Переменные в `example.env`:

* Приложение: `APP_NAME`, `APP_VERSION`, `SERVER_PORT`
* БД для приложения: `DATABASE_HOST`, `DATABASE_PORT`, `DATABASE_NAME`, `DATABASE_USERNAME`, `DATABASE_PASSWORD`
* Админ БД (скрипты): `PG_HOST`, `PG_USER`, `PG_PASSWORD`
* Read-only пользователь (инстансы 8081, 8082): `READONLY_USER`, `READONLY_USER_PASSWORD`

Демо-учетки и пароли в документации (например, в `howToRun.md`) — только для локальной разработки; в продакшене использовать свои значения и хранить их в `.env` или секрет-хранилище.

## Запуск проекта

### Требования
* Java 21+  
* PostgreSQL 12+  
* Nginx (для полного сценария)  
* Gradle (или Gradle Wrapper в проекте)  

### Настройка

1. **Создать `.env` из шаблона (креды не коммитить):**
   ```bash
   cp example.env .env
   # Отредактировать .env: указать реальные DATABASE_*, PG_*, READONLY_*
   ```

2. **Инициализация БД:**
   ```bash
   ./scripts/init_db.sh init
   ```

3. **Создание read-only пользователя БД (для инстансов 8081, 8082):**
   ```bash
   ./scripts/create_readonly_user.sh
   ```

4. **Запуск инстансов приложения (8080, 8081, 8082):**
   ```bash
   ./scripts/start_instances.sh
   ```
   Остановка: `./scripts/stop_instances.sh`

5. **Только один инстанс (без Nginx):**
   ```bash
   ./gradlew bootRun
   ```
   Переменные берутся из `.env` (через `application.properties` и `spring.config.import`).

Подробные шаги по установке Nginx, PostgreSQL, OpenSSL, настройке HTTPS и `/etc/hosts` — в **howToRun.md**.

## Nginx

* Конфиги проекта: `nginx/nginx.conf`, `nginx/conf.d/ecommerce.conf`.
* Для применения скопировать конфиги в систему (или указать в Nginx свой путь к проекту) и перезагрузить Nginx. Детали — в `nginx/CONFIGURE_NGINX_MANUAL.md` и `howToRun.md`.
* Домен для HTTPS: `ecommerce.local` (добавить в `/etc/hosts` и при необходимости в `C:\Windows\System32\drivers\etc\hosts`).

## Тестирование

* **Postman:** коллекция `Postman_collection/E-Commerce API.postman_collection.json`.
* **Полная проверка задания (Nginx, балансировка, кеш, gzip, HTTPS):**
  ```bash
  ./scripts/test_full_assignment.sh
  ```
  Результаты пишутся в `logs/test_full_assignment_*.log`.

## Автор проекта
**directpi**

[В начало](#проект-nginxpro)
