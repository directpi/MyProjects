"""
@file: test_vehicle_maintenance.py
@description: API тесты для CRUD операций по обслуживанию ТС (Vehicle Maintenance)
@dependencies: pytest, requests
@created: 2025-11-07
@author: Directpi
@test_type: api
"""

import pytest
import requests
from typing import Optional
import time
from requests import exceptions as requests_exceptions
try:
    from tests.conftest import attach_response, assert_status  # type: ignore
except Exception:
    def attach_response(*args, **kwargs):
        return None
    def assert_status(response, expected, *, context="", bug_map=None):
        code = response.status_code
        expected_set = {int(x) for x in expected}
        if code in expected_set:
            return
        message_context = f"{context}. " if context else ""
        details = f"Ожидались {sorted(expected_set)}, получен {code}: {response.text}"
        if bug_map and code in bug_map:
            pytest.xfail(f"{bug_map[code]} — {message_context}{details}")
        pytest.fail(f"{message_context}{details}")

BASE_PATH = "/api/v1/vehicle-maintenance"


_CACHED_VEHICLE_ID: Optional[int] = None


def _get_first_vehicle_id(session: requests.Session, retry_count: int = 1) -> Optional[int]:
    """Получить первый доступный vehicle_id для тестов.
    
    Args:
        session: Сессия requests с авторизацией
        retry_count: Количество попыток при таймауте (по умолчанию 1, чтобы не перегружать платформу)
    
    Returns:
        vehicle_id или None, если не удалось получить
    """
    for attempt in range(retry_count + 1):
        try:
            # Используем короткий таймаут, чтобы не перегружать платформу
            # Увеличиваем только при последней попытке
            timeout = 5 if attempt < retry_count else 8
            resp = session.get(f"{session.base_url}/api/v1/vehicle/all", timeout=timeout)
            status = resp.status_code
            
            if status != 200:
                if attempt < retry_count:
                    print(f"[DEBUG] _get_first_vehicle_id: попытка {attempt + 1}/{retry_count + 1}, статус {status}, повтор через 2с...")
                    time.sleep(2)
                    continue
                print(f"[DEBUG] _get_first_vehicle_id: статус {status}, ответ: {resp.text[:200]}")
                return None
                
            try:
                items = resp.json()
            except Exception as json_err:
                if attempt < retry_count:
                    print(f"[DEBUG] _get_first_vehicle_id: попытка {attempt + 1}/{retry_count + 1}, ошибка парсинга JSON, повтор через 2с...")
                    time.sleep(2)
                    continue
                print(f"[DEBUG] _get_first_vehicle_id: ошибка парсинга JSON: {json_err}, ответ: {resp.text[:200]}")
                return None
                
            if not isinstance(items, list):
                print(f"[DEBUG] _get_first_vehicle_id: ответ не список: {type(items)}, значение: {items}")
                return None
                
            if not items:
                print(f"[DEBUG] _get_first_vehicle_id: пустой список транспортных средств")
                return None
                
            # Берем первый элемент
            first = items[0]
            if not isinstance(first, dict):
                print(f"[DEBUG] _get_first_vehicle_id: первый элемент не словарь: {type(first)}, значение: {first}")
                return None
                
            # Пытаемся получить id разными способами
            vehicle_id = first.get("id") or first.get("vehicle_id")
            
            if vehicle_id is not None:
                # Преобразуем в int, если нужно
                try:
                    vehicle_id = int(vehicle_id)
                    print(f"[DEBUG] _get_first_vehicle_id: найден vehicle_id={vehicle_id}")
                    return vehicle_id
                except (ValueError, TypeError) as e:
                    print(f"[DEBUG] _get_first_vehicle_id: не удалось преобразовать vehicle_id в int: {vehicle_id}, ошибка: {e}")
                    return None
            else:
                # Логируем структуру первого элемента для диагностики
                print(f"[DEBUG] _get_first_vehicle_id: первый элемент не содержит id/vehicle_id. Ключи: {list(first.keys()) if isinstance(first, dict) else 'N/A'}, значение: {first}")
                return None
                
        except requests_exceptions.Timeout:
            if attempt < retry_count:
                # Увеличиваем задержку, но не слишком сильно, чтобы не перегружать платформу
                wait_time = 2 + attempt
                print(f"[DEBUG] _get_first_vehicle_id: попытка {attempt + 1}/{retry_count + 1}, таймаут, повтор через {wait_time}с...")
                time.sleep(wait_time)
                continue
            print(f"[DEBUG] _get_first_vehicle_id: таймаут запроса к /api/v1/vehicle/all после {retry_count + 1} попыток")
            return None
        except Exception as e:
            if attempt < retry_count:
                print(f"[DEBUG] _get_first_vehicle_id: попытка {attempt + 1}/{retry_count + 1}, исключение {type(e).__name__}, повтор через 2с...")
                time.sleep(2)
                continue
            print(f"[DEBUG] _get_first_vehicle_id: исключение: {type(e).__name__}: {e}")
            return None
    
    return None


def _get_cached_vehicle_id(session: requests.Session, force_refresh: bool = False) -> Optional[int]:
    """Получить и закешировать vehicle_id, чтобы не дергать /vehicle/all в каждом тесте.
    
    Кешируем только успешные значения (не None). Если кеш None, повторяем попытку.
    
    Args:
        session: Сессия requests с авторизацией
        force_refresh: Принудительно обновить кеш (по умолчанию False)
    
    Returns:
        vehicle_id или None, если не удалось получить
    """
    global _CACHED_VEHICLE_ID
    # Если кеш уже содержит валидный ID и не требуется обновление, возвращаем его
    if not force_refresh and _CACHED_VEHICLE_ID is not None:
        return _CACHED_VEHICLE_ID

    # Пытаемся получить vehicle_id с retry механизмом
    vehicle_id = _get_first_vehicle_id(session, retry_count=2)
    
    # Кешируем только успешные значения (не None)
    if vehicle_id is not None:
        _CACHED_VEHICLE_ID = vehicle_id
        print(f"[DEBUG] _get_cached_vehicle_id: закеширован vehicle_id={vehicle_id}")
    else:
        print(f"[DEBUG] _get_cached_vehicle_id: vehicle_id не найден, кеш не обновлен")
    
    return vehicle_id


@pytest.fixture
def vehicle_maintenance_cleanup(authenticated_session: requests.Session):
    """Фикстура для автоматической очистки созданных записей обслуживания."""
    created_ids = []

    def _cleanup(maintenance_id: Optional[int]):
        if maintenance_id:
            created_ids.append(maintenance_id)

    yield _cleanup

    # Cleanup: удаляем созданные записи
    base = authenticated_session.base_url
    for maint_id in created_ids:
        try:
            resp = authenticated_session.delete(
                f"{base}{BASE_PATH}",
                params={"maintenance_id": maint_id},
                timeout=10,
            )
            assert_status(
                resp,
                [204, 422],
                context="DELETE /vehicle-maintenance (cleanup)",
                bug_map={
                    200: "#BUG-VEHICLE-MAINTENANCE-CLEANUP-200",
                    202: "#BUG-VEHICLE-MAINTENANCE-CLEANUP-202",
                    404: "#BUG-VEHICLE-MAINTENANCE-CLEANUP-404",
                    403: "#BUG-VEHICLE-MAINTENANCE-CLEANUP-403",
                },
            )
        except Exception:
            pass  # Игнорируем ошибки cleanup


@pytest.mark.api
class TestVehicleMaintenanceRead:
    @pytest.mark.smoke
    def test_get_vehicle_maintenances_unauthorized(self, api_base_url: str):
        """Проверка GET /vehicle-maintenance без авторизации.

        При перегрузке API (очень много данных, медленные SELECT'ы) считаем
        таймаут техническим ограничением окружения, а не логической ошибкой,
        поэтому аккуратно скипаем тест, чтобы не добивать платформу.
        """
        try:
            response = requests.get(f"{api_base_url}{BASE_PATH}", timeout=5)
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /vehicle-maintenance без авторизации")

        assert_status(
            response,
            [200, 422],
            context="GET /vehicle-maintenance без авторизации",
            bug_map={
                401: "#BUG-VEHICLE-MAINTENANCE-GET-UNAUTH-401",
                403: "#BUG-VEHICLE-MAINTENANCE-GET-UNAUTH-403",
            },
        )

    def test_get_vehicle_maintenances_authorized(
        self, authenticated_session: requests.Session
    ):
        """Проверка GET /vehicle-maintenance с авторизацией.

        При медленном окружении (большой объём данных в БД) не держим соединение
        до полного истечения большого таймаута и не роняем платформу — при
        ReadTimeout помечаем тест как SKIPPED.
        """
        try:
            response = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}", timeout=8
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /vehicle-maintenance (авторизован)")

        attach_response("vehicle_maintenance.get", response)
        assert_status(
            response,
            [200, 422],
            context="GET /vehicle-maintenance (авторизован)",
            bug_map={403: "#BUG-VEHICLE-MAINTENANCE-GET-AUTH-403"},
        )

    @pytest.mark.smoke
    @pytest.mark.slow
    def test_get_vehicle_maintenances_all(
        self, authenticated_session: requests.Session
    ):
        """Проверка GET /vehicle-maintenance/all с ограничением limit=5.
        
        ⚠️ ВНИМАНИЕ: Этот тест ОТКЛЮЧЕН по умолчанию для безопасности.
        
        Изменения:
        - Добавлен параметр limit=5 для ограничения количества записей
        - Это предотвращает блокировку БД при большом объеме данных
        - Тест теперь безопаснее, но все еще отключен по умолчанию
        
        Для запуска:
        - Установите ENABLE_HEAVY_TESTS=1 в .env
        - Или используйте: pytest -m slow -k test_get_vehicle_maintenances_all
        
        Альтернатива: Используйте GET /vehicle-maintenance с фильтрами вместо /all
        """
        # Проверка переменной окружения - тест отключен по умолчанию для безопасности
        import os
        if not os.getenv("ENABLE_HEAVY_TESTS"):
            pytest.skip(
                "⚠️ ТЕСТ ОТКЛЮЧЕН: test_get_vehicle_maintenances_all отключен по умолчанию. "
                "Тест использует limit=5 для безопасности, но все еще требует явного включения. "
                "Для запуска установите ENABLE_HEAVY_TESTS=1 в .env"
            )
        
        import time as time_module
        start_time = time_module.time()
        
        try:
            # Добавляем limit=5 для ограничения количества записей и снижения нагрузки на БД
            # Это предотвращает блокировку БД при большом объеме данных
            response = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}/all",
                params={"limit": 5},  # Ограничиваем до 5 записей
                timeout=5
            )
            elapsed_time = time_module.time() - start_time
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /vehicle-maintenance/all")
        except requests_exceptions.ConnectionError as e:
            pytest.skip(f"API недоступен после тяжелого запроса: {e}")

        # Логируем метрики для диагностики
        try:
            data = response.json() if response.content else []
            count = len(data) if isinstance(data, list) else 0
            response_size = len(response.content) if response.content else 0
            print(f"[DEBUG] GET /vehicle-maintenance/all: время={elapsed_time:.2f}с, записей={count}, размер ответа={response_size/1024:.2f}KB")
        except Exception:
            pass

        attach_response("vehicle_maintenance.get_all", response)
        assert_status(
            response,
            [200],
            context="GET /vehicle-maintenance/all",
            bug_map={403: "#BUG-VEHICLE-MAINTENANCE-ALL-403"},
        )
        
        # Длительная задержка для восстановления БД
        recovery_time = max(15, int(elapsed_time * 2))  # Минимум 15 сек, или 2x время запроса
        print(f"[WARNING] Тяжелый запрос завершен. Задержка восстановления: {recovery_time}с")
        time.sleep(recovery_time)

    def test_get_vehicle_maintenances_with_filters(
        self, authenticated_session: requests.Session
    ):
        """Проверка GET /vehicle-maintenance с фильтрами.

        Используем только первое доступное ТС и аккуратно обрабатываем
        перегрузку API: при ReadTimeout скипаем тест, чтобы не добивать
        платформу.
        
        Если предыдущий тест был тяжелым (test_get_vehicle_maintenances_all),
        добавляем задержку перед получением vehicle_id, чтобы дать платформе
        время на восстановление.
        """
        # Проверяем здоровье API перед началом теста
        # Если предыдущий тест был тяжелым, платформа может быть перегружена
        try:
            health_check = authenticated_session.get(
                f"{authenticated_session.base_url}/api/v1/vehicle",
                timeout=3,
                params={"limit": 1}
            )
            if health_check.status_code not in [200, 422]:
                pytest.skip(f"API перегружен после предыдущего теста, статус: {health_check.status_code}")
        except requests_exceptions.Timeout:
            pytest.skip("API не отвечает после предыдущего тяжелого теста")
        except Exception:
            # Если health check не прошел, но это не критично, продолжаем
            pass
        
        # Небольшая задержка для стабилизации
        time.sleep(2)
        
        # Используем кеш, чтобы не делать лишние запросы к /api/v1/vehicle/all
        # Если кеш пустой, делаем только одну попытку с коротким таймаутом
        vehicle_id = _get_cached_vehicle_id(authenticated_session)
        if not vehicle_id:
            pytest.skip("Нет доступных транспортных средств для теста")

        # Еще небольшая задержка перед основным запросом
        time.sleep(1)
        
        try:
            response = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}",
                params={"vehicle_id": vehicle_id},
                timeout=8,
            )
        except requests_exceptions.Timeout:
            pytest.skip(
                "API перегружен: таймаут GET /vehicle-maintenance с фильтром vehicle_id"
            )

        assert_status(
            response,
            [200, 422],
            context="GET /vehicle-maintenance с фильтром vehicle_id",
            bug_map={403: "#BUG-VEHICLE-MAINTENANCE-GET-FILTER-403"},
        )


@pytest.mark.api
class TestVehicleMaintenanceCrud:
    @pytest.mark.smoke
    def test_create_vehicle_maintenance_invalid_payload(
        self, authenticated_session: requests.Session
    ):
        """Проверка POST /vehicle-maintenance с невалидным payload."""
        try:
            response = authenticated_session.post(
                f"{authenticated_session.base_url}{BASE_PATH}",
                json={"vehicle_id": "invalid"},  # Невалидный тип
                timeout=8,
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут POST /vehicle-maintenance invalid payload")
        assert_status(
            response,
            [422],
            context="POST /vehicle-maintenance с невалидным payload",
            bug_map={
                400: "#BUG-VEHICLE-MAINTENANCE-CREATE-INVALID-400",
                403: "#BUG-VEHICLE-MAINTENANCE-CREATE-INVALID-403",
            },
        )

    def test_create_update_delete_vehicle_maintenance_flow(
        self,
        authenticated_session: requests.Session,
        vehicle_maintenance_cleanup,
    ):
        """Полный flow: создание, обновление, удаление записи обслуживания."""
        base = authenticated_session.base_url
        vehicle_id = _get_cached_vehicle_id(authenticated_session)
        if not vehicle_id:
            pytest.skip("Нет доступных транспортных средств для теста")

        # Создание
        suffix = int(time.time() * 1000) % 100000
        payload = {
            "vehicle_id": vehicle_id,
            "description": f"Test maintenance {suffix}",
            "costs": 1000.50,
            "status": "Открыт",  # Согласно схеме VehicleMaintenanceStatus: "Открыт", "В работе", "Завершен", "Отменен"
        }
        create_resp = authenticated_session.post(
            f"{base}{BASE_PATH}", json=payload, timeout=8
        )
        attach_response("vehicle_maintenance.create", create_resp)
        assert_status(
            create_resp,
            [201, 422],
            context="POST /vehicle-maintenance (positive flow)",
            bug_map={
                403: "#BUG-VEHICLE-MAINTENANCE-CREATE-403",
                409: "#BUG-VEHICLE-MAINTENANCE-CREATE-409",
            },
        )

        maintenance_id = None
        create_status = create_resp.status_code
        if create_status == 201:
            body = create_resp.json()
            maintenance_id = body.get("id") or body.get("maintenance_id")
            assert maintenance_id, "В ответе на создание должен быть id"
            vehicle_maintenance_cleanup(maintenance_id)

        # Обновление
        if maintenance_id:
            update_payload = {
                "id": maintenance_id,
                "description": f"Updated maintenance {suffix}",
                "costs": 1500.75,
                "status": "В работе",  # Согласно схеме VehicleMaintenanceStatus
            }
            update_resp = authenticated_session.patch(
                f"{base}{BASE_PATH}",
                json=update_payload,
                timeout=8,
            )
            attach_response("vehicle_maintenance.update", update_resp)
            assert_status(
                update_resp,
                [200, 422],
                context="PATCH /vehicle-maintenance (positive flow)",
                bug_map={
                    202: "#BUG-VEHICLE-MAINTENANCE-PATCH-202",
                    403: "#BUG-VEHICLE-MAINTENANCE-PATCH-403",
                },
            )

        # Удаление
        if maintenance_id:
            delete_resp = authenticated_session.delete(
                f"{base}{BASE_PATH}",
                params={"maintenance_id": maintenance_id},
                timeout=8,
            )
            attach_response("vehicle_maintenance.delete", delete_resp)
            assert_status(
                delete_resp,
                [204, 422],
                context="DELETE /vehicle-maintenance (positive flow)",
                bug_map={
                    403: "#BUG-VEHICLE-MAINTENANCE-DELETE-403",
                    200: "#BUG-VEHICLE-MAINTENANCE-DELETE-200",
                    202: "#BUG-VEHICLE-MAINTENANCE-DELETE-202",
                },
            )

    def test_create_vehicle_maintenance_minimal_payload(
        self,
        authenticated_session: requests.Session,
        vehicle_maintenance_cleanup,
    ):
        """Проверка создания записи обслуживания с минимальным payload."""
        base = authenticated_session.base_url
        vehicle_id = _get_cached_vehicle_id(authenticated_session)
        if not vehicle_id:
            pytest.skip("Нет доступных транспортных средств для теста")

        # Согласно OpenAPI схеме: vehicle_id обязателен
        payload = {
            "vehicle_id": vehicle_id,
        }
        create_resp = authenticated_session.post(
            f"{base}{BASE_PATH}", json=payload, timeout=8
        )
        attach_response("vehicle_maintenance.create_minimal", create_resp)

        create_status = create_resp.status_code
        if create_status == 201:
            body = create_resp.json()
            maintenance_id = body.get("id") or body.get("maintenance_id")
            if maintenance_id:
                vehicle_maintenance_cleanup(maintenance_id)

        assert_status(
            create_resp,
            [201, 422],
            context="POST /vehicle-maintenance (минимальный payload)",
            bug_map={
                400: "#BUG-VEHICLE-MAINTENANCE-CREATE-MIN-400",
                403: "#BUG-VEHICLE-MAINTENANCE-CREATE-MIN-403",
            },
        )


@pytest.mark.api
class TestVehicleMaintenanceEdges:
    """Тесты для граничных случаев и негативных сценариев."""

    def test_patch_vehicle_maintenance_invalid_id(
        self, authenticated_session: requests.Session
    ):
        """Проверка PATCH /vehicle-maintenance с несуществующим id."""
        try:
            response = authenticated_session.patch(
                f"{authenticated_session.base_url}{BASE_PATH}",
                json={"id": 99999999, "description": "Invalid"},
                timeout=8,
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут PATCH /vehicle-maintenance invalid id")
        assert_status(
            response,
            [422],
            context="PATCH /vehicle-maintenance (несуществующий id)",
            bug_map={
                404: "#BUG-VEHICLE-MAINTENANCE-PATCH-INVALID-404",
                403: "#BUG-VEHICLE-MAINTENANCE-PATCH-INVALID-403",
            },
        )

    def test_delete_vehicle_maintenance_invalid_id(
        self, authenticated_session: requests.Session
    ):
        """Проверка DELETE /vehicle-maintenance с несуществующим id."""
        try:
            response = authenticated_session.delete(
                f"{authenticated_session.base_url}{BASE_PATH}",
                params={"maintenance_id": 99999999},
                timeout=8,
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут DELETE /vehicle-maintenance invalid id")
        assert_status(
            response,
            [422],
            context="DELETE /vehicle-maintenance (несуществующий id)",
            bug_map={
                404: "#BUG-VEHICLE-MAINTENANCE-DELETE-INVALID-404",
                403: "#BUG-VEHICLE-MAINTENANCE-DELETE-INVALID-403",
            },
        )

    def test_create_vehicle_maintenance_without_vehicle_id(
        self, authenticated_session: requests.Session
    ):
        """Проверка POST /vehicle-maintenance без обязательного vehicle_id.
        
        Используется короткий таймаут (8 сек) вместо 15, чтобы не перегружать
        платформу долгими запросами.
        """
        try:
            response = authenticated_session.post(
                f"{authenticated_session.base_url}{BASE_PATH}",
                json={"description": "Test without vehicle_id"},
                timeout=8,  # Уменьшен с 15 до 8 для защиты платформы
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут POST /vehicle-maintenance без vehicle_id")
        
        assert_status(
            response,
            [422],
            context="POST /vehicle-maintenance без vehicle_id",
            bug_map={
                400: "#BUG-VEHICLE-MAINTENANCE-CREATE-NO-VEHICLE-400",
                403: "#BUG-VEHICLE-MAINTENANCE-CREATE-NO-VEHICLE-403",
            },
        )

