[К оглавлению](../README.md)

### Проект: Java_Bootcamp (Tic_Tac_Toe)

В этом проекте создано веб-приложение на языке **Java** с использованием **Spring Framework** для реализации многопользовательской игры в крестики-нолики с поддержкой нескольких игроков и игры против ИИ (алгоритм MiniMax).

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

Проект представляет собой RESTful API для игры в крестики-нолики с поддержкой:

* Игры против компьютера с AI на алгоритме minimax
* Многопользовательской игры между двумя игроками
* Системы аутентификации и авторизации (JWT, access/refresh)
* Оптимистичной блокировки для обработки конкурентных ходов
* Хранения состояния игр в PostgreSQL

## Технологический стек

* **Java 18+** – основной язык программирования  
* **Spring Boot** – основной фреймворк  
* **Spring Security** – аутентификация и авторизация  
* **Spring Data JPA** – работа с базой данных  
* **PostgreSQL** – реляционная база данных  
* **Flyway** – миграции базы данных  
* **Lombok** – сокращение boilerplate кода  
* **GSON** – работа с JSON  
* **SSL/TLS** – защищенное соединение  

## Функциональность

* ✅ Регистрация и аутентификация пользователей  
* ✅ Создание игр (против компьютера или другого игрока)  
* ✅ Присоединение к существующим играм  
* ✅ Оптимистичная блокировка для конкурентных ходов  
* ✅ AI для игры против компьютера (алгоритм minimax)  
* ✅ Валидация ходов и состояний игры  
* ✅ Управление жизненным циклом игр  
* ✅ Swagger/OpenAPI документация API  
* ✅ HTTPS поддержка  

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
│   │   │   ├── AppConfig.java
│   │   │   └── SecurityConfig.java
│   │   ├── datasource/              # Datasource layer
│   │   │   ├── mapper/              # Мапперы Domain ↔ Entity
│   │   │   │   └── GameMapper.java
│   │   │   ├── model/               # JPA-сущности
│   │   │   │   └── GameEntity.java
│   │   │   └── repository/          # Репозитории
│   │   │       ├── GameRepository.java
│   │   │       └── UserRepository.java
│   │   ├── domain/                  # Domain layer
│   │   │   ├── model/               # Доменные модели
│   │   │   │   ├── Game.java
│   │   │   │   ├── GameBoard.java
│   │   │   │   ├── GameState.java
│   │   │   │   ├── PlayerStats.java
│   │   │   │   ├── Role.java
│   │   │   │   └── User.java
│   │   │   └── service/             # Сервисный слой
│   │   │       ├── AuthService.java
│   │   │       ├── AuthServiceImpl.java
│   │   │       ├── JwtProvider.java
│   │   │       ├── GameService.java
│   │   │       ├── GameServiceImpl.java
│   │   │       ├── UserService.java
│   │   │       └── UserServiceImpl.java
│   │   ├── utils/                   # Утилиты
│   │   │   ├── GameStateConverter.java
│   │   │   ├── IntArrayConverter.java
│   │   │   ├── JwtUtil.java
│   │   │   └── Utils.java
│   │   └── web/                     # Web слой
│   │       ├── controller/          # REST-контроллеры
│   │       │   ├── AuthController.java
│   │       │   ├── GameController.java
│   │       │   ├── HomeController.java
│   │       │   └── UserController.java
│   │       ├── exception/
│   │       │   ├── ErrorResponse.java
│   │       │   └── GlobalExceptionHandler.java
│   │       ├── filter/
│   │       │   ├── AuthFilter.java
│   │       │   └── JwtAuthentication.java
│   │       ├── mapper/
│   │       │   └── GameMapper.java
│   │       └── model/
│   │           ├── AuthStatusResponse.java
│   │           ├── GameDTO.java
│   │           ├── JwtRequest.java
│   │           ├── JwtResponse.java
│   │           ├── LoginRequest.java
│   │           ├── MoveRequestDTO.java
│   │           ├── PlayerStatsDTO.java
│   │           ├── RefreshJwtRequest.java
│   │           ├── SignUpRequest.java
│   │           └── UserDTO.java
│   └── resources/
│       ├── application.properties
│       ├── keystore.jks
│       └── migration/
│           └── V1__Create_tables.sql
└── test/
    ├── java/example/...             # Юнит и интеграционные тесты
    └── resources/application-test.properties
```

Дополнительно:
```plaintext
Postman/
├── TicTacToe_JWT_API_Complete.postman_collection.json
└── Player N.postman_environment.json (N = 1..6)
```

## Реализация domain-слоя

### Модели данных
* **Game** – основная модель игры, содержащая 

    * UUID id - уникальный идентификатор игры
    * GameBoard board - игровое поле   
    * int[][] previousBoard - предыдущее состояние доски   
    * GameState state - текущее состояние игры   
    * UUID player1Id, UUID player2Id - идентификаторы игроков   
    * boolean vsComputer - флаг игры против компьютера   
    * Integer version - версия для оптимистичной блокировки   


* **GameBoard** – представление игрового поля в виде целочисленной матрицы 3x3  
    * 0 - пустая клетка
    * 1 - крестик (игрок 1)
    * 2 - нолик (игрок 2 или компьютер)
* **GameState** – enum с состояниями игры:
    * WAITING_FOR_PLAYERS - ожидание второго игрока
    * PLAYER_1_TURN - ход первого игрока
    * PLAYER_2_TURN - ход второго игрока
    * PLAYER_1_WIN - победа первого игрока
    * PLAYER_2_WIN - победа второго игрока
    * DRAW - ничья 
* **User** – Модель пользователя с реализацией Spring Security `UserDetails`:

    * UUID id - уникальный идентификатор
    * String login - уникальный логин
    * String password - хешированный пароль

### Сервисы
* **GameService** – управление игровым процессом, валидация ходов, AI компьютера  
* **AuthService** – аутентификация и регистрация пользователей  
* **UserService** – управление пользователями  

## Реализация datasource-слоя

### JPA сущности
* **GameEntity** – сущность для хранения игр в БД с конвертацией состояний и массивов  
* **User** – сущность пользователя с хешированием паролей  

### Репозитории
* **GameRepository** – кастомные запросы для поиска активных игр  
* **UserRepository** – поиск пользователей по логину  

### Мапперы
* **GameMapper** – преобразование между domain-моделью `Game` и JPA-сущностью `GameEntity`  

## Реализация web-слоя

### Контроллеры
* **GameController** – управление играми (создание, ходы, получение состояния)  
* **AuthController** – регистрация, аутентификация, проверка статуса, обновление токенов  
* **UserController** – информация о пользователях  

### DTO модели
* **GameDTO** – transfer object для игр  
* **MoveRequestDTO** – запрос на выполнение хода с версией  
* **AuthStatusResponse** – ответ со статусом аутентификации  
* **JwtRequest/JwtResponse/RefreshJwtRequest** – модели для JWT аутентификации и обновления токенов  

## Реализация di-слоя

### Конфигурация
* **AppConfig** – основная конфигурация Spring, component scanning  
* **SecurityConfig** – настройка безопасности, фильтров, CORS/SSL  

### Внедрение зависимостей
* Автоматическое связывание через `@Autowired`  
* Явная конфигурация бинов в `SecurityConfig`  
* Профили и `properties`-конфигурация  

## Безопасность
* **JWT (Access/Refresh)** – аутентификация через заголовок `Authorization: Bearer <access_token>`
* **Обновление токенов** – эндпоинты для обновления access и refresh токенов
* **AuthFilter** – проверка JWT, наполнение `SecurityContext` (`JwtAuthentication`)
* **Password Encoding** – BCrypt
* **HTTPS** – поддержка через `keystore.jks` (см. настройки)
* **CSRF** – отключен для REST API
* **Swagger/OpenAPI** – публично доступен
* **GlobalExceptionHandler** – централизованная обработка исключений

## База данных

### Миграции
* **Flyway** – управление версиями схемы БД
* **V1__Create_tables.sql** – инициализация таблиц `users`, `games`, `moves`

### Модель данных
* Хранение состояния игр с поддержкой версионности  
* Связи между пользователями и играми  
* Конвертация сложных типов (массивы, enum) в JSON  

## API Endpoints

### Аутентификация
* `POST /auth/register` – регистрация пользователя
* `POST /auth/login` – получение пары токенов (access, refresh)
* `POST /auth/refresh` – обновление access токена по действующему refresh токену
* `POST /auth/refresh-token` – перевыпуск refresh токена
* `GET /auth/check` – проверка статуса аутентификации
* `POST /auth/logout` – выход из системы

### Игры
* `POST /game/create?vsComputer={boolean}` – создание новой игры  
* `GET /game/available` – список доступных игр  
* `POST /game/join/{gameId}` – присоединение к игре  
* `GET /game/{gameId}` – получение информации об игре  
* `POST /game/move/{gameId}` – выполнение хода  
* `GET /user/{id}` – информация о пользователе  

## Запуск проекта

### Требования
* Java 18+  
* PostgreSQL 12+  
* Gradle 7.4+  

### Настройка
* Создать БД `tictactoe_db` в PostgreSQL
* Настроить `application.properties` (БД, JWT, SSL, Swagger при необходимости)
  - `spring.datasource.url=jdbc:postgresql://localhost:5432/tictactoe_db`
  - `spring.datasource.username=...`
  - `spring.datasource.password=...`
  - `jwt.secret=<достаточно_длинный_секрет>`
  - `jwt.access.expiration=3600000`            # 1 час
  - `jwt.refresh.expiration=2592000000`        # 30 дней
  - SSL (опционально): `server.ssl.key-store=classpath:keystore.jks`, `server.ssl.key-store-password=...`, `server.port=8443`
* Сгенерировать SSL-сертификат или использовать предоставленный `keystore.jks`

### Запуск
```bash
# Запуск приложения
./gradlew bootRun

# Сборка проекта
./gradlew build
```

## Тестирование
### Тесты
В проекте присутствуют юнит/интеграционные тесты для фильтров безопасности, сервисов домена и REST-контроллеров (`src/test/java/example/...`).

### Postman коллекция  
- Полное покрытие сценариев: регистрация/логин, создание/присоединение к игре, ходы, проверка статуса
- Автоматизация: pre-request скрипты управляют `accessToken`/`refreshToken`
- Окружения для одновременной игры несколькими пользователями (`Player 1..6`)
- Готово к прогону в Postman и Newman
 - Тестовые сценарии для двух игроков
 - Поддержка переменных окружения
 - Автоматическое управление состоянием игр
 - Тестовые пользователи: по умолчанию `myserver`
 - Поддерживается регистрация новых пользователей
 - Отображение состояние игровой доски в консоли Postman

## Автор проекта
**directpi**  
