[К оглавлению](../../README.md)
# Тестируемый проект: Система управления автопарком
**Описание проекта:**
Комплексная система для автоматизации процессов управления автопарком транспортного предприятия, включающая **веб-интерфейс**, **API-приложение** и **телеграм-бот** для взаимодействия с пользователями.

## Тесты

Проект включает пять типов тестов: **API**, **Интеграционные** (API ↔ PostgreSQL, MinIO, GLONASS), **Нагрузочные**, **UI/E2E тесты с Page Object Model** и **Unit**.

### API Тесты

#### Описание
- Покрываются основные бизнес-сценарии: авторизация (login, register, refresh, healthcheck), полный CRUD для User и Vehicle, списочные и статистические коллекции, негативные сценарии (валидация и доступ).
- Тестируется **100% эндпоинтов из OpenAPI схемы** (105/105 методов) + **4 не задокументированных эндпоинта** (109 уникальных эндпоинтов в тестах) — для QA-инфраструктуры (см. docs/api_endpoints_summary.md).
- Покрытие включает: User (CRUD, роли, группы, платежи, зарплаты, компенсации), Vehicle (CRUD, типы, обслуживание, файлы), Driver (заказы, смены, файлы), Mechanic (заказы, файлы), Logist (заказы), Reports, Glonass, System, MinIO.

### Интеграционные тесты

#### Описание
- Проверка взаимодействия компонентов системы с внешними сервисами и БД.
- Покрытие включает: API ↔ PostgreSQL, API ↔ MinIO (файловое хранилище), API ↔ GLONASS (внешний сервис).
- Интеграционные тесты находятся в директории `tests/api/` и помечены маркером `@pytest.mark.integration`.
- Основные файлы: `test_glonass.py`, `test_minio_file.py`, `test_system.py`.

### Нагрузочные тесты

#### Описание
- Нефункциональное тестирование производительности и стабильности системы под нагрузкой.
- Проверка времени отклика критичных эндпоинтов (login, GET /user, GET /vehicle) при параллельных запросах.
- Метрики: среднее время отклика, перцентили (p50, p95, p99), количество успешных/неуспешных запросов.
- Нагрузочные тесты находятся в файле `tests/api/test_load.py` и помечены маркером `@pytest.mark.load`.
- **Важно:** Нагрузочные тесты исключены из обычного прогона (`-m "not load"` в pytest.ini) и запускаются отдельно.

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

### Только API тесты
```bash
pytest -m api
```

### Только UI тесты
```bash
pytest -m ui
# или
pytest tests/ui/
```

### Только Unit тесты
```bash
pytest -m unit
# или
pytest tests/unit/
```

### Только Smoke
```bash
pytest -m smoke
```

### Только E2E тесты
```bash
pytest -m e2e
```

### Только Regression тесты
```bash
pytest -m regression
```

### Только Интеграционные тесты
```bash
pytest -m integration
```

### Только Нагрузочные тесты
```bash
pytest -m load -v -s
# или
pytest tests/api/test_load.py -v -s
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

## Структура

```
tests/
│
├── api/                           # ============================================
│   │                              # 1. API ТЕСТЫ (функциональные)
│   │                              # ============================================
│   ├── test_auth.py              # Тесты авторизации
│   ├── test_user.py              # CRUD User, профиль
│   ├── test_user_role.py         # Роли пользователей
│   ├── test_user_group.py        # Группы пользователей
│   ├── test_user_payment.py      # Платежи пользователей
│   ├── test_user_salary.py       # Зарплаты пользователей
│   ├── test_user_compensation.py # Компенсации пользователей
│   ├── test_vehicle.py           # CRUD Vehicle, коллекции
│   ├── test_vehicle_type.py      # Типы транспортных средств
│   ├── test_vehicle_maintenance.py # Обслуживание ТС
│   ├── test_vehicle_file.py      # Файлы ТС
│   ├── test_driver_order.py      # Заказы водителей
│   ├── test_driver_order_file.py # Файлы заказов водителей
│   ├── test_driver_shift.py      # Смены водителей
│   ├── test_mechanic_order.py    # Заказы механиков
│   ├── test_mechanic_order_file.py # Файлы заказов механиков
│   ├── test_logist_order.py      # Заказы логистов
│   ├── test_report.py            # Отчёты
│   ├── test_report_file.py       # Файлы отчётов
│   ├── test_additional_coverage.py # Дополнительное покрытие
│   │
│   │                              # ============================================
│   │                              # 2. ИНТЕГРАЦИОННЫЕ ТЕСТЫ (@pytest.mark.integration)
│   │                              # ============================================
│   ├── test_glonass.py           # Интеграция с ГЛОНАСС
│   ├── test_system.py            # Системные эндпоинты
│   ├── test_minio_file.py        # Работа с MinIO
│   │
│   │                              # ============================================
│   │                              # 3. НАГРУЗОЧНЫЕ ТЕСТЫ (@pytest.mark.load)
│   │                              # ============================================
│   └── test_load.py              # Нагрузочные тесты
│
├── ui/                            # ============================================
│   │                              # 4. UI/E2E ТЕСТЫ (@pytest.mark.ui, @pytest.mark.e2e)
│   │                              # ============================================
│   ├── conftest.py               # Фикстуры для UI тестов (Selenium)
│   ├── pages/                    # Page Object Model
│   │   ├── login_page.py
│   │   ├── dashboard_page.py
│   │   ├── vehicle_list_page.py
│   │   ├── vehicle_form_page.py
│   │   ├── maintenance_list_page.py
│   │   ├── maintenance_form_page.py
│   │   └── reports_page.py
│   └── tests/                    # UI тесты
│       ├── test_smoke_login_dashboard.py
│       ├── test_smoke_simple.py
│       ├── test_e2e_vehicle_simple.py
│       ├── test_e2e_vehicle_maintenance_report.py
│       ├── test_regression_vehicle_crud.py
│       ├── test_regression_maintenance.py
│       ├── test_ui_navigation.py
│       ├── test_ui_logout.py
│       ├── test_check_creds.py
│       ├── test_debug_open.py
│       └── test_debug_vehicle_form.py
│
├── unit/                          # ============================================
│   │                              # 5. UNIT ТЕСТЫ (@pytest.mark.unit)
│   │                              # ============================================
│   ├── test_mapper_stub.py       # Тесты маппинга
│   ├── test_pricing_stub.py      # Тесты ценообразования
│   └── test_validation_stub.py   # Тесты валидации
├── config/                        # Конфиги и шаблон .env.example
│   └── .env.example
├── conftest.py                    # Pytest фикстуры и хуки (API)
└── requirements.txt               # Зависимости

pytest.ini (в КОРНЕ)
```

## Тестовые маркеры

- `@pytest.mark.unit` — быстрые модульные тесты/заглушки без внешних зависимостей
- `@pytest.mark.api` — интеграционные API-тесты
- `@pytest.mark.ui` — UI/E2E тесты через браузер
- `@pytest.mark.smoke` — быстрый дымовой прогон (критический бизнес-поток/валидный end‑to‑end)
- `@pytest.mark.regression` — регрессионный набор
- `@pytest.mark.slow` — тяжёлые или долгие тесты, не входят в Smoke/CI базовый
- `@pytest.mark.integration` — интеграционные сценарии (например, с БД или другими сервисами)
- `@pytest.mark.e2e` — сквозные сценарии
- `@pytest.mark.requires_auth` — тесты, требующие авторизации
- `@pytest.mark.load` — нагрузочные/ресурсоёмкие проверки (запускаются отдельно!)
- `@pytest.mark.security` — проверки безопасности

## Покрытие эндпоинтов (на сегодня)

### API
- **Auth**: login, register, refresh, healthcheck, change-password (негативы), refresh-edges
- **User**: GET, POST, PATCH, DELETE, GET/PATCH me, user-role (GET/PATCH/DELETE), ownership edges
- **User расширенное**: группы, платежи, зарплаты, компенсации
- **Vehicle**: GET, POST, PATCH, DELETE, коллекции, stats, disconnect-glonass, invalid-id, duplicate state_number
- **Vehicle расширенное**: типы, обслуживание, файлы
- **Driver**: заказы, смены, файлы заказов
- **Mechanic**: заказы, файлы заказов
- **Logist**: заказы
- **Reports**: отчёты и файлы отчётов
- **Glonass**: интеграция с ГЛОНАСС
- **System**: системные эндпоинты
- **MinIO**: работа с файловым хранилищем

### Нагрузочные
- Критичные эндпоинты: POST /auth/login, GET /user, GET /vehicle
- Метрики производительности: среднее время отклика, перцентили (p50, p95, p99)
- Параллельные запросы для проверки стабильности под нагрузкой

### UI/E2E
- Авторизация и выход
- Навигация по интерфейсу
- CRUD операции с транспортными средствами
- Обслуживание транспортных средств
- Отчёты
- Регрессионные проверки

### Unit
- Маппинг данных
- Ценообразование
- Валидация

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

