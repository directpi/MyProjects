# Сводка API Эндпоинтов

Документ создан на основе анализа OpenAPI схемы.

## Общая статистика
- **Всего эндпоинтов:** 105 методов (65 путей)
- **Покрытие тестами:** 100% (105/105) + 4 не задокументированных эндпоинта
- **Базовая версия API:** `/api/v1`
- **Технология:** FastAPI

## Модули API

### 1. Authentication (`/api/v1/auth`)
- `POST /api/v1/auth/login` - Авторизация (form-data)
- `POST /api/v1/auth/register` - Регистрация (JSON)
- `POST /api/v1/auth/refresh-access-token` - Обновление токена (требует авторизацию)
- `POST /api/v1/auth/change-password` - Смена пароля (требует авторизацию)
- `GET /api/v1/auth/temporary-token` - Получение временного токена
- `POST /api/v1/auth/temporary-token` - Валидация временного токена
- `POST /api/v1/auth/healthcheck` - Проверка здоровья сервиса

**Статус тестирования:** ✅ Полное покрытие (login/register/refresh/healthcheck/change-password)

### 2. User Management (`/api/v1/user`)
- `GET /api/v1/user/me` - Информация о текущем пользователе
- `PATCH /api/v1/user/me` - Обновление профиля
- `GET /api/v1/user` - Список пользователей
- `POST /api/v1/user` - Создание пользователя
- `PATCH /api/v1/user` - Обновление пользователя
- `DELETE /api/v1/user` - Удаление пользователя
- `GET /api/v1/user/all` - Все пользователи
- `GET /api/v1/user/stats/count` - Статистика по количеству

**Статус тестирования:** ✅ Полное покрытие (CRUD, /me, /all, /stats, роли, группы, платежи, зарплаты, компенсации)

### 3. User Compensation (`/api/v1/user-compensation`)
- `GET /api/v1/user-compensation`
- `POST /api/v1/user-compensation`
- `DELETE /api/v1/user-compensation`

**Статус тестирования:** ✅ Полное покрытие

### 4. User Payment (`/api/v1/user-payment`)
- `GET /api/v1/user-payment`
- `POST /api/v1/user-payment`
- `PATCH /api/v1/user-payment`
- `DELETE /api/v1/user-payment`
- `GET /api/v1/user-payment/all`
- `GET /api/v1/user-payment/excel/all` - Экспорт в Excel

**Статус тестирования:** ✅ Полное покрытие

### 5. User Salary (`/api/v1/user-salary`)
- `GET /api/v1/user-salary/my` - Моя зарплата
- `GET /api/v1/user-salary`
- `GET /api/v1/user-salary/all`
- `GET /api/v1/user-salary/drivers/excel` - Экспорт зарплат водителей

**Статус тестирования:** ✅ Полное покрытие

### 6. User Role & Group
- `/api/v1/user-role` - CRUD операции
- `/api/v1/user-group` - CRUD операции

**Статус тестирования:** ✅ Полное покрытие

### 7. Vehicle Management (`/api/v1/vehicle`)
- `GET /api/v1/vehicle` - Список транспорта
- `POST /api/v1/vehicle` - Создание
- `PATCH /api/v1/vehicle` - Обновление
- `DELETE /api/v1/vehicle` - Удаление
- `GET /api/v1/vehicle/all` - Все транспортные средства
- `GET /api/v1/vehicle/stats/current-operation` - Статистика
- `PATCH /api/v1/vehicle/disconnect-glonass` - Отключение ГЛОНАСС

**Статус тестирования:** ✅ Полное покрытие (CRUD, /all, /stats, disconnect-glonass, типы, обслуживание, файлы)

### 8. Vehicle Type (`/api/v1/vehicle-type`)
- CRUD операции + `GET /api/v1/vehicle-type/all`

**Статус тестирования:** ✅ Полное покрытие

### 9. Vehicle Maintenance (`/api/v1/vehicle-maintenance`)
- CRUD операции + `GET /api/v1/vehicle-maintenance/all`

**Статус тестирования:** ✅ Полное покрытие

### 10. Vehicle Files (`/api/v1/vehicle-file`)
- `POST /api/v1/vehicle-file/image` - Загрузка изображения
- `POST /api/v1/vehicle-file/documents` - Загрузка документов
- `POST /api/v1/vehicle-file/maintenance-image` - Изображение ТО
- `POST /api/v1/vehicle-file/maintenance-documents` - Документы ТО

**Статус тестирования:** ✅ Полное покрытие

### 11. GLONASS Integration (`/api/v1/glonass`)
- `GET /api/v1/glonass/vehicles` - Список транспорта
- `GET /api/v1/glonass/vehicle-mileage` - Пробег
- `GET /api/v1/glonass/vehicle-mileage/excel` - Экспорт пробега
- `GET /api/v1/glonass/user` - Пользователь GLONASS

**Статус тестирования:** ✅ Полное покрытие (интеграционные тесты)

### 12. Orders

#### Logist Order (`/api/v1/logist-order`)
- CRUD операции
- `GET /api/v1/logist-order/all`
- Статистика: `/stats/all`, `/stats/status/count`, `/stats/excel/all`

**Статус тестирования:** ✅ Полное покрытие

#### Driver Order (`/api/v1/driver-order`)
- CRUD операции
- `GET /api/v1/driver-order/all`
- Файлы: `/driver-order-file/image-of-load`, `/image-of-unload`, `/documents`

**Статус тестирования:** ✅ Полное покрытие

#### Driver Shift (`/api/v1/driver-shift`)
- `GET /api/v1/driver-shift/my-active` - Активная смена
- CRUD операции + `/all`

**Статус тестирования:** ✅ Полное покрытие

#### Mechanic Order (`/api/v1/mechanic-order`)
- CRUD операции + `/all`
- Файлы: `/mechanic-order-file/image`

**Статус тестирования:** ✅ Полное покрытие

### 13. Reports (`/api/v1/report`)
- CRUD операции
- `GET /api/v1/report/logist/my` - Мои отчеты (логист)
- Статистика: `/stats/status/count`, `/stats/status/count/logist`
- `GET /api/v1/report/all`
- Файлы: `/report-file/image`, `/report-file/documents`

**Статус тестирования:** ✅ Полное покрытие

### 14. Minio Files (`/api/v1/minio-file`)
- `GET /api/v1/minio-file/{minio_file_name}` - Получение файла
- `DELETE /api/v1/minio-file/{minio_file_name}` - Удаление файла

**Статус тестирования:** ✅ Полное покрытие (интеграционные тесты)

### 15. System (`/api/v1/system`)
- Системные эндпоинты для управления интеграциями

**Статус тестирования:** ✅ Полное покрытие (интеграционные тесты)

## Статус покрытия

Все эндпоинты из OpenAPI схемы (105 методов) покрыты тестами на 100%.

### Типы тестов
- **API тесты** - функциональное тестирование всех эндпоинтов
- **Интеграционные тесты** - проверка взаимодействия с внешними сервисами (GLONASS, MinIO, System)
- **Нагрузочные тесты** - проверка производительности критичных эндпоинтов (login, GET /user, GET /vehicle)

### Дополнительно
- 4 не задокументированных эндпоинта также покрыты тестами (например, `PATCH /api/v1/vehicle/disconnect-glonass`)

