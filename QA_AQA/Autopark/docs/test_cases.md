### Новые автотесты (2025-10-30)
- Auth: refresh-access-token (некорректный Bearer) — Ожид.: 400/401/403 — Статус: Pass — Ссылка: `tests/api/test_auth.py::TestAuthRefreshTokenEdges::test_refresh_token_malformed_bearer`
- Auth: change-password (unauthorized) — Ожид.: 401/403 — Pass — `TestAuthChangePasswordNegatives::test_change_password_unauthorized`
- Auth: change-password (wrong current) — Ожид.: 400/401/403/422 — Pass — `...::test_change_password_wrong_current`
- Auth: change-password (empty new) — Ожид.: 422/400 — Pass — `...::test_change_password_empty_new`
- User-role GET без user_id — Ожид.: 200/400/403/422 — Pass — `tests/api/test_user.py::TestUserRole::test_get_roles_requires_user_id`
- User-role PATCH invalid enum — Ожид.: 400/403/422 — Pass — `...::test_set_role_invalid_enum`
- User-role PATCH valid/forbidden — Ожид.: 200/201/202/400/403/422 — Pass — `...::test_set_role_valid_or_forbidden`
- User-role DELETE by query — Ожид.: 400/403/404/422 — Pass — `...::test_delete_role_by_query`
- Ownership: PATCH foreign user — Ожид.: 200/202/403/404/422 — Pass — `...::TestOwnershipEdges::test_patch_foreign_user`
- Ownership: DELETE foreign user — Ожид.: 200/202/204/403/404/422 — Pass — `...::TestOwnershipEdges::test_delete_foreign_user`
- User: register empty email — Ожид.: 422/400 — Pass — `...::TestUserValidationMore::test_register_empty_email`
- User: register empty phone — Ожид.: 422/400 — Pass — `...::TestUserValidationMore::test_register_empty_phone`
- User: register max-length — Ожид.: 201/422/400 — Pass — `...::TestUserValidationMore::test_register_max_length_fields`
- User: register phone with letters — Ожид.: 422/400 — Fail (получен 201) — `...::TestUserValidationMore::test_register_phone_with_letters` — См. `#BUG-USER-PHONE-VALIDATION`
- User: register invalid email — Ожид.: 422/400 — Fail (получен 201) — `tests/api/test_user.py::TestUserValidationNegatives::test_register_invalid_email` — См. `#BUG-USER-EMAIL-VALIDATION`
 - User: register phone too short — Ожид.: 422/400 — TBD — `...::TestUserValidationMore::test_register_phone_too_short`
 - User: register minimal invalid email — Ожид.: 422/400 — TBD — `...::TestUserValidationMore::test_register_email_minimal_invalid`
- Vehicle: PATCH invalid id — Ожид.: 404/422/403 — Pass — `tests/api/test_vehicle.py::TestVehicleInvalidIds::test_patch_vehicle_invalid_id`
- Vehicle: DELETE invalid id — Ожид.: 404/422/403 — Pass — `...::test_delete_vehicle_invalid_id`
- Vehicle: duplicate state_number — Ожид.: 409/400/422 — Pass — `...::test_create_duplicate_state_number`
 - Vehicle: disconnect-glonass missing vehicle_id — Ожид.: 422/400/403 — TBD — `tests/api/test_vehicle.py::TestVehicleCollections::test_disconnect_glonass_missing_id`
# Тест-кейсы API (Auth / User / Vehicle)

| ID           | Название                                        | Preconditions                | Steps                                                                                           | Ожидаемый результат                 | Status | Метка     | Ссылка           |
|--------------|-------------------------------------------------|------------------------------|-------------------------------------------------------------------------------------------------|-------------------------------------|--------|-----------|------------------|
| TC-AUTH-001  | Login: успешная авторизация                     | USER создан, пароль валиден   | 1. POST /auth/login c верным USER/PASS                                                          | 202, вернулся access_token          | Pass   | Smoke     | test_auth.py     |
| TC-AUTH-002  | Login: неправильный USER/PASS                   |                              | 1. POST /auth/login c неправильным паролем                                                      | 401/403, нет токена                 | Pass   | Smoke     | test_auth.py     |
| TC-AUTH-003  | Register: успешная регистрация                  | уникальный email/phone       | 1. POST /auth/register, заполнить json                                                          | 201, пользователь создан            | Pass   | Smoke     | test_auth.py     |
| TC-AUTH-004  | Register: невалидный email                      | уникальный телефон           | 1. POST /auth/register с email="not-an-email"                                                 | 400/422, пользователь НЕ создан     | Fail*  | Regression| test_user.py     |
| TC-AUTH-005  | Register: дубликат email/phone                  | уже создан USER/email/phone   | 1. POST /auth/register с существующими email/phone                                              | 409/400, конфликт                   | Pass   | Edge      | test_user.py     |
| TC-AUTH-006  | Refresh token: валидный                         | получить refresh_token        | 1. POST /auth/refresh-access-token с токеном                                                    | 201, новый access_token             | Pass   | Smoke     | test_auth.py     |
| TC-AUTH-007  | Refresh token: пустой/невалидный                | без refresh_token в header    | 1. POST /auth/refresh-access-token без токена, ожидать ошибку                                   | 401/403, error                      | Pass   | Regression| test_auth.py     |
| TC-AUTH-008  | Healthcheck                                    |                              | 1. POST /auth/healthcheck                                                                        | 202/200                             | Pass   | Smoke     | test_auth.py     |
| TC-USER-001  | GET /user: неавторизованный                     |                              | 1. GET /user                                                                                   | 401/403 error                       | Pass   | Smoke     | test_user.py     |
| TC-USER-002  | GET /user: успешный (авторизован)               | валидный токен               | 1. GET /user c токеном                                                                           | 200                                 | Pass   | Smoke     | test_user.py     |
| TC-USER-003  | POST /user: невалидный payload                  | валидный токен               | 1. POST /user с неполным/bad payload                                                            | 400/422/403                         | Pass   | Regression| test_user.py     |
| TC-USER-004  | CRUD user: полный happy path                    | валидный токен               | 1. Создать user, 2. PATCH (name), 3. DELETE                                                     | Все шаги вернули успешный код       | Pass   | Smoke     | test_user.py     |
| TC-USER-005  | PATCH /user: invalid id                         | валидный токен               | 1. PATCH c id=99999999                                                                           | 404/422/403                         | Pass   | Edge      | test_user.py     |
| TC-USER-006  | PATCH /user: no auth                            | без токена                   | 1. PATCH /user без авторизации                                                                   | 401/403                             | Pass   | Edge      | test_user.py     |
| TC-USER-007  | PATCH /user: max длина name/surname             | валидный токен               | 1. PATCH /user с name/surname по 50 символов                                                     | 200/400/422                         | Pass   | Regression| test_user.py     |
| TC-USER-008  | DELETE /user: happy path                        | валидный токен, есть ID      | 1. POST+DELETE /user                                                                             | 204/200                             | Pass   | Smoke     | test_user.py     |
| TC-USER-009  | DELETE /user: невалидный id                     | валидный токен               | 1. DELETE user_id=99999998                                                                       | 404/422/403                         | Pass   | Edge      | test_user.py     |
| TC-USER-010  | DELETE /user: no auth                           | нет токена                   | 1. DELETE /user без авторизации                                                                  | 401/403                             | Pass   | Edge      | test_user.py     |
| TC-USER-011  | Негативные проверки email/phone                 | уникальный email/phone       | 1. POST invalid email, 2. POST invalid phone                                                     | 422/400                             | Fail*  | Regression| test_user.py     |
| TC-USER-012  | Дубликат email/phone                            | предварительно создать USER   | 1. POST с теми же email и phone                                                                  | 409/400 Conflict                    | Pass   | Edge      | test_user.py     |
| TC-VEH-001   | GET /vehicle: неавторизованный                  |                              | 1. GET /vehicle                                                                                 | 401/403                             | Pass   | Smoke     | test_vehicle.py  |
| TC-VEH-002   | GET /vehicle: авторизованный                    | валидный токен               | 1. GET /vehicle с токеном                                                                         | 200/403                             | Pass   | Smoke     | test_vehicle.py  |
| TC-VEH-003   | POST /vehicle: невалидный payload               | валидный токен               | 1. POST /vehicle неполный payload                                                                | 400/422/403                         | Pass   | Regression| test_vehicle.py  |
| TC-VEH-004   | PATCH /vehicle: обновить допустимые поля        | валидный токен, валидный id  | 1. PATCH Vehicle c id, brand и name (без смены номера/типа/статуса)                              | 200/202                             | Pass   | Regression| test_vehicle.py  |
| TC-VEH-005   | PATCH /vehicle: disconnect-glonass               | валидный id                  | 1. PATCH disconnect-glonass                                                                      | 200/202/400/404                     | Pass   | Edge      | test_vehicle.py  |
| TC-VEH-006   | POST/DELETE /vehicle: happy path                 | валидный токен, init данные  | 1. POST создать, 2. PATCH обновить, 3. DELETE                                                    | Все шаги с 2xx                      | Pass   | Smoke     | test_vehicle.py  |
| TC-VEH-007   | PATCH /vehicle: невалидный id                    | валидный токен               | PATCH /vehicle c id=9999999                                                                      | 404/422/403                         | Pass   | Edge      | test_vehicle.py  |
| TC-VEH-008   | PATCH /vehicle: нет токена                       | нет токена                   | PATCH /vehicle без авторизации                                                                   | 401/403                             | Pass   | Edge      | test_vehicle.py  |
| TC-VEH-009   | DELETE /vehicle: невалидный id                   | валидный токен               | DELETE /vehicle_id=9999999                                                                       | 404/422/403                         | Pass   | Edge      | test_vehicle.py  |

_*Fail/Fail* - зафиксирована текущая серверная логика: invalid email и phone может приводить к 201 (создание) или 409 (конфликт), ожидается 422/400. Проблема задокументирована в /docs

Примечание:
- Во всех тестах с созданием данных включён robust cleanup (фикстуры `user_cleanup`, `vehicle_cleanup`) — тестовые записи удаляются после тестов.

---

Список дополняется по мере расширения тестов. Все подробности по статусу в `/docs`.
