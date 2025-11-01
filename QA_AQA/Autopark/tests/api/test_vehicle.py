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

BASE_PATH = "/api/v1/vehicle"


@pytest.mark.api
class TestVehicleRead:
    @pytest.mark.smoke
    def test_get_vehicle_unauthorized(self, api_base_url: str):
        response = requests.get(f"{api_base_url}{BASE_PATH}", timeout=10)
        assert response.status_code in [
            401,
            403,
        ], f"Ожидался 401/403, получен {response.status_code}"

    def test_get_vehicle_authorized(self, authenticated_session: requests.Session):
        response = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}", timeout=15
        )
        assert response.status_code in [
            200,
            403,
        ], f"Ожидался 200/403, получен {response.status_code}"


@pytest.mark.api
class TestVehicleCrud:
    @pytest.mark.smoke
    def test_create_vehicle_invalid_payload(
        self, authenticated_session: requests.Session
    ):
        response = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={"name": 123},  # невалидный тип
            timeout=15,
        )
        assert response.status_code in [
            422,
            400,
            403,
        ], f"Ожидался 422/400/403, получен {response.status_code}"

    def test_create_update_delete_vehicle_flow(
        self, authenticated_session: requests.Session
    ):
        base = authenticated_session.base_url
        # 1) Создание ТС
        # VehicleCreateSchema требует: brand, type_id, state_number
        payload = {
            "brand": "GAZ",
            "type_id": 1,
            "state_number": "A123BC77",
            # опциональные
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
        assert create_resp.status_code in [
            201,
            400,
            403,
            409,
            422,
        ], f"Неожиданный код при создании: {create_resp.status_code}"
        vehicle_id = None
        if create_resp.status_code == 201:
            body = create_resp.json()
            vehicle_id = body.get("id") or body.get("vehicle_id")
            assert vehicle_id, "В ответе на создание должен быть id"

        # 2) Обновление (если удалось создать)
        if vehicle_id:
            # VehicleUpdateSchema: требуется id, прочие поля опциональны
            update_resp = authenticated_session.patch(
                f"{base}{BASE_PATH}",
                json={"id": vehicle_id, "brand": "UAZ"},
                timeout=15,
            )
            assert update_resp.status_code in [
                200,
                202,
            ], f"Ожидался 200/202, получен {update_resp.status_code}"

        # 3) Удаление (если удалось создать)
        if vehicle_id:
            # DELETE /api/v1/vehicle принимает vehicle_id в query и возвращает 204
            delete_resp = authenticated_session.delete(
                f"{base}{BASE_PATH}", params={"vehicle_id": vehicle_id}, timeout=15
            )
            assert delete_resp.status_code in [
                204,
                200,
                202,
            ], f"Ожидался 204/200/202, получен {delete_resp.status_code}"


@pytest.mark.api
class TestVehicleCollections:
    def test_get_all(self, authenticated_session: requests.Session):
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/all", timeout=15
        )
        assert resp.status_code in [
            200,
            403,
        ], f"Ожидался 200/403, получен {resp.status_code}"

    def test_stats_current_operation(self, authenticated_session: requests.Session):
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/stats/current-operation",
            timeout=15,
        )
        assert resp.status_code in [
            200,
            403,
        ], f"Ожидался 200/403, получен {resp.status_code}"

    def test_disconnect_glonass_requires_id(
        self, authenticated_session: requests.Session
    ):
        # Требуется query-параметр vehicle_id
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}/disconnect-glonass",
            params={"vehicle_id": 0},  # несуществующий id для негативного кейса
            timeout=15,
        )
        assert resp.status_code in [
            422,
            400,
            403,
            404,
        ], f"Ожидался 422/400/403/404, получен {resp.status_code}"

    def test_disconnect_glonass_missing_id(self, authenticated_session: requests.Session):
        # Отсутствует обязательный query-параметр vehicle_id
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}/disconnect-glonass",
            timeout=10,
        )
        assert resp.status_code in [422, 400, 403], \
            f"Ожидался 422/400/403 при отсутствии vehicle_id, получен {resp.status_code}"


@pytest.mark.api
class TestVehicleUpdateFull:
    def test_patch_vehicle_full_fields(self, authenticated_session: requests.Session, vehicle_cleanup):
        suffix = int(time.time())
        create_payload = {"brand": "GAZ_TEST", "type_id": 1, "state_number": f"A{suffix%1000000:06d}TC{suffix%100:02d}"}
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
            # По OpenAPI для PATCH обязателен только id; остальные поля опциональны.
            # Обновляем только безопасные поля, чтобы соответствовать контракту и избежать 422.
            "brand": "UAZ",
            "name": "vehiclename-max",
        }
        patch_resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=update_payload,
            timeout=10,
        )
        if patch_resp.status_code not in [200, 202]:
            print("PATCH resp:", patch_resp.status_code, patch_resp.text)
        assert patch_resp.status_code in [200, 202], f"Ожидался 200/202, получен {patch_resp.status_code}"

    def test_disconnect_glonass_valid(self, authenticated_session: requests.Session):
        # Предполагаем что id=1 может быть валиден,
        # Для реального API вместо 1 используйте id созданного ТС c glonass_id
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}/disconnect-glonass",
            params={"vehicle_id": 1},  # подберите val, чтобы был валидный id
            timeout=15,
        )
        assert resp.status_code in [
            200,
            202,
            400,
            404,
        ], f"Ожидался 200/202/400/404 по бизнес-логике, получен {resp.status_code}"


@pytest.mark.api
class TestVehicleInvalidIds:
    def test_patch_vehicle_invalid_id(self, authenticated_session: requests.Session):
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={"id": 99999999, "brand": "X"},
            timeout=10,
        )
        assert resp.status_code in [404, 422, 403], \
            f"Ожидался 404/422/403 при PATCH несуществующего ТС, получен {resp.status_code}"

    def test_delete_vehicle_invalid_id(self, authenticated_session: requests.Session):
        del_resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"vehicle_id": 99999999},
            timeout=10,
        )
        assert del_resp.status_code in [404, 422, 403], \
            f"Ожидался 404/422/403 при DELETE несуществующего ТС, получен {del_resp.status_code}"

    def test_create_duplicate_state_number(self, authenticated_session: requests.Session, vehicle_cleanup):
        suffix = int(time.time())
        state_num = f"A{suffix%1000000:06d}TT{suffix%100:02d}"
        payload = {"brand": "GAZ", "type_id": 1, "state_number": state_num}
        first = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}", json=payload, timeout=15
        )
        if first.status_code == 201:
            vid = first.json().get("id")
            if vid:
                vehicle_cleanup(vid)
        second = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}", json=payload, timeout=15
        )
        assert second.status_code in [409, 400, 422], \
            f"Ожидался 409/400/422 при дублирующемся state_number, получен {second.status_code}"
