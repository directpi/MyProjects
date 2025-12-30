"""
@file: test_logist_order.py
@description: API тесты для модуля логистических заказов (logist-order)
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

from tests.conftest import attach_response, assert_status

BASE_PATH = "/api/v1/logist-order"


@pytest.fixture
def logist_order_cleanup(authenticated_session: requests.Session):
    """Фикстура для очистки созданных логистических заказов."""
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
            # По схеме DELETE /logist-order возвращает [204, 422]
            if resp.status_code not in (204, 422, 404):
                print("logist_order_cleanup WARN:", oid, resp.status_code, resp.text)
        except Exception as exc:  # noqa: BLE001
            print("logist_order_cleanup ERROR:", oid, exc)


@pytest.mark.api
class TestLogistOrderCrud:
    @pytest.mark.smoke
    def test_create_get_delete_logist_order(
        self,
        authenticated_session: requests.Session,
        logist_order_cleanup,
    ):
        """
        Позитивный сценарий: создание рейса, получение и удаление.
        Контракт: POST 201, GET 200, DELETE 204.
        """
        unique = uuid.uuid4().hex[:8]
        payload = {
            "customer_name": f"Customer {unique}",
            "customer_contacts": "test@example.com",
            "price": 12345.67,
            # Дополнительные поля схемы опциональны (loading_*, unloading_*, status)
        }

        # Создание
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=payload,
            timeout=20,
        )
        attach_response("logist_order.create", create_resp)
        assert_status(
            create_resp,
            [201],
            context="POST /logist-order",
            bug_map={403: "#BUG-LOGIST-ORDER-CREATE-403"},
        )

        order_id = None
        if create_resp.status_code == 201:
            order_id = create_resp.json().get("id")
            logist_order_cleanup(order_id)

        if not order_id:
            pytest.skip("Не удалось получить order_id после создания")

        # Получение по order_id
        get_resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"order_id": order_id},
            timeout=15,
        )
        attach_response("logist_order.get", get_resp)
        assert_status(
            get_resp,
            [200],
            context="GET /logist-order",
            bug_map={403: "#BUG-LOGIST-ORDER-GET-403"},
        )

        # Получение списка all
        all_resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/all",
            timeout=15,
        )
        attach_response("logist_order.all", all_resp)
        assert_status(
            all_resp,
            [200],
            context="GET /logist-order/all",
            bug_map={403: "#BUG-LOGIST-ORDER-ALL-403"},
        )

        # Удаление
        delete_resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"order_id": order_id},
            timeout=15,
        )
        attach_response("logist_order.delete", delete_resp)
        assert_status(
            delete_resp,
            [204],
            context="DELETE /logist-order",
            bug_map={403: "#BUG-LOGIST-ORDER-DELETE-403"},
        )

    def test_logist_order_filters_and_all(
        self,
        authenticated_session: requests.Session,
        logist_order_cleanup,
    ):
        """Проверка фильтров и /all по созданному заказу (контракт: 200)."""
        unique = uuid.uuid4().hex[:6]
        payload = {
            "customer_name": f"CustomerF {unique}",
            "customer_contacts": "filter@example.com",
            "price": 111.0,
            "status": "Открыт",
        }
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=payload,
            timeout=15,
        )
        assert_status(
            create_resp,
            [201],
            context="POST /logist-order (filters setup)",
            bug_map={403: "#BUG-LOGIST-ORDER-CREATE-403"},
        )
        order_id = create_resp.json().get("id") if create_resp.status_code == 201 else None
        logist_order_cleanup(order_id)
        if not order_id:
            pytest.skip("Не удалось создать заказ для фильтров")

        # GET с order_id
        resp_get = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"order_id": order_id},
            timeout=10,
        )
        assert_status(
            resp_get,
            [200],
            context="GET /logist-order c order_id",
            bug_map={403: "#BUG-LOGIST-ORDER-GET-403"},
        )

        # GET /all
        resp_all = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/all",
            timeout=10,
        )
        assert_status(
            resp_all,
            [200],
            context="GET /logist-order/all",
            bug_map={403: "#BUG-LOGIST-ORDER-ALL-403"},
        )

        # GET со статусами (список)
        resp_status = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"status": ["Открыт"]},
            timeout=10,
        )
        assert_status(
            resp_status,
            [200],
            context="GET /logist-order c status filter",
            bug_map={403: "#BUG-LOGIST-ORDER-GET-403"},
        )


@pytest.mark.api
class TestLogistOrderStats:
    def test_logist_order_stats(self, authenticated_session: requests.Session):
        """Проверка /logist-order/stats (контракт: 200)."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/stats",
            timeout=10,
        )
        attach_response("logist_order.stats", resp)
        assert_status(
            resp,
            [200],
            context="GET /logist-order/stats",
            bug_map={403: "#BUG-LOGIST-ORDER-STATS-403"},
        )

    def test_logist_order_stats_all(self, authenticated_session: requests.Session):
        """Проверка /logist-order/stats/all (контракт: 200)."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/stats/all",
            timeout=10,
        )
        attach_response("logist_order.stats_all", resp)
        assert_status(
            resp,
            [200],
            context="GET /logist-order/stats/all",
            bug_map={403: "#BUG-LOGIST-ORDER-STATS-ALL-403"},
        )

    def test_logist_order_stats_excel_all(self, authenticated_session: requests.Session):
        """Проверка /logist-order/stats/excel/all (контракт: 200)."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/stats/excel/all",
            timeout=15,
        )
        attach_response("logist_order.stats_excel_all", resp)
        assert_status(
            resp,
            [200],
            context="GET /logist-order/stats/excel/all",
            bug_map={403: "#BUG-LOGIST-ORDER-STATS-EXCEL-ALL-403"},
        )

    def test_logist_order_stats_status_count(self, authenticated_session: requests.Session):
        """Проверка /logist-order/stats/status/count (контракт: 200)."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/stats/status/count",
            params={"period_days": 30},
            timeout=10,
        )
        attach_response("logist_order.stats_status_count", resp)
        assert_status(
            resp,
            [200],
            context="GET /logist-order/stats/status/count",
            bug_map={403: "#BUG-LOGIST-ORDER-STATS-STATUS-COUNT-403"},
        )

    def test_logist_order_get_all_standalone(
        self, authenticated_session: requests.Session
    ):
        """GET /logist-order/all — получение всех логистических заказов (отдельный тест)."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/all",
            timeout=10,
        )
        attach_response("logist_order.get_all_standalone", resp)
        assert_status(
            resp,
            [200],
            context="GET /logist-order/all",
            bug_map={403: "#BUG-LOGIST-ORDER-ALL-403"},
        )


@pytest.mark.api
class TestLogistOrderValidation:
    def test_create_logist_order_missing_required(
        self, authenticated_session: requests.Session
    ):
        """Негативный тест: отсутствие обязательных полей должно возвращать 422."""
        payload = {
            "customer_contacts": "no-name@test.com",
            # опущен customer_name и price
        }
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=payload,
            timeout=15,
        )
        attach_response("logist_order.create_invalid", resp)
        assert_status(
            resp,
            [422],
            context="POST /logist-order без обязательных полей",
            bug_map={201: "#BUG-LOGIST-ORDER-CREATE-INVALID-201"},
        )

    def test_create_logist_order_missing_price(
        self, authenticated_session: requests.Session
    ):
        """Негативный тест: отсутствие price (обязательно) → 422 по схеме."""
        payload = {
            "customer_name": "NoPrice",
            "customer_contacts": "noprice@example.com",
            # price отсутствует
        }
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=payload,
            timeout=15,
        )
        attach_response("logist_order.create_invalid_price", resp)
        assert_status(
            resp,
            [422],
            context="POST /logist-order без price",
            bug_map={201: "#BUG-LOGIST-ORDER-CREATE-INVALID-201"},
        )

    def test_logist_order_patch_missing_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: PATCH /logist-order без id → 422."""
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={},
            timeout=10,
        )
        attach_response("logist_order.patch_missing_id", resp)
        assert_status(
            resp,
            [422],
            context="PATCH /logist-order без id",
            bug_map={403: "#BUG-LOGIST-ORDER-PATCH-403"},
        )

