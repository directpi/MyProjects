# API Тесты

## Описание
- Покрываются основные бизнес-сценарии: авторизация (login, register, refresh, healthcheck), полный CRUD для User и Vehicle, списочные и статистические коллекции, негативные сценарии (валидация и доступ).
- Тестируется ~13 ключевых эндпоинтов — для QA-инфраструктуры (см. docs/api_endpoints_summary.md).

## Установка

### Виртуальное окружение (рекомендуется)
```bash
python3 -m venv .venv
source .venv/bin/activate
# Установка зависимостей из корня репозитория
pip install -r tests/requirements.txt
```

```bash
pip install -r requirements.txt
```
# cd /home/directpi/PET_Projects/Autopark && source .venv/bin/activate && .venv/bin/pytest -v -s
## Конфигурация

1. Все переменные окружения в корневом каталоге проекта в `.env`:
2. Заполните переменные в `.env`:
   - `PLATFORM_URL` - URL платформы (используется как базовый URL для API)
   - `API_DOCS_URL` - URL API документации (альтернативный вариант)
   - `BOT_URL` - URL бота (опционально)
   - `TEST_USER` - Имя/логин тестового пользователя
   - `TEST_USER_EMAIL` - Email тестового пользователя
   - `TEST_USER_PASSWORD` - Пароль тестового пользователя
   
   **Примечание:** `conftest.py` поддерживает альтернативные имена переменных для обратной совместимости.

## Запуск тестов

### Все тесты
```bash
pytest
```

### Только тесты User
```bash
pytest tests/api/test_user.py
```

### Только тесты Vehicle
```bash
pytest tests/api/test_vehicle.py
```

### Только Smoke
```bash
pytest -m smoke
```

### Нагрузочные тесты
```bash
# Запуск всех нагрузочных тестов (помечены маркером @pytest.mark.load)
pytest tests/api/test_load.py -m load -v -s

# Или через маркер напрямую
pytest -m load -v -s
```

**Примечание:** Нагрузочные тесты создают значительную нагрузку на API. Рекомендуется:
- Запускать на выделенном нагрузочном стенде (не на dev окружении)
- Запускать отдельно от обычных тестов
- Мониторить метрики сервера (CPU, Memory, Response Time) во время выполнения

### С детальным выводом
```bash
pytest -v -s
```

### Генерация HTML отчета
```bash
pytest --html=reports/report.html --self-contained-html
```

### Allure отчеты

**ВАЖНО:** Allure показывает только **ВЫПОЛНЕННЫЕ** тесты. 

#### Порядок обновления отчета:

**Шаг 1: Очистить старые результаты (рекомендуется)**
```bash
# Удалить старые результаты для чистого отчета
rm -rf tests/reports/allure-results/*
```

**Шаг 2: Запустить все тесты**
```bash
# Сбор артефактов (настроено в pytest.ini автоматически)
pytest
```
Результаты появятся в `tests/reports/allure-results`.

**Шаг 3: Сгенерировать отчет**
```bash
# Соберите отчёт в статическую папку
allure generate tests/reports/allure-results -o tests/reports/allure-report --clean
```

**Шаг 4: Запустить HTTP сервер**
```bash
# Поднимите локальный HTTP‑сервер над статикой
python3 -m http.server -d tests/reports/allure-report 8000
```

**Шаг 5: Открыть в браузере**
```
http://127.0.0.1:8000/
```

#### Альтернативный способ (локальный просмотр):
```bash
# Требуется установленный allure (CLI)
# Автоматически запускает сервер и открывает браузер
allure serve tests/reports/allure-results
```

Генерация статического отчёта в папку:
```bash
# 1) Соберите отчёт в статическую папку
allure generate tests/reports/allure-results -o tests/reports/allure-report --clean

# 2) Поднимите локальный HTTP‑сервер над статикой
python3 -m http.server -d tests/reports/allure-report 8000

# 3) Откройте в браузере
# http://127.0.0.1:8000/
```

## Структура

```
tests/
├── api/                    # API тесты
│   ├── test_auth.py       # Тесты авторизации
│   ├── test_user.py       # CRUD User, профиль
│   ├── test_vehicle.py    # CRUD Vehicle, коллекции
│   └── test_load.py       # Нагрузочные тесты (POST /auth/login, GET /user)
├── config/                # Конфиги и шаблон .env.example
│   ├── .env.example
│   └── ...
├── conftest.py            # Pytest фикстуры и хуки
└── requirements.txt       # Зависимости

pytest.ini (в КОРНЕ)
```

## Тестовые маркеры

- `@pytest.mark.smoke` — Smoke тест (критический бизнес-поток/валидный end‑to‑end)
- `@pytest.mark.api` — API тест (маркирует группы фич по типу)
- `@pytest.mark.integration` — интеграционные (например, с БД или другими сервисами)
- `@pytest.mark.slow` — долгие тесты, не входят в Smoke/CI базовый
- `@pytest.mark.load` — нагрузочные тесты (создают высокую нагрузку на API)

## Покрытие эндпоинтов (на сегодня)
- Auth: login, register, refresh, healthcheck, change-password (негативы), refresh-edges
- User: GET, POST, PATCH, DELETE, GET/PATCH me, user-role (GET/PATCH/DELETE), ownership edges
- Vehicle: GET, POST, PATCH, DELETE, коллекции, stats, disconnect-glonass, invalid-id, duplicate state_number

Актуальный чеклист — см. docs/api_endpoints_summary.md


## Очистка тестовых данных (Robust cleanup)

В тестах включён автоматический cleanup созданных записей через фикстуры из `conftest.py`:
- `user_cleanup(user_id)` — зарегистрировать созданного пользователя для удаления после теста
- `vehicle_cleanup(vehicle_id)` — зарегистрировать созданное ТС для удаления после теста

Пример использования:
```python
def test_create_then_delete_user(authenticated_session, user_cleanup):
    resp = authenticated_session.post(f"{authenticated_session.base_url}/api/v1/user", json={
        "name": "Tmp", "surname": "User", "phone": "+79990001122", "password": "TestPass123!"
    })
    assert resp.status_code == 201
    user_id = resp.json()["id"]
    user_cleanup(user_id)  # запись будет удалена в teardown
```

Это гарантирует чистоту окружения даже при падениях тестов.

