"""
@file: test_driver_shift.py
@description: API тесты для модуля смен водителей (driver-shift)
@dependencies: pytest, requests
@created: 2025-12-11
@author: Directpi
@test_type: api
"""

from __future__ import annotations

import pytest
import requests
from requests import exceptions as requests_exceptions

from tests.conftest import attach_response, assert_status

BASE_PATH = "/api/v1/driver-shift"


@pytest.mark.api
class TestDriverShift:
    def test_driver_shift_get(self, authenticated_session: requests.Session):
        """GET /driver-shift — получение смен водителя."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            timeout=10,
        )
        attach_response("driver_shift.get", resp)
        assert_status(
            resp,
            [200],
            bug_map={403: "#BUG-DRIVER-SHIFT-GET-403"},
        )

    @pytest.mark.slow
    def test_driver_shift_get_all(self, authenticated_session: requests.Session):
        """GET /driver-shift/all — получение всех смен."""
        try:
            resp = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}/all",
                timeout=8,
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /driver-shift/all")
        attach_response("driver_shift.get_all", resp)
        assert_status(
            resp,
            [200],
            bug_map={403: "#BUG-DRIVER-SHIFT-ALL-403"},
        )

    def test_driver_shift_my_active(self, authenticated_session: requests.Session):
        """GET /driver-shift/my-active — активная смена текущего водителя."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/my-active",
            timeout=10,
        )
        attach_response("driver_shift.my_active", resp)
        assert_status(
            resp,
            [200, 404],
            context="GET /driver-shift/my-active",
            bug_map={403: "#BUG-DRIVER-SHIFT-MY-ACTIVE-403"},
        )

    def test_driver_shift_create_invalid(
        self, authenticated_session: requests.Session
    ):
        """Негативный: POST /driver-shift без данных → 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={},  # отсутствие обязательных полей
            timeout=10,
        )
        attach_response("driver_shift.create_invalid", resp)
        assert_status(
            resp,
            [422],
            context="POST /driver-shift без данных",
            bug_map={403: "#BUG-DRIVER-SHIFT-CREATE-403"},
        )

    def test_driver_shift_patch_missing_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: PATCH /driver-shift без id → 422."""
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={},
            timeout=10,
        )
        attach_response("driver_shift.patch_missing_id", resp)
        assert_status(
            resp,
            [422],
            context="PATCH /driver-shift без id",
            bug_map={403: "#BUG-DRIVER-SHIFT-PATCH-403"},
        )

    def test_driver_shift_delete_missing_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: DELETE /driver-shift без id → 422."""
        resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            timeout=10,
        )
        attach_response("driver_shift.delete_missing_id", resp)
        assert_status(
            resp,
            [422],
            context="DELETE /driver-shift без id",
            bug_map={403: "#BUG-DRIVER-SHIFT-DELETE-403"},
        )









