Порядок работы с сервером через Postman
========================================

1. Регистрация пользователя
----------------------------
POST /auth/register
https://localhost:8443/auth/register
Headers:
    Content-Type: application/json

Body (raw, JSON):
{
  "login": "player1",
  "password": "password"
}

Ответ:
{
    "authenticated": true,
    "userId": "a6b10ee1-1d30-437f-a9f4-93119ae8cfc2",
    "username": "player1",
    "message": "Пользователь зарегистрирован"
}


2. Авторизация пользователя
----------------------------
POST /auth/login
https://localhost:8443/auth/login
Headers:
    Authorization: Basic (логин:пароль)

Пример: Basic base64(newuser:password)

Ответ: успешная аутентификация (200 OK)
{
    "authenticated": true,
    "userId": "a6b10ee1-1d30-437f-a9f4-93119ae8cfc2",
    "username": "player1",
    "message": "Аутентификация успешна"
}


3. Проверка авторизации
------------------------
GET /auth/check
https://localhost:8443/auth/check
Headers:
    Authorization: Basic (логин:пароль)

Ответ: успешная аутентификация (200 OK)
{
    "authenticated": true,
    "userId": "a6b10ee1-1d30-437f-a9f4-93119ae8cfc2",
    "username": "player1",
    "message": "Пользователь авторизован"
}


4. Выход пользователя
----------------------
POST /auth/logout
https://localhost:8443/auth/logout
Headers:
    Authorization: Basic (логин:пароль)

Ответ: успешный выход (200 OK)
Пример:
{
  "message": "Logout successful"
}


5. Создание игры против компьютера
-----------------------------------
POST /game/create?vsComputer=true
https://localhost:8443/game/create?vsComputer=true
Headers:
    Authorization: Basic (логин:пароль)
    X-Player-Id: <UUID игрока>
Params:
    vsComputer = true
Ответ:
{
  "id" : "8d0fb02a-f63e-4ede-8baa-8e28d0e09a82",
  "board" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ],
  "player1Id" : "99b735f3-d649-4662-a306-8e157f9e82a2",
  "player2Id" : null,
  "state" : "PLAYER_1_TURN",
  "errorMessage" : null,
  "version" : 0,
  "vsComputer" : true,
  "previousBoard" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ]
}


6. Создание игры с другим игроком
---------------------------------
POST /game/create?vsComputer=false
https://localhost:8443/game/create?vsComputer=false
Headers:
    Authorization: Basic (логин:пароль)
    X-Player-Id: <UUID текущего игрока>
Params:
    vsComputer = false
Ответ:
{
  "id" : "7104214e-dc16-409b-9194-eb49095f955b",
  "board" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ],
  "player1Id" : "1991040a-8f96-44fd-bda1-6a66e55dde26",
  "player2Id" : null,
  "state" : "WAITING_FOR_PLAYERS",
  "errorMessage" : null,
  "version" : 0,
  "vsComputer" : false,
  "previousBoard" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ]
}


7. Получение доступных игр
--------------------------
GET /game/available
https://localhost:8443/game/available
Headers:
    Authorization: Basic (логин:пароль)
    <UUID второго игрока>
Ответ:
[ {
  "id" : "db406882-c8eb-4bb7-84ee-9c9ca90a06c7",
  "board" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ],
  "player1Id" : "582909b8-6ef9-4dfe-83fd-7e6fcb780986",
  "player2Id" : null,
  "state" : "WAITING_FOR_PLAYERS",
  "errorMessage" : null,
  "version" : 0,
  "vsComputer" : false,
  "previousBoard" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ]
} ,
{
  "id" : "1991040a-8f96-44fd-bda1-6a66e55dde26",
  "board" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ],
  "player1Id" : "582909b8-6ef9-4dfe-83fd-7e6fcb780986",
  "player2Id" : null,
  "state" : "WAITING_FOR_PLAYERS",
  "errorMessage" : null,
  "version" : 0,
  "vsComputer" : false,
  "previousBoard" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ]
}
]

8. Присоединение к игре
------------------------
POST /game/join/{gameId}
https://localhost:8443/game/join/{{currentGameId}}
Headers:
    Authorization: Basic (логин:пароль)
    X-Player-Id: <UUID второго игрока>

Ответ:
{
  "id" : "db406882-c8eb-4bb7-84ee-9c9ca90a06c7",
  "board" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ],
  "player1Id" : "582909b8-6ef9-4dfe-83fd-7e6fcb780986",
  "player2Id" : "1991040a-8f96-44fd-bda1-6a66e55dde26",
  "state" : "PLAYER_1_TURN",
  "errorMessage" : null,
  "version" : 1,
  "vsComputer" : false,
  "previousBoard" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ]
}


9. Получение игры по ID
------------------------
GET /game/{id}
https://localhost:8443/game/{{currentGameId}}
Headers:
    Authorization: Basic (логин:пароль)
    X-Player-Id : <UUID текущего игрока>

Ответ:
{
  "id" : "8d0fb02a-f63e-4ede-8baa-8e28d0e09a82",
  "board" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ],
  "player1Id" : "99b735f3-d649-4662-a306-8e157f9e82a2",
  "player2Id" : null,
  "state" : "PLAYER_1_TURN",
  "errorMessage" : null,
  "version" : 0,
  "vsComputer" : true,
  "previousBoard" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ]
}


10. Ход игрока
---------------
POST /game/move/{id}
https://localhost:8443/game/move/{{currentGameId}}
Headers:
    Authorization: Basic (логин:пароль)
    X-Player-Id: <UUID игрока>
    Content-Type: application/json

Body:
{
  "board": [
    [0, 0, 0],
    [0, 1, 0],
    [0, 0, 0]
  ],
  "version": {{currentVersion}}
}

Ответ:
{
  "id" : "e3ab5d52-3f87-4ea6-a89e-6381ca45e073",
  "board" : [ [ 0, 0, 0 ], [ 0, 1, 0 ], [ 0, 0, 0 ] ],
  "player1Id" : "a6b10ee1-1d30-437f-a9f4-93119ae8cfc2",
  "player2Id" : "bcfe4ac2-74a2-47ea-b2e0-9cc8437bf318",
  "state" : "PLAYER_2_TURN",
  "errorMessage" : null,
  "version" : 1,
  "vsComputer" : false,
  "previousBoard" : [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 0, 0, 0 ] ]
}


11. Информация о пользователе
------------------------------
GET /user/{id}
https://localhost:8443/user/{{playerId}}
Headers:
    Authorization: Basic (логин:пароль)

Ответ:
{
    "id": "99b735f3-d649-4662-a306-8e157f9e82a2",
    "login": "player1",
    "password": "$2a$10$qgd/qR7Fgm1RrkobAy8ww.ZDyvXgmH/veyjknrt7A3cDkJD/JfUHy",
    "enabled": true,
    "username": "player1",
    "authorities": [
        {
            "authority": "USER"
        }
    ],
    "accountNonExpired": true,
    "accountNonLocked": true,
    "credentialsNonExpired": true
}


12. Главная страница
---------------------
GET /
https://localhost:8443/
Headers:
    Authorization: Basic (логин:пароль)

Ответ:
Добро пожаловать в Tic-Tac-Toe API! Используйте /auth для авторизации и /game для игры.


Примечания
-----------
- Для всех защищённых эндпоинтов необходимо указывать заголовки:
  * Authorization: Basic base64(login:пароль)
  * X-Player-Id: UUID игрока (если требуется)

- Для игры с компьютером поле player2Id = null, vsComputer = true.

- Версия игры (version) увеличивается после каждого хода.

Системные настройки
--------------------
При необходимости можно переключить используемую версию Java:
sudo update-alternatives --config java
