[К оглавлению](../../README.md)
# Тестируемый проект: Система управления автопарком
**Описание проекта:**
Комплексная система для автоматизации процессов управления автопарком транспортного предприятия, включающая **веб-интерфейс**, **API-приложение** и **телеграм-бот** для взаимодействия с пользователями.

## API Тесты

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

## Конфигурация

1. Все переменные окружения в корневом каталоге проекта в `.env`:
2. Заполните переменные в `.env`:
   - `TEST_USER`, `TEST_PASSWORD`, `API_BASE_URL` и т.д.

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

### С детальным выводом
```bash
pytest -v -s
```

### Генерация HTML отчета
```bash
pytest --html=reports/report.html --self-contained-html
```

## Структура

```
tests/
├── api/                    # API тесты
│   ├── test_auth.py       # Тесты авторизации
│   ├── test_user.py       # CRUD User, профиль
│   └── test_vehicle.py    # CRUD Vehicle, коллекции
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

