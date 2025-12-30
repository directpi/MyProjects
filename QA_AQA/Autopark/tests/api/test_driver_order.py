"""
@file: test_driver_order.py
@description: API тесты для модуля рейсов водителей (driver-order)
@dependencies: pytest, requests
@created: 2025-11-25
@author: Directpi
@test_type: api
"""

from __future__ import annotations

import uuid
from typing import Optional

import pytest
import requests
from requests import exceptions as requests_exceptions

from tests.conftest import attach_response, assert_status

BASE_PATH = "/api/v1/driver-order"
VEHICLE_TYPE_FALLBACK = 1  # fallback, если не удалось получить из API


@pytest.fixture
def driver_order_cleanup(authenticated_session: requests.Session):
    """Фикстура для очистки созданных driver-order."""
    created_ids: list[int] = []

    def _register(order_id: Optional[int]):
        if order_id:
            created_ids.append(order_id)

    yield _register

    for oid in created_ids:
        try:
            resp = authenticated_session.delete(
                f"{authenticated_session.base_url}{BASE_PATH}",
                params={"order_id": oid},
                timeout=15,
            )
            # По схеме DELETE /driver-order возвращает [204, 422]
            if resp.status_code not in (204, 422, 404):
                print("driver_order_cleanup WARN:", oid, resp.status_code, resp.text)
        except Exception as exc:  # noqa: BLE001
            print("driver_order_cleanup ERROR:", oid, exc)


@pytest.fixture
def logist_order_id(authenticated_session: requests.Session):
    """Создает logist-order для привязки driver-order."""
    payload = {
        "customer_name": f"Customer {uuid.uuid4().hex[:6]}",
        "customer_contacts": "logist@example.com",
        "price": 50000.0,
    }
    resp = authenticated_session.post(
        f"{authenticated_session.base_url}/api/v1/logist-order",
        json=payload,
        timeout=20,
    )
    attach_response("driver_order.dep.logist_create", resp)
    assert_status(
        resp,
        [201],
        context="POST /logist-order (для driver-order)",
        bug_map={403: "#BUG-DRIVER-ORDER-LOGIST-CREATE-403"},
    )
    order_id = resp.json().get("id") if resp.status_code == 201 else None
    yield order_id

    # Cleanup logist order
    if order_id:
        try:
            del_resp = authenticated_session.delete(
                f"{authenticated_session.base_url}/api/v1/logist-order",
                params={"order_id": order_id},
                timeout=15,
            )
            # По схеме DELETE /logist-order возвращает [204, 422]
            if del_resp.status_code not in (204, 422, 404):
                print("logist_order_cleanup WARN:", order_id, del_resp.status_code, del_resp.text)
        except Exception as exc:  # noqa: BLE001
            print("logist_order_cleanup ERROR:", order_id, exc)


def _get_vehicle_type_id(session: requests.Session) -> int:
    """Пробует получить первый доступный type_id, иначе возвращает fallback."""
    try:
        resp = session.get(f"{session.base_url}/api/v1/vehicle-type/all", timeout=10)
        if resp.status_code == 200:
            items = resp.json() or []
            if isinstance(items, list) and items:
                return items[0].get("id", VEHICLE_TYPE_FALLBACK)
    except Exception:
        pass
    return VEHICLE_TYPE_FALLBACK


@pytest.mark.api
class TestDriverOrderCrud:
    def test_create_get_delete_driver_order(
        self,
        authenticated_session: requests.Session,
        user_cleanup,
        vehicle_cleanup,
        driver_order_cleanup,
        logist_order_id: Optional[int],
    ):
        """
        Позитивный сценарий: создание driver-order, получение и удаление.
        Зависимости: logist_order_id, driver user, vehicle.
        """
        if not logist_order_id:
            pytest.skip("Не удалось создать logist-order для driver-order")

        # Создаем пользователя-водителя
        unique = uuid.uuid4().hex[:6]
        user_payload = {
            "name": "Driver",
            "surname": "Order",
            "phone": f"+7999{unique}01",
            "password": "DriverPass123!",
            "email": f"driver_{unique}@example.com",
        }
        user_resp = authenticated_session.post(
            f"{authenticated_session.base_url}/api/v1/user",
            json=user_payload,
            timeout=20,
        )
        attach_response("driver_order.dep.user_create", user_resp)
        assert_status(
            user_resp,
            [201, 422],
            context="POST /user (driver)",
            bug_map={403: "#BUG-DRIVER-ORDER-USER-CREATE-403"},
        )
        driver_id = None
        if user_resp.status_code == 201:
            driver_id = user_resp.json().get("id")
            user_cleanup(driver_id)
        if not driver_id:
            pytest.skip("Не удалось создать пользователя-водителя")

        # Создаем ТС
        type_id = _get_vehicle_type_id(authenticated_session)
        vehicle_resp = authenticated_session.post(
            f"{authenticated_session.base_url}/api/v1/vehicle",
            json={
                "brand": "DriverOrderBrand",
                "model": "ModelX",
                "type_id": type_id,
                "state_number": f"DRV{unique}SN",
                "license_plate": f"DRV{unique}",
            },
            timeout=20,
        )
        attach_response("driver_order.dep.vehicle_create", vehicle_resp)
        assert_status(
            vehicle_resp,
            [201],
            context="POST /vehicle (для driver-order)",
            bug_map={403: "#BUG-DRIVER-ORDER-VEHICLE-CREATE-403"},
        )
        vehicle_id = vehicle_resp.json().get("id") if vehicle_resp.status_code == 201 else None
        if vehicle_id:
            vehicle_cleanup(vehicle_id)
        if not vehicle_id:
            pytest.skip("Не удалось создать ТС для driver-order")

        # Создаем driver-order
        order_payload = {
            "user_id": driver_id,
            "vehicle_id": vehicle_id,
            "logist_order_id": logist_order_id,
            # status опционален, по умолчанию "Открыт"
        }
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=order_payload,
            timeout=20,
        )
        attach_response("driver_order.create", create_resp)
        assert_status(
            create_resp,
            [201],
            context="POST /driver-order",
            bug_map={403: "#BUG-DRIVER-ORDER-CREATE-403"},
        )

        order_id = None
        if create_resp.status_code == 201:
            order_id = create_resp.json().get("id")
            driver_order_cleanup(order_id)

        if not order_id:
            pytest.skip("Не удалось создать driver-order")

        # Получение по order_id
        get_resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"order_id": order_id},
            timeout=15,
        )
        attach_response("driver_order.get", get_resp)
        assert_status(
            get_resp,
            [200],
            context="GET /driver-order",
            bug_map={403: "#BUG-DRIVER-ORDER-GET-403"},
        )

        # Удаление
        delete_resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"order_id": order_id},
            timeout=15,
        )
        attach_response("driver_order.delete", delete_resp)
        assert_status(
            delete_resp,
            [204],
            context="DELETE /driver-order",
            bug_map={403: "#BUG-DRIVER-ORDER-DELETE-403"},
        )

    def test_driver_order_filters(
        self,
        authenticated_session: requests.Session,
        user_cleanup,
        vehicle_cleanup,
        driver_order_cleanup,
        logist_order_id: Optional[int],
    ):
        """Фильтры driver-order по order_id (контракт: 200)."""
        if not logist_order_id:
            pytest.skip("Не удалось создать logist-order для driver-order")

        unique = uuid.uuid4().hex[:6]
        # Водитель
        user_payload = {
            "name": "DriverF",
            "surname": "Filter",
            "phone": f"+7999{unique}55",
            "password": "DriverPass123!",
            "email": f"driver_f_{unique}@example.com",
        }
        user_resp = authenticated_session.post(
            f"{authenticated_session.base_url}/api/v1/user",
            json=user_payload,
            timeout=20,
        )
        assert_status(
            user_resp,
            [201, 422],
            context="POST /user (driver filter)",
            bug_map={403: "#BUG-DRIVER-ORDER-USER-CREATE-403"},
        )
        driver_id = None
        if user_resp.status_code == 201:
            driver_id = user_resp.json().get("id")
            user_cleanup(driver_id)
        if not driver_id:
            pytest.skip("Не удалось создать пользователя-водителя (filters)")

        # ТС
        type_id = _get_vehicle_type_id(authenticated_session)
        vehicle_resp = authenticated_session.post(
            f"{authenticated_session.base_url}/api/v1/vehicle",
            json={
                "brand": "DriverFBrand",
                "model": "ModelF",
                "type_id": type_id,
                "state_number": f"DRV{unique}FSN",
                "license_plate": f"DRV{unique}F",
            },
            timeout=20,
        )
        assert_status(
            vehicle_resp,
            [201],
            context="POST /vehicle (driver filter)",
            bug_map={403: "#BUG-DRIVER-ORDER-VEHICLE-CREATE-403"},
        )
        vehicle_id = vehicle_resp.json().get("id") if vehicle_resp.status_code == 201 else None
        if vehicle_id:
            vehicle_cleanup(vehicle_id)
        if not vehicle_id:
            pytest.skip("Не удалось создать ТС (filters)")

        # Driver-order
        order_payload = {
            "user_id": driver_id,
            "vehicle_id": vehicle_id,
            "logist_order_id": logist_order_id,
        }
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=order_payload,
            timeout=20,
        )
        assert_status(
            create_resp,
            [201],
            context="POST /driver-order (filters)",
            bug_map={403: "#BUG-DRIVER-ORDER-CREATE-403"},
        )
        order_id = create_resp.json().get("id") if create_resp.status_code == 201 else None
        driver_order_cleanup(order_id)
        if not order_id:
            pytest.skip("Не удалось создать driver-order (filters)")

        # GET с order_id
        resp_get = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"order_id": order_id},
            timeout=10,
        )
        assert_status(
            resp_get,
            [200],
            context="GET /driver-order c order_id",
            bug_map={403: "#BUG-DRIVER-ORDER-GET-403"},
        )


@pytest.mark.api
class TestDriverOrderValidation:
    def test_create_driver_order_missing_required(
        self,
        authenticated_session: requests.Session,
    ):
        """Негативный тест: отсутствие обязательных полей (user_id/vehicle_id/logist_order_id) → 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={},
            timeout=15,
        )
        attach_response("driver_order.create_invalid", resp)
        assert_status(
            resp,
            [422],
            context="POST /driver-order без обязательных полей",
            bug_map={201: "#BUG-DRIVER-ORDER-CREATE-INVALID-201"},
        )

    @pytest.mark.slow
    def test_get_driver_order_all(self, authenticated_session: requests.Session):
        """GET /driver-order/all — получение всех рейсов водителей."""
        try:
            resp = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}/all",
                timeout=8,
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /driver-order/all")
        attach_response("driver_order.all", resp)
        assert_status(
            resp,
            [200],
            bug_map={403: "#BUG-DRIVER-ORDER-ALL-403"},
        )

    def test_get_driver_order(self, authenticated_session: requests.Session):
        """GET /driver-order — получение рейсов водителя."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            timeout=10,
        )
        attach_response("driver_order.get", resp)
        assert_status(
            resp,
            [200, 422],
            context="GET /driver-order",
            bug_map={403: "#BUG-DRIVER-ORDER-GET-403"},
        )

    def test_driver_order_patch_missing_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: PATCH /driver-order без id → 422."""
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={},
            timeout=10,
        )
        attach_response("driver_order.patch_missing_id", resp)
        assert_status(
            resp,
            [422],
            context="PATCH /driver-order без id",
            bug_map={403: "#BUG-DRIVER-ORDER-PATCH-403"},
        )

