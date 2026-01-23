[К оглавлению](../README.md)

# Проект: RESTfull_API_MyShop

В этом проекте создано RESTful API на языке **Java** с использованием **Spring Framework** для управления электронным магазином бытовой техники. Проект реализует полный CRUD функционал для клиентов, товаров, поставщиков, адресов и изображений.

## Содержание
- [Общая информация](#общая-информация)
- [Технологический стек](#технологический-стек)
- [Функциональность](#функциональность)
- [Структура проекта](#структура-проекта)
- [Реализация domain-слоя](#реализация-domain-слоя)
- [Реализация datasource-слоя](#реализация-datasource-слоя)
- [Реализация web-слоя](#реализация-web-слоя)
- [Реализация di-слоя](#реализация-di-слоя)
- [Безопасность](#безопасность)
- [База данных](#база-данных)
- [API Endpoints](#api-endpoints)
- [Запуск проекта](#запуск-проекта)
- [Тестирование](#тестирование)
- [Автор проекта](#автор-проекта)

## Общая информация

Проект представляет собой RESTful API для управления электронным магазином с поддержкой:

* Управления клиентами (CRUD операции, поиск по имени/фамилии, пагинация)
* Управления товарами (CRUD операции, управление запасами)
* Управления поставщиками (CRUD операции, изменение адресов)
* Управления изображениями товаров (загрузка, получение, обновление, удаление)
* Управления адресами (создание, обновление)
* Валидации входных данных
* Обработки ошибок с корректными HTTP статусами
* OpenAPI/Swagger документации

## Технологический стек

* **Java 21** – основной язык программирования  
* **Spring Boot 3.4.3** – основной фреймворк  
* **Spring Security** – аутентификация и авторизация (HTTP Basic Auth)  
* **Spring Data JDBC** – работа с базой данных через JDBC  
* **PostgreSQL** – реляционная база данных  
* **Lombok** – сокращение boilerplate кода  
* **Jackson** – работа с JSON  
* **SpringDoc OpenAPI** – документация API (Swagger UI)  
* **Spring Boot Actuator** – мониторинг и health checks  
* **Gradle** – система сборки  

## Функциональность

* ✅ CRUD операции для клиентов, товаров, поставщиков, изображений  
* ✅ Поиск клиентов по имени и фамилии  
* ✅ Пагинация для списка клиентов  
* ✅ Управление запасами товаров (увеличение/уменьшение)  
* ✅ Загрузка и получение изображений товаров (binary data)  
* ✅ Валидация входных данных (Bean Validation + кастомная валидация)  
* ✅ Обработка ошибок с корректными HTTP статусами (400, 404, 500)  
* ✅ OpenAPI/Swagger документация  
* ✅ Health check endpoints  
* ✅ HTTP Basic аутентификация  

## Структура проекта

Проект разделен на слои по принципам чистой архитектуры:

* web  
* domain  
* datasource  
* di (config)  

```plaintext
src/
├── main/
│   ├── java/example/
│   │   ├── config/                  # DI (Configuration)
│   │   │   ├── DataJdbcConfig.java
│   │   │   ├── OpenApiConfig.java
│   │   │   └── SecurityConfig.java
│   │   ├── datasource/              # Datasource layer
│   │   │   ├── mapper/              # Мапперы Domain ↔ DAO
│   │   │   │   ├── AddressRowMapper.java
│   │   │   │   ├── CategoryRowMapper.java
│   │   │   │   ├── ClientRowMapper.java
│   │   │   │   ├── ProductRowMapper.java
│   │   │   │   └── SupplierRowMapper.java
│   │   │   ├── model/               # DAO объекты (Data Access Objects)
│   │   │   │   ├── AddressDAO.java
│   │   │   │   ├── CategoryDAO.java
│   │   │   │   ├── ClientDAO.java
│   │   │   │   ├── ImageDAO.java
│   │   │   │   ├── ProductDAO.java
│   │   │   │   └── SupplierDAO.java
│   │   │   └── repository/          # Репозитории
│   │   │       ├── AddressRepository.java
│   │   │       ├── CategoryRepository.java
│   │   │       ├── ClientRepository.java
│   │   │       ├── ImageRepository.java
│   │   │       ├── ProductRepository.java
│   │   │       ├── SupplierRepository.java
│   │   │       └── impl/             # JDBC реализации
│   │   │           ├── JdbcAddressRepository.java
│   │   │           ├── JdbcCategoryRepository.java
│   │   │           ├── JdbcClientRepository.java
│   │   │           ├── JdbcImageRepository.java
│   │   │           ├── JdbcProductRepository.java
│   │   │           └── JdbcSupplierRepository.java
│   │   ├── domain/                  # Domain layer
│   │   │   ├── exception/           # Доменные исключения
│   │   │   │   ├── CreateEntityException.java
│   │   │   │   ├── DeleteEntityException.java
│   │   │   │   ├── EntityNotFoundException.java
│   │   │   │   ├── ServiceException.java
│   │   │   │   ├── UpdateEntityException.java
│   │   │   │   └── ValidationException.java
│   │   │   ├── mapper/              # Мапперы Domain ↔ DTO
│   │   │   │   ├── AddressMapper.java
│   │   │   │   ├── ClientMapper.java
│   │   │   │   ├── ImageMapper.java
│   │   │   │   ├── ProductMapper.java
│   │   │   │   └── SupplierMapper.java
│   │   │   ├── model/               # Доменные модели (DTO)
│   │   │   │   ├── AddressDTO.java
│   │   │   │   ├── ClientDTO.java
│   │   │   │   ├── ImageDTO.java
│   │   │   │   ├── ProductDTO.java
│   │   │   │   └── SupplierDTO.java
│   │   │   └── service/             # Сервисный слой
│   │   │       ├── ClientService.java
│   │   │       ├── ImageService.java
│   │   │       ├── ProductService.java
│   │   │       └── SupplierService.java
│   │   └── web/                     # Web слой
│   │       ├── controller/          # REST-контроллеры
│   │       │   ├── ClientController.java
│   │       │   ├── HealthController.java
│   │       │   ├── ImageController.java
│   │       │   ├── ProductController.java
│   │       │   └── SupplierController.java
│   │       ├── exception/
│   │       │   └── GlobalExceptionHandler.java
│   │       ├── mapper/              # Мапперы Web ↔ Domain
│   │       │   ├── ClientWebMapper.java
│   │       │   ├── ImageWebMapper.java
│   │       │   ├── ProductWebMapper.java
│   │       │   └── SupplierWebMapper.java
│   │       └── model/
│   │           ├── request/          # Request модели
│   │           │   ├── CreateClientRequest.java
│   │           │   ├── CreateProductRequest.java
│   │           │   ├── CreateSupplierRequest.java
│   │           │   ├── UpdateAddressRequest.java
│   │           │   └── UpdateStockRequest.java
│   │           └── response/        # Response модели
│   │               ├── AddressResponse.java
│   │               ├── ClientResponse.java
│   │               ├── ErrorResponse.java
│   │               ├── ImageResponse.java
│   │               ├── ProductResponse.java
│   │               └── SupplierResponse.java
│   └── resources/
│       ├── application.properties
│       ├── schema.sql               # SQL схема БД
│       └── data.sql                 # Тестовые данные
└── test/
    └── (тесты отсутствуют)
```

**Дополнительно:**
```plaintext
Postman_collection/
└── E-Commerce API.postman_collection.json

scripts/
└── init_db.sh                      # Скрипт инициализации БД
```

## Реализация domain-слоя

### Модели данных (DTO)
* **ClientDTO** – модель клиента с полями:
    * UUID id - уникальный идентификатор
    * String firstName, lastName - имя и фамилия
    * LocalDate birthday - дата рождения
    * String gender - пол (MALE/FEMALE)
    * LocalDate registrationDate - дата регистрации
    * UUID addressId - идентификатор адреса
* **ProductDTO** – модель товара с полями:
    * UUID id - уникальный идентификатор
    * String name - название товара
    * String categoryCode - код категории
    * BigDecimal price - цена
    * Integer availableStock - количество на складе
    * LocalDate lastUpdateDate - дата последнего обновления
    * UUID supplierId - идентификатор поставщика
    * UUID imageId - идентификатор изображения
* **SupplierDTO** – модель поставщика
* **AddressDTO** – модель адреса (country, city, street)
* **ImageDTO** – модель изображения (byte[] imageData)

### Сервисы
* **ClientService** – управление клиентами (создание, поиск, удаление, обновление адреса, пагинация)
* **ProductService** – управление товарами (создание, получение, удаление, управление запасами)
* **SupplierService** – управление поставщиками (создание, получение, удаление, обновление адреса)
* **ImageService** – управление изображениями (загрузка, получение, обновление, удаление)

### Валидация
* Bean Validation аннотации в Request моделях (`@NotBlank`, `@NotNull`, `@Size`, `@DecimalMin`, `@DecimalMax`)
* Кастомная валидация в сервисах (проверка категорий, дат, цен, количества)

## Реализация datasource-слоя

### DAO объекты
* **ClientDAO**, **ProductDAO**, **SupplierDAO**, **AddressDAO**, **ImageDAO**, **CategoryDAO** – объекты для работы с БД

### Репозитории
* **JdbcClientRepository** – CRUD операции для клиентов, поиск по имени/фамилии, пагинация
* **JdbcProductRepository** – CRUD операции для товаров, управление запасами
* **JdbcSupplierRepository** – CRUD операции для поставщиков
* **JdbcAddressRepository** – CRUD операции для адресов
* **JdbcImageRepository** – CRUD операции для изображений
* **JdbcCategoryRepository** – получение категорий

### Мапперы
* **RowMapper** реализации для преобразования ResultSet → DAO
* Использование параметризованных запросов (защита от SQL injection)

## Реализация web-слоя

### Контроллеры
* **ClientController** – управление клиентами (`/api/v1/clients`)
* **ProductController** – управление товарами (`/api/v1/products`)
* **SupplierController** – управление поставщиками (`/api/v1/suppliers`)
* **ImageController** – управление изображениями (`/api/v1/images`)
* **HealthController** – health check endpoints (`/api/v1/health`)

### DTO модели
* **Request модели** – `CreateClientRequest`, `CreateProductRequest`, `CreateSupplierRequest`, `UpdateAddressRequest`, `UpdateStockRequest`
* **Response модели** – `ClientResponse`, `ProductResponse`, `SupplierResponse`, `AddressResponse`, `ImageResponse`, `ErrorResponse`

### Обработка ошибок
* **GlobalExceptionHandler** – централизованная обработка исключений:
    * `ValidationException` → 400 Bad Request
    * `EntityNotFoundException` → 404 Not Found
    * `MethodArgumentNotValidException` → 400 Bad Request (с деталями валидации)
    * `MethodArgumentTypeMismatchException` → 400 Bad Request
    * `Exception` → 500 Internal Server Error (с логированием)

## Реализация di-слоя

### Конфигурация
* **DataJdbcConfig** – настройка Spring Data JDBC
* **SecurityConfig** – настройка безопасности, фильтров, CORS, HTTP Basic Auth
* **OpenApiConfig** – настройка Swagger/OpenAPI документации

### Внедрение зависимостей
* Автоматическое связывание через `@Autowired`
* Явная конфигурация бинов в конфигурационных классах
* Профили и `properties`-конфигурация

## Безопасность

* **HTTP Basic Authentication** – аутентификация через заголовок `Authorization: Basic <base64>`
* **InMemory UserDetailsService** – демо-пользователи (ТОЛЬКО для разработки!)
    * Учетные данные читаются из переменных окружения (`.env` файл)
    * В продакшене необходимо заменить на реализацию с БД (`JdbcUserDetailsManager` или кастомный `UserDetailsService`)
* **Password Encoding** – BCrypt
* **CORS** – настроен для `localhost:3000` и `localhost:8080` с `allowCredentials=true`
* **CSRF** – отключен для REST API
* **Swagger/OpenAPI** – публично доступен
* **GlobalExceptionHandler** – централизованная обработка исключений (без раскрытия внутренней информации)
* **SQL Injection Protection** – использование параметризованных запросов (JdbcTemplate)
* **Input Validation** – Bean Validation + кастомная валидация в сервисах

**ВАЖНО**: Демо-пользователи используются ТОЛЬКО для разработки. В продакшене необходимо использовать базу данных и переменные окружения для хранения учетных данных.

## База данных

### Схема данных
* **clients** – клиенты (id, client_name, client_surname, birthday, gender, registration_date, address_id)
* **products** – товары (id, name, category_id, price, available_stock, last_update_date, supplier_id, image_id)
* **suppliers** – поставщики (id, name, address_id, phone_number)
* **addresses** – адреса (id, country, city, street)
* **images** – изображения (id, image_data)
* **categories** – категории товаров (id, code, display_name)

### Особенности
* Использование UUID для первичных ключей
* Внешние ключи с каскадными операциями (ON DELETE RESTRICT/SET NULL, ON UPDATE CASCADE)
* CHECK ограничения для валидации данных (даты, цены, пол, телефон)
* Индексы для оптимизации запросов

### Инициализация
* SQL скрипты: `schema.sql` (схема) и `data.sql` (тестовые данные)
* Скрипт `scripts/init_db.sh` для автоматической инициализации БД

## API Endpoints

### Клиенты (`/api/v1/clients`)
* `POST /api/v1/clients` – создание клиента
* `GET /api/v1/clients/{id}` – получение клиента по ID
* `DELETE /api/v1/clients/{id}` – удаление клиента
* `GET /api/v1/clients?firstName={name}&lastName={surname}` – поиск по имени и фамилии
* `GET /api/v1/clients?limit={limit}&offset={offset}` – получение всех клиентов с пагинацией
* `PATCH /api/v1/clients/{id}/address` – обновление адреса клиента

### Товары (`/api/v1/products`)
* `POST /api/v1/products` – создание товара
* `GET /api/v1/products/{id}` – получение товара по ID
* `GET /api/v1/products/available` – получение всех доступных товаров
* `DELETE /api/v1/products/{id}` – удаление товара
* `PATCH /api/v1/products/{id}/stock/decrease` – уменьшение количества товара
* `PATCH /api/v1/products/{id}/stock/increase` – увеличение количества товара
* `GET /api/v1/products/{id}/stock` – получение текущего количества товара

### Поставщики (`/api/v1/suppliers`)
* `POST /api/v1/suppliers` – создание поставщика
* `GET /api/v1/suppliers/{id}` – получение поставщика по ID
* `GET /api/v1/suppliers` – получение всех поставщиков
* `DELETE /api/v1/suppliers/{id}` – удаление поставщика
* `PATCH /api/v1/suppliers/{id}/address` – обновление адреса поставщика

### Изображения (`/api/v1/images`)
* `POST /api/v1/images?productId={id}` – загрузка изображения (Content-Type: application/octet-stream)
* `GET /api/v1/images/{id}` – получение изображения по ID (Content-Type: application/octet-stream)
* `GET /api/v1/images/product/{productId}` – получение изображения товара
* `PUT /api/v1/images/{id}` – обновление изображения
* `DELETE /api/v1/images/{id}` – удаление изображения

### Health Check (`/api/v1/health`)
* `GET /api/v1/health` – проверка состояния приложения

### Swagger UI
* `GET /swagger/index.html` – Swagger UI документация
* `GET /api-docs` – OpenAPI спецификация (JSON)

## Запуск проекта

### Требования
* Java 21+  
* PostgreSQL 12+  
* Gradle 7.4+ (или использовать Gradle Wrapper)  

### Настройка

1. **Создать файл `.env` в корне проекта:**
```properties
APP_NAME=E-Commerce API
APP_VERSION=1.0.0
SERVER_PORT=8080

DATABASE_HOST=localhost
DATABASE_PORT=5432
DATABASE_NAME=your_database_name
DATABASE_USERNAME=your_database_user
DATABASE_PASSWORD=your_database_password

PG_HOST=localhost
PG_PORT=5432
PG_USER=your_postgres_admin_user
PG_PASSWORD=your_postgres_admin_password

# Security - Demo Users (только для разработки!)
# Учетные данные для демо-пользователей (в продакшене использовать БД!)
DEMO_USER_USERNAME=your_demo_user
DEMO_USER_PASSWORD=your_demo_user_password
DEMO_USER_ROLES=USER

DEMO_ADMIN_USERNAME=your_demo_admin
DEMO_ADMIN_PASSWORD=your_demo_admin_password
DEMO_ADMIN_ROLES=ADMIN,USER
```

2. **Инициализация базы данных:**
```bash
# Автоматическая инициализация (создание БД, пользователя, схемы, данных)
./gradlew initDb

# Или вручную через скрипт
bash scripts/init_db.sh init
```

3. **Проверка подключения к БД:**
```bash
./gradlew checkDb
# или
bash scripts/init_db.sh check
```

### Запуск
```bash
# Запуск приложения
./gradlew bootRun

# Сборка проекта
./gradlew build

# Проверка Java версии и порта перед запуском
./gradlew checkJava checkPort
```

После запуска:
* Приложение доступно на `http://localhost:8080`
* Swagger UI: `http://localhost:8080/swagger/index.html`
* Health check: `http://localhost:8080/api/v1/health`
* API Base URL: `http://localhost:8080/api/v1`

### Аутентификация

Для доступа к защищенным endpoints используйте HTTP Basic Authentication.

**Демо-пользователи (только для разработки!):**
Учетные данные настраиваются через переменные окружения в `.env` файле:
* `DEMO_USER_USERNAME` / `DEMO_USER_PASSWORD` / `DEMO_USER_ROLES` (роль USER)
* `DEMO_ADMIN_USERNAME` / `DEMO_ADMIN_PASSWORD` / `DEMO_ADMIN_ROLES` (роли ADMIN, USER)

Пример с curl (замените на ваши учетные данные из `.env`):
```bash
curl -u <username>:<password> http://localhost:8080/api/v1/clients
```

**ВАЖНО**: В продакшене необходимо заменить `InMemoryUserDetailsManager` на реализацию, использующую базу данных (например, `JdbcUserDetailsManager` или кастомную реализацию `UserDetailsService`).

## Тестирование

### Postman коллекция
В проекте присутствует Postman коллекция `Postman_collection/E-Commerce API.postman_collection.json` с примерами запросов для всех endpoints.

## Автор проекта
**directpi**

[В начало](#проект-restfull_api_myshop)
