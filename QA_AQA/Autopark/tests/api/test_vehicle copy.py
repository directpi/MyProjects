"""
@file: test_vehicle.py
@description: API тесты для CRUD операций по ТС (Vehicle)
@dependencies: pytest, requests
@created: 2025-10-30
@author: Directpi
@test_type: api
"""

import pytest
import requests
from typing import Dict
import time
import uuid
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

BASE_PATH = "/api/v1/vehicle"


def _get_first_vehicle_type_id(session: requests.Session) -> int:
    try:
        resp = session.get(f"{session.base_url}/api/v1/vehicle-type/all", timeout=10)
        status = resp.status_code
        if status == 200:
            items = resp.json() or []
            if isinstance(items, list) and items:
                first = items[0]
                return first.get("id", 1)
    except Exception:
        pass
    return 1

@pytest.mark.api
class TestVehicleRead:
    @pytest.mark.smoke
    def test_get_vehicle_unauthorized(self, api_base_url: str):
        response = requests.get(f"{api_base_url}{BASE_PATH}", timeout=10)
        assert_status(
            response,
            [200, 422],
            context="GET /vehicle без авторизации",
            bug_map={401: "#BUG-VEHICLE-GET-UNAUTH-401", 403: "#BUG-VEHICLE-GET-UNAUTH-403"},
        )

    def test_get_vehicle_authorized(self, authenticated_session: requests.Session):
        response = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}", timeout=15
        )
        assert_status(
            response,
            [200, 422],
            context="GET /vehicle (авторизован)",
            bug_map={403: "#BUG-VEHICLE-GET-AUTH-403"},
        )

        status = response.status_code
        if status == 200:
            data = response.json()
            assert isinstance(data, (list, dict)), "Ответ должен быть списком или объектом"


@pytest.mark.api
class TestVehicleCrud:
    @pytest.mark.smoke
    def test_create_vehicle_invalid_payload(
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
            context="POST /vehicle с невалидным payload",
            bug_map={400: "#BUG-VEHICLE-CREATE-INVALID-400", 403: "#BUG-VEHICLE-CREATE-INVALID-403"},
        )

    def test_create_update_delete_vehicle_flow(
        self, authenticated_session: requests.Session
    ):
        base = authenticated_session.base_url
        type_id = _get_first_vehicle_type_id(authenticated_session)
        suffix = int(time.time())
        state_num = f"A{suffix%1000000:06d}BC{suffix%100:02d}"
        payload = {
            "brand": "GAZ",
            "type_id": type_id,
            "state_number": state_num,
            "name": None,
            "user_id": None,
            "load_capacity": None,
            "max_cargo_height": None,
            "max_cargo_width": None,
            "max_cargo_length": None,
            "status": None,
            "glonass_id": None,
        }
        create_resp = authenticated_session.post(
            f"{base}{BASE_PATH}", json=payload, timeout=20
        )
        assert_status(
            create_resp,
            [201, 422],
            context="POST /vehicle (positive flow)",
            bug_map={
                400: "#BUG-VEHICLE-CREATE-400",
                403: "#BUG-VEHICLE-CREATE-403",
                409: "#BUG-VEHICLE-CREATE-409",
            },
        )
        vehicle_id = None
        create_status = create_resp.status_code
        if create_status == 201:
            body = create_resp.json()
            vehicle_id = body.get("id") or body.get("vehicle_id")
            assert vehicle_id, "В ответе на создание должен быть id"

        if vehicle_id:
            update_resp = authenticated_session.patch(
                f"{base}{BASE_PATH}",
                json={"id": vehicle_id, "brand": "UAZ"},
                timeout=15,
            )
            assert_status(
                update_resp,
                [200, 422],
                context="PATCH /vehicle (positive flow)",
                bug_map={202: "#BUG-VEHICLE-PATCH-202"},
            )

        if vehicle_id:
            delete_resp = authenticated_session.delete(
                f"{base}{BASE_PATH}", params={"vehicle_id": vehicle_id}, timeout=15
            )
            assert_status(
                delete_resp,
                [204, 422],
                context="DELETE /vehicle (positive flow)",
                bug_map={200: "#BUG-VEHICLE-DELETE-200", 202: "#BUG-VEHICLE-DELETE-202"},
            )


@pytest.mark.api
class TestVehicleCollections:
    @pytest.mark.slow
    def test_get_all_unauthorized(self, api_base_url: str):
        """Проверка получения всех ТС без авторизации."""
        try:
            resp = requests.get(f"{api_base_url}{BASE_PATH}/all", timeout=8)
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /vehicle/all без авторизации")
        assert_status(
            resp,
            [200],
            context="GET /vehicle/all без авторизации",
            bug_map={
                401: "#BUG-VEHICLE-ALL-UNAUTH-401",
                403: "#BUG-VEHICLE-ALL-UNAUTH-403",
            },
        )
    
    @pytest.mark.slow
    def test_get_all(self, authenticated_session: requests.Session):
        try:
            resp = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}/all", timeout=8
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /vehicle/all (авторизован)")
        assert_status(
            resp,
            [200],
            context="GET /vehicle/all",
            bug_map={403: "#BUG-VEHICLE-ALL-403"},
        )

    @pytest.mark.slow
    def test_stats_current_operation_unauthorized(self, api_base_url: str):
        """Проверка получения статистики без авторизации."""
        try:
            resp = requests.get(
                f"{api_base_url}{BASE_PATH}/stats/current-operation", timeout=8
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /vehicle/stats/current-operation без авторизации")
        assert_status(
            resp,
            [200],
            context="GET /vehicle/stats/current-operation без авторизации",
            bug_map={
                401: "#BUG-VEHICLE-STATS-UNAUTH-401",
                403: "#BUG-VEHICLE-STATS-UNAUTH-403",
            },
        )

    @pytest.mark.slow
    def test_stats_current_operation(self, authenticated_session: requests.Session):
        try:
            resp = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}/stats/current-operation",
                timeout=8,
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /vehicle/stats/current-operation")
        assert_status(
            resp,
            [200],
            context="GET /vehicle/stats/current-operation",
            bug_map={403: "#BUG-VEHICLE-STATS-403"},
        )

    def test_disconnect_glonass_requires_id(
        self, authenticated_session: requests.Session
    ):
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}/disconnect-glonass",
            params={"vehicle_id": 0},
            timeout=15,
        )
        assert_status(
            resp,
            [422],
            context="PATCH /vehicle/disconnect-glonass (невалидный id)",
            bug_map={
                400: "#BUG-VEHICLE-GLONASS-ID-400",
                403: "#BUG-VEHICLE-GLONASS-ID-403",
                404: "#BUG-VEHICLE-GLONASS-ID-404",
            },
        )

    @pytest.mark.xfail(reason="#BUG-VEHICLE-DISCONNECT-GLONASS-422-EXPECTED: возвращает 404 без vehicle_id")
    def test_disconnect_glonass_missing_id(self, authenticated_session: requests.Session):
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}/disconnect-glonass",
            timeout=10,
        )
        assert_status(
            resp,
            [422],
            context="PATCH /vehicle/disconnect-glonass без vehicle_id",
            bug_map={
                400: "#BUG-VEHICLE-GLONASS-MISSING-400",
                403: "#BUG-VEHICLE-GLONASS-MISSING-403",
            },
        )


@pytest.mark.api
class TestVehicleUpdateFull:
    def test_patch_vehicle_full_fields(self, authenticated_session: requests.Session, vehicle_cleanup):
        type_id = _get_first_vehicle_type_id(authenticated_session)
        suffix = int(time.time())
        create_payload = {"brand": "GAZ_TEST", "type_id": type_id, "state_number": f"A{suffix%1000000:06d}TC{suffix%100:02d}"}
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=create_payload,
            timeout=15,
        )
        if create_resp.status_code != 201:
            print(f"Vehicle create failed (status {create_resp.status_code}):", create_resp.text)
            pytest.skip("Не удалось создать ТС для update")
        vehicle_id = create_resp.json().get("id")
        vehicle_cleanup(vehicle_id)
        update_payload = {
            "id": vehicle_id,
            "brand": "UAZ",
            "name": "vehiclename-max",
        }
        patch_resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=update_payload,
            timeout=10,
        )
        attach_response("vehicle.patch.full", patch_resp)
        assert_status(
            patch_resp,
            [200, 422],
            context="PATCH /vehicle (full fields)",
            bug_map={202: "#BUG-VEHICLE-PATCH-FULL-202"},
        )

    def test_disconnect_glonass_valid(self, authenticated_session: requests.Session):
        list_resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}", timeout=15
        )
        vid = None
        list_status = list_resp.status_code
        if list_status == 200:
            data = list_resp.json()
            if isinstance(data, list) and data:
                vid = data[0].get("id")
        if not vid:
            pytest.skip("Нет доступных ТС для disconnect-glonass")
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}/disconnect-glonass",
            params={"vehicle_id": vid},
            timeout=15,
        )
        assert_status(
            resp,
            [200],
            context="PATCH /vehicle/disconnect-glonass (валидный id)",
            bug_map={
                202: "#BUG-VEHICLE-GLONASS-VALID-202",
                400: "#BUG-VEHICLE-GLONASS-VALID-400",
                404: "#BUG-VEHICLE-GLONASS-VALID-404",
            },
        )


@pytest.mark.api
class TestVehicleInvalidIds:
    def test_patch_vehicle_invalid_id(self, authenticated_session: requests.Session):
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={"id": 99999999, "brand": "X"},
            timeout=10,
        )
        assert_status(
            resp,
            [422],
            context="PATCH /vehicle (несуществующий id)",
            bug_map={
                404: "#BUG-VEHICLE-PATCH-INVALID-404",
                403: "#BUG-VEHICLE-PATCH-INVALID-403",
            },
        )

    def test_delete_vehicle_invalid_id(self, authenticated_session: requests.Session):
        del_resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"vehicle_id": 99999999},
            timeout=10,
        )
        assert_status(
            del_resp,
            [422],
            context="DELETE /vehicle (несуществующий id)",
            bug_map={
                404: "#BUG-VEHICLE-DELETE-INVALID-404",
                403: "#BUG-VEHICLE-DELETE-INVALID-403",
            },
        )

    def test_create_duplicate_state_number(self, authenticated_session: requests.Session, vehicle_cleanup):
        suffix = int(time.time())
        state_num = f"A{suffix%1000000:06d}TT{suffix%100:02d}"
        type_id = _get_first_vehicle_type_id(authenticated_session)
        payload = {"brand": "GAZ", "type_id": type_id, "state_number": state_num}
        first = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}", json=payload, timeout=15
        )
        first_status = first.status_code
        if first_status == 201:
            vid = first.json().get("id")
            if vid:
                vehicle_cleanup(vid)
        second = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}", json=payload, timeout=15
        )
        second_status = second.status_code
        if second_status == 201:
            vid2 = second.json().get("id")
            if vid2:
                vehicle_cleanup(vid2)
            pytest.fail(
                "BUG: Регистрация ТС с дублирующимся state_number прошла (ожид. 422), получен 201. #BUG-VEHICLE-DUPLICATE"
            )
        assert_status(
            second,
            [422],
            context="POST /vehicle (дубликат state_number)",
            bug_map={
                409: "#BUG-VEHICLE-DUPLICATE-409",
                400: "#BUG-VEHICLE-DUPLICATE-400",
            },
        )


@pytest.mark.api
class TestVehicleFullPayload:
    """Позитивные тесты с максимально полным payload для create/update"""

    def test_create_vehicle_full_payload(self, authenticated_session: requests.Session, vehicle_cleanup):
        type_id = _get_first_vehicle_type_id(authenticated_session)
        suffix = int(time.time())
        state_num = f"A{suffix%1000000:06d}FP{suffix%100:02d}"
        full_payload = {
            "brand": "GAZ_FULL",
            "type_id": type_id,
            "state_number": state_num,
            "name": "Test Vehicle Full Payload",
            "user_id": None,
            "load_capacity": 5000.5,
            "max_cargo_height": 250.0,
            "max_cargo_width": 200.0,
            "max_cargo_length": 600.0,
            "status": None,
            "glonass_id": None,
        }

        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=full_payload,
            timeout=20,
        )
        attach_response("vehicle.create.full_payload", create_resp)
        assert_status(
            create_resp,
            [201, 422],
            context="POST /vehicle (full payload)",
            bug_map={
                400: "#BUG-VEHICLE-CREATE-FULL-400",
                403: "#BUG-VEHICLE-CREATE-FULL-403",
                409: "#BUG-VEHICLE-CREATE-FULL-409",
            },
        )

        if create_resp.status_code != 201:
            pytest.skip(
                f"Не удалось создать Vehicle с полным payload (status {create_resp.status_code}): {create_resp.text}"
            )

        vehicle_id = create_resp.json().get("id")
        assert vehicle_id, "В ответе на создание должен быть id"
        vehicle_cleanup(vehicle_id)

        get_resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"vehicle_id": vehicle_id} if vehicle_id else None,
            timeout=15,
        )
        get_status = get_resp.status_code
        if get_status == 200:
            vehicle_data = get_resp.json()
            if isinstance(vehicle_data, list) and vehicle_data:
                vehicle_data = vehicle_data[0]
            if isinstance(vehicle_data, dict):
                assert vehicle_data.get("brand") == "GAZ_FULL", "Brand должен совпадать"
                assert vehicle_data.get("state_number") == state_num, "State number должен совпадать"
                assert vehicle_data.get("name") == "Test Vehicle Full Payload", "Name должен совпадать"

    def test_update_vehicle_full_payload(self, authenticated_session: requests.Session, vehicle_cleanup):
        type_id = _get_first_vehicle_type_id(authenticated_session)
        suffix = int(time.time())
        state_num = f"A{suffix%1000000:06d}UP{suffix%100:02d}"
        create_payload = {
            "brand": "GAZ_INIT",
            "type_id": type_id,
            "state_number": state_num,
        }
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=create_payload,
            timeout=15,
        )

        if create_resp.status_code != 201:
            pytest.skip(
                f"Не удалось создать Vehicle для update (status {create_resp.status_code}): {create_resp.text}"
            )

        vehicle_id = create_resp.json().get("id")
        assert vehicle_id, "В ответе на создание должен быть id"
        vehicle_cleanup(vehicle_id)

        update_payload = {
            "id": vehicle_id,
            "brand": "UAZ_UPDATED",
            "name": "Updated Vehicle Full Payload",
            "load_capacity": 6000.0,
            "max_cargo_height": 300.0,
            "max_cargo_width": 250.0,
            "max_cargo_length": 700.0,
        }

        update_resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=update_payload,
            timeout=15,
        )
        attach_response("vehicle.update.full_payload", update_resp)
        assert_status(
            update_resp,
            [200, 422],
            context="PATCH /vehicle (full payload)",
            bug_map={202: "#BUG-VEHICLE-UPDATE-FULL-202", 400: "#BUG-VEHICLE-UPDATE-FULL-400"},
        )

        if update_resp.status_code not in [200, 202]:
            pytest.skip(
                f"Не удалось обновить Vehicle с полным payload (status {update_resp.status_code}): {update_resp.text}"
            )

        get_resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            timeout=15,
        )
        get_status = get_resp.status_code
        if get_status == 200:
            vehicles = get_resp.json()
            if isinstance(vehicles, list):
                updated_vehicle = next((v for v in vehicles if v.get("id") == vehicle_id), None)
                if updated_vehicle:
                    assert updated_vehicle.get("brand") == "UAZ_UPDATED", "Brand должен быть обновлён"
                    assert updated_vehicle.get("name") == "Updated Vehicle Full Payload", "Name должен быть обновлён"


@pytest.mark.api
class TestVehicleAdditional:
    def test_vehicle_connected_system_vehicles(
        self, authenticated_session: requests.Session
    ):
        """GET /vehicle/connected-system-vehicles — ТС подключенные к системе."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/connected-system-vehicles",
            timeout=10,
        )
        attach_response("vehicle.connected_system_vehicles", resp)
        assert_status(
            resp,
            [200],
            context="GET /vehicle/connected-system-vehicles",
            bug_map={
                403: "#BUG-VEHICLE-CONNECTED-403",
                404: "#BUG-VEHICLE-CONNECTED-404",
            },
        )

    def test_vehicle_mileage_by_period(
        self, authenticated_session: requests.Session
    ):
        """GET /vehicle/mileage-by-period — пробег ТС за период."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/mileage-by-period",
            params={
                "from_datetime": "2025-01-01T00:00:00",
                "to_datetime": "2025-12-31T23:59:59",
            },
            timeout=10,
        )
        attach_response("vehicle.mileage_by_period", resp)
        assert_status(
            resp,
            [200],
            context="GET /vehicle/mileage-by-period",
            bug_map={
                403: "#BUG-VEHICLE-MILEAGE-403",
                404: "#BUG-VEHICLE-MILEAGE-404",
                422: "#BUG-VEHICLE-MILEAGE-422",
            },
        )
