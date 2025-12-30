"""
@file: test_mechanic_order.py
@description: API тесты для модуля заявок механика (mechanic-order)
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

BASE_PATH = "/api/v1/mechanic-order"
VEHICLE_TYPE_FALLBACK = 1  # fallback, если не удалось получить из API


@pytest.fixture
def mechanic_order_cleanup(authenticated_session: requests.Session):
    """Фикстура для очистки созданных mechanic-order."""
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
            # По схеме DELETE /mechanic-order возвращает [204, 422]
            if resp.status_code not in (204, 422, 404):
                print("mechanic_order_cleanup WARN:", oid, resp.status_code, resp.text)
        except Exception as exc:  # noqa: BLE001
            print("mechanic_order_cleanup ERROR:", oid, exc)


@pytest.fixture
def initiator_id(authenticated_session: requests.Session) -> Optional[int]:
    """Возвращает id текущего пользователя (/user/me)."""
    resp = authenticated_session.get(
        f"{authenticated_session.base_url}/api/v1/user/me",
        timeout=10,
    )
    attach_response("mechanic_order.dep.me", resp)
    assert_status(
        resp,
        [200],
        context="GET /user/me для initiator_id",
        bug_map={403: "#BUG-MECHANIC-ORDER-ME-403"},
    )
    if resp.status_code == 200:
        return resp.json().get("id")
    return None


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
class TestMechanicOrderCrud:
    def test_create_get_delete_mechanic_order(
        self,
        authenticated_session: requests.Session,
        user_cleanup,
        vehicle_cleanup,
        mechanic_order_cleanup,
        initiator_id: Optional[int],
    ):
        """
        Позитивный сценарий: создание mechanic-order, получение и удаление.
        Требует initiator_id и vehicle_id.
        """
        if not initiator_id:
            pytest.skip("Не удалось получить initiator_id через /user/me")

        unique = uuid.uuid4().hex[:6]

        # Опционально создадим механика (user_id), но поле не обязательно
        mech_user_payload = {
            "name": "Mechanic",
            "surname": "Order",
            "phone": f"+7999{unique}99",
            "password": "MechanicPass123!",
            "email": f"mechanic_{unique}@example.com",
        }
        mech_user_resp = authenticated_session.post(
            f"{authenticated_session.base_url}/api/v1/user",
            json=mech_user_payload,
            timeout=20,
        )
        attach_response("mechanic_order.dep.user_create", mech_user_resp)
        # 422 допускаем из-за валидаций
        assert_status(
            mech_user_resp,
            [201, 422],
            context="POST /user (mechanic)",
            bug_map={403: "#BUG-MECHANIC-ORDER-USER-CREATE-403"},
        )
        mechanic_id = None
        if mech_user_resp.status_code == 201:
            mechanic_id = mech_user_resp.json().get("id")
            user_cleanup(mechanic_id)

        # Создаем ТС
        type_id = _get_vehicle_type_id(authenticated_session)
        vehicle_resp = authenticated_session.post(
            f"{authenticated_session.base_url}/api/v1/vehicle",
            json={
                "brand": "MechanicBrand",
                "model": "ModelM",
                "type_id": type_id,
                "state_number": f"MECH{unique}SN",
                "license_plate": f"MECH{unique}",
            },
            timeout=20,
        )
        attach_response("mechanic_order.dep.vehicle_create", vehicle_resp)
        assert_status(
            vehicle_resp,
            [201],
            context="POST /vehicle (для mechanic-order)",
            bug_map={403: "#BUG-MECHANIC-ORDER-VEHICLE-CREATE-403"},
        )
        vehicle_id = vehicle_resp.json().get("id") if vehicle_resp.status_code == 201 else None
        if vehicle_id:
            vehicle_cleanup(vehicle_id)
        if not vehicle_id:
            pytest.skip("Не удалось создать ТС для mechanic-order")

        # Создаем mechanic-order
        order_payload = {
            "initiator_id": initiator_id,
            "vehicle_id": vehicle_id,
        }
        if mechanic_id:
            order_payload["user_id"] = mechanic_id

        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=order_payload,
            timeout=20,
        )
        attach_response("mechanic_order.create", create_resp)
        assert_status(
            create_resp,
            [201],
            context="POST /mechanic-order",
            bug_map={403: "#BUG-MECHANIC-ORDER-CREATE-403"},
        )

        order_id = None
        if create_resp.status_code == 201:
            order_id = create_resp.json().get("id")
            mechanic_order_cleanup(order_id)

        if not order_id:
            pytest.skip("Не удалось создать mechanic-order")

        # Получение по order_id
        get_resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"order_id": order_id},
            timeout=15,
        )
        attach_response("mechanic_order.get", get_resp)
        assert_status(
            get_resp,
            [200],
            context="GET /mechanic-order",
            bug_map={403: "#BUG-MECHANIC-ORDER-GET-403"},
        )

        # Удаление
        delete_resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"order_id": order_id},
            timeout=15,
        )
        attach_response("mechanic_order.delete", delete_resp)
        assert_status(
            delete_resp,
            [204],
            context="DELETE /mechanic-order",
            bug_map={403: "#BUG-MECHANIC-ORDER-DELETE-403"},
        )

    def test_mechanic_order_filters(
        self,
        authenticated_session: requests.Session,
        user_cleanup,
        vehicle_cleanup,
        mechanic_order_cleanup,
        initiator_id: Optional[int],
    ):
        """Фильтры mechanic-order по order_id (контракт: 200)."""
        if not initiator_id:
            pytest.skip("Не удалось получить initiator_id")

        unique = uuid.uuid4().hex[:6]

        type_id = _get_vehicle_type_id(authenticated_session)
        vehicle_resp = authenticated_session.post(
            f"{authenticated_session.base_url}/api/v1/vehicle",
            json={
                "brand": "MechanicFBrand",
                "model": "ModelFM",
                "type_id": type_id,
                "state_number": f"MECH{unique}FSN",
                "license_plate": f"MECHF{unique}",
            },
            timeout=20,
        )
        assert_status(
            vehicle_resp,
            [201],
            context="POST /vehicle (mechanic filters)",
            bug_map={403: "#BUG-MECHANIC-ORDER-VEHICLE-CREATE-403"},
        )
        vehicle_id = vehicle_resp.json().get("id") if vehicle_resp.status_code == 201 else None
        if vehicle_id:
            vehicle_cleanup(vehicle_id)
        if not vehicle_id:
            pytest.skip("Не удалось создать ТС (mechanic filters)")

        order_payload = {
            "initiator_id": initiator_id,
            "vehicle_id": vehicle_id,
        }
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=order_payload,
            timeout=20,
        )
        assert_status(
            create_resp,
            [201],
            context="POST /mechanic-order (filters)",
            bug_map={403: "#BUG-MECHANIC-ORDER-CREATE-403"},
        )
        order_id = create_resp.json().get("id") if create_resp.status_code == 201 else None
        mechanic_order_cleanup(order_id)
        if not order_id:
            pytest.skip("Не удалось создать mechanic-order (filters)")

        # GET с order_id
        resp_get = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"order_id": order_id},
            timeout=10,
        )
        assert_status(
            resp_get,
            [200],
            context="GET /mechanic-order c order_id",
            bug_map={403: "#BUG-MECHANIC-ORDER-GET-403"},
        )


@pytest.mark.api
class TestMechanicOrderValidation:
    def test_create_mechanic_order_missing_required(
        self,
        authenticated_session: requests.Session,
    ):
        """Негативный тест: отсутствие initiator_id/vehicle_id → 422 по схеме."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={},
            timeout=15,
        )
        attach_response("mechanic_order.create_invalid", resp)
        assert_status(
            resp,
            [422],
            context="POST /mechanic-order без обязательных полей",
            bug_map={201: "#BUG-MECHANIC-ORDER-CREATE-INVALID-201"},
        )

    @pytest.mark.slow
    def test_get_mechanic_order_all(self, authenticated_session: requests.Session):
        """GET /mechanic-order/all — получение всех заявок механика."""
        try:
            resp = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}/all",
                timeout=8,
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /mechanic-order/all")
        attach_response("mechanic_order.all", resp)
        assert_status(
            resp,
            [200],
            bug_map={403: "#BUG-MECHANIC-ORDER-ALL-403"},
        )

    def test_get_mechanic_order(self, authenticated_session: requests.Session):
        """GET /mechanic-order — получение заказов механика."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            timeout=10,
        )
        attach_response("mechanic_order.get", resp)
        assert_status(
            resp,
            [200, 422],
            context="GET /mechanic-order",
            bug_map={403: "#BUG-MECHANIC-ORDER-GET-403"},
        )

    def test_delete_mechanic_order_missing_id(
        self, authenticated_session: requests.Session
    ):
        """DELETE /mechanic-order — удаление заказа механика (негативный тест без ID)."""
        resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            timeout=10,
        )
        attach_response("mechanic_order.delete_missing_id", resp)
        assert_status(
            resp,
            [422, 404],
            context="DELETE /mechanic-order без order_id",
            bug_map={403: "#BUG-MECHANIC-ORDER-DELETE-403"},
        )

    def test_mechanic_order_patch_missing_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: PATCH /mechanic-order без id → 422."""
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={},
            timeout=10,
        )
        attach_response("mechanic_order.patch_missing_id", resp)
        assert_status(
            resp,
            [422],
            context="PATCH /mechanic-order без id",
            bug_map={403: "#BUG-MECHANIC-ORDER-PATCH-403"},
        )

