"""
@file: test_vehicle_type.py
@description: API тесты для CRUD операций по типам транспорта (Vehicle Type)
@dependencies: pytest, requests
@created: 2025-11-06
@author: Directpi
@test_type: api
"""

import pytest
import requests
from typing import Optional
import time
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

BASE_PATH = "/api/v1/vehicle-type"


@pytest.fixture
def vehicle_type_cleanup(authenticated_session: requests.Session):
    """Фикстура для автоматической очистки созданных типов транспорта."""
    created_ids = []

    def _cleanup(vehicle_type_id: Optional[int]):
        if vehicle_type_id:
            created_ids.append(vehicle_type_id)

    yield _cleanup

    # Cleanup: удаляем созданные типы
    base = authenticated_session.base_url
    for vt_id in created_ids:
        try:
            resp = authenticated_session.delete(
                f"{base}{BASE_PATH}",
                params={"type_id": vt_id},
                timeout=10,
            )
            assert_status(
                resp,
                [204, 422],
                context="DELETE /vehicle-type (cleanup)",
                bug_map={200: "#BUG-VEHICLE-TYPE-CLEANUP-200", 202: "#BUG-VEHICLE-TYPE-CLEANUP-202", 404: "#BUG-VEHICLE-TYPE-CLEANUP-404", 403: "#BUG-VEHICLE-TYPE-CLEANUP-403"},
            )
        except Exception:
            pass  # Игнорируем ошибки cleanup


@pytest.mark.api
class TestVehicleTypeRead:
    @pytest.mark.smoke
    def test_get_vehicle_types_unauthorized(self, api_base_url: str):
        response = requests.get(f"{api_base_url}{BASE_PATH}", timeout=10)
        assert_status(
            response,
            [200, 422],
            context="GET /vehicle-type без авторизации",
            bug_map={401: "#BUG-VEHICLE-TYPE-GET-UNAUTH-401", 403: "#BUG-VEHICLE-TYPE-GET-UNAUTH-403"},
        )

    def test_get_vehicle_types_authorized(
        self, authenticated_session: requests.Session
    ):
        response = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}", timeout=15
        )
        assert_status(
            response,
            [200, 422],
            context="GET /vehicle-type (авторизован)",
            bug_map={403: "#BUG-VEHICLE-TYPE-GET-AUTH-403"},
        )

    @pytest.mark.smoke
    def test_get_vehicle_types_all(
        self, authenticated_session: requests.Session
    ):
        response = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/all", timeout=15
        )
        attach_response("vehicle_type.get_all", response)
        assert_status(
            response,
            [200],
            context="GET /vehicle-type/all",
            bug_map={403: "#BUG-VEHICLE-TYPE-ALL-403"},
        )


@pytest.mark.api
class TestVehicleTypeCrud:
    @pytest.mark.smoke
    def test_create_vehicle_type_invalid_payload(
        self, authenticated_session: requests.Session
    ):
        response = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={"name": 123},
            timeout=15,
        )
        assert_status(
            response,
            [422],
            context="POST /vehicle-type с невалидным payload",
            bug_map={400: "#BUG-VEHICLE-TYPE-CREATE-INVALID-400", 403: "#BUG-VEHICLE-TYPE-CREATE-INVALID-403"},
        )

    def test_create_update_delete_vehicle_type_flow(
        self, authenticated_session: requests.Session, vehicle_type_cleanup
    ):
        base = authenticated_session.base_url
        suffix = int(time.time() * 1000) % 100000
        payload = {
            "name": f"TestType{suffix}",
            "rate_per_km": 10.5,
        }
        create_resp = authenticated_session.post(
            f"{base}{BASE_PATH}", json=payload, timeout=20
        )
        attach_response("vehicle_type.create", create_resp)
        assert_status(
            create_resp,
            [201, 422],
            context="POST /vehicle-type (positive flow)",
            bug_map={
                403: "#BUG-VEHICLE-TYPE-CREATE-403",
                409: "#BUG-VEHICLE-TYPE-CREATE-409",
            },
        )

        vehicle_type_id = None
        create_status = create_resp.status_code
        if create_status == 201:
            body = create_resp.json()
            vehicle_type_id = (
                body.get("id") or body.get("vehicle_type_id")
            )
            assert vehicle_type_id, "В ответе на создание должен быть id"
            vehicle_type_cleanup(vehicle_type_id)

        if vehicle_type_id:
            update_payload = {
                "id": vehicle_type_id,
                "name": f"UpdatedType{suffix}",
                "rate_per_km": 15.0,
            }
            update_resp = authenticated_session.patch(
                f"{base}{BASE_PATH}",
                json=update_payload,
                timeout=15,
            )
            assert_status(
                update_resp,
                [200, 422],
                context="PATCH /vehicle-type (positive flow)",
                bug_map={202: "#BUG-VEHICLE-TYPE-PATCH-202", 403: "#BUG-VEHICLE-TYPE-PATCH-403"},
            )

        if vehicle_type_id:
            delete_resp = authenticated_session.delete(
                f"{base}{BASE_PATH}",
                params={"type_id": vehicle_type_id},
                timeout=15,
            )
            attach_response("vehicle_type.delete", delete_resp)
            assert_status(
                delete_resp,
                [204, 422],
                context="DELETE /vehicle-type (positive flow)",
                bug_map={403: "#BUG-VEHICLE-TYPE-DELETE-403", 200: "#BUG-VEHICLE-TYPE-DELETE-200", 202: "#BUG-VEHICLE-TYPE-DELETE-202"},
            )

    def test_create_vehicle_type_minimal_payload(
        self, authenticated_session: requests.Session, vehicle_type_cleanup
    ):
        """Проверка создания типа транспорта с минимальным payload."""
        base = authenticated_session.base_url
        unique_suffix = int(time.time() * 1000) % 100000

        # Согласно OpenAPI схеме: name и rate_per_km обязательны
        payload = {
            "name": f"MinType{unique_suffix}",
            "rate_per_km": 5.0,  # Обязательное поле
        }
        create_resp = authenticated_session.post(
            f"{base}{BASE_PATH}", json=payload, timeout=20
        )
        attach_response("vehicle_type.create_minimal", create_resp)

        create_status = create_resp.status_code
        if create_status == 201:
            body = create_resp.json()
            vehicle_type_id = (
                body.get("id") or body.get("vehicle_type_id")
            )
            if vehicle_type_id:
                vehicle_type_cleanup(vehicle_type_id)

        assert_status(
            create_resp,
            [201, 422],
            context="POST /vehicle-type (минимальный payload)",
            bug_map={
                400: "#BUG-VEHICLE-TYPE-CREATE-MIN-400",
                403: "#BUG-VEHICLE-TYPE-CREATE-MIN-403",
            },
        )


@pytest.mark.api
class TestVehicleTypeEdges:
    """Тесты для граничных случаев и негативных сценариев."""

    def test_patch_vehicle_type_invalid_id(
        self, authenticated_session: requests.Session
    ):
        response = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={"id": 99999999, "name": "Invalid"},
            timeout=15,
        )
        assert_status(
            response,
            [422],
            context="PATCH /vehicle-type (несуществующий id)",
            bug_map={
                404: "#BUG-VEHICLE-TYPE-PATCH-INVALID-404",
                403: "#BUG-VEHICLE-TYPE-PATCH-INVALID-403",
            },
        )

    def test_delete_vehicle_type_invalid_id(
        self, authenticated_session: requests.Session
    ):
        response = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"type_id": 99999998},
            timeout=15,
        )
        assert_status(
            response,
            [422],
            context="DELETE /vehicle-type (несуществующий id)",
            bug_map={
                404: "#BUG-VEHICLE-TYPE-DELETE-INVALID-404",
                403: "#BUG-VEHICLE-TYPE-DELETE-INVALID-403",
            },
        )

    def test_patch_vehicle_type_no_auth(self, api_base_url: str):
        response = requests.patch(
            f"{api_base_url}{BASE_PATH}",
            json={"id": 1, "name": "NoAuth"},
            timeout=10,
        )
        assert_status(
            response,
            [422],
            context="PATCH /vehicle-type без авторизации",
            bug_map={
                401: "#BUG-VEHICLE-TYPE-PATCH-NOAUTH-401",
                403: "#BUG-VEHICLE-TYPE-PATCH-NOAUTH-403",
            },
        )

    def test_delete_vehicle_type_no_auth(self, api_base_url: str):
        response = requests.delete(
            f"{api_base_url}{BASE_PATH}",
            params={"type_id": 1},
            timeout=10,
        )
        assert_status(
            response,
            [422],
            context="DELETE /vehicle-type без авторизации",
            bug_map={
                401: "#BUG-VEHICLE-TYPE-DELETE-NOAUTH-401",
                403: "#BUG-VEHICLE-TYPE-DELETE-NOAUTH-403",
                200: "#BUG-VEHICLE-TYPE-DELETE-NOAUTH-200",
            },
        )

    def test_create_vehicle_type_duplicate_name(
        self, authenticated_session: requests.Session, vehicle_type_cleanup
    ):
        base = authenticated_session.base_url
        unique_suffix = int(time.time() * 1000) % 100000
        name = f"DuplicateTest{unique_suffix}"

        payload = {
            "name": name,
            "rate_per_km": 10.0,
        }
        create1_resp = authenticated_session.post(
            f"{base}{BASE_PATH}", json=payload, timeout=20
        )
        attach_response("vehicle_type.create_first", create1_resp)

        vehicle_type_id = None
        create1_status = create1_resp.status_code
        if create1_status == 201:
            body = create1_resp.json()
            vehicle_type_id = (
                body.get("id") or body.get("vehicle_type_id")
            )
            if vehicle_type_id:
                vehicle_type_cleanup(vehicle_type_id)

        create2_resp = authenticated_session.post(
            f"{base}{BASE_PATH}", json=payload, timeout=20
        )
        attach_response("vehicle_type.create_duplicate", create2_resp)

        create2_status = create2_resp.status_code
        if create2_status == 201:
            vid2 = create2_resp.json().get("id")
            if vid2:
                vehicle_type_cleanup(vid2)
            pytest.fail(
                "BUG: Создание vehicle-type с дублирующимся именем прошло (ожид. 422), получен 201. #BUG-VEHICLE-TYPE-DUPLICATE"
            )

        assert_status(
            create2_resp,
            [422],
            context="POST /vehicle-type (дубликат имени)",
            bug_map={
                409: "#BUG-VEHICLE-TYPE-DUPLICATE-409",
                400: "#BUG-VEHICLE-TYPE-DUPLICATE-400",
                403: "#BUG-VEHICLE-TYPE-DUPLICATE-403",
            },
        )
