"""
@file: test_glonass.py
@description: API тесты для модуля интеграции с GLONASS
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

BASE_PATH = "/api/v1/glonass"


@pytest.mark.api
class TestGlonass:
    def test_glonass_user(self, authenticated_session: requests.Session):
        """GET /glonass/user — получение пользователя GLONASS."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/user",
            timeout=10,
        )
        attach_response("glonass.user", resp)
        if resp.status_code == 500:
            pytest.xfail("#BUG-GLONASS-USER-500: Internal Server Error")
        assert_status(
            resp,
            [200],
            context="GET /glonass/user",
            bug_map={
                403: "#BUG-GLONASS-USER-403",
                404: "#BUG-GLONASS-USER-404",
            },
        )

    def test_glonass_vehicles(self, authenticated_session: requests.Session):
        """GET /glonass/vehicles — получение ТС из GLONASS."""
        try:
            resp = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}/vehicles",
                timeout=8,
            )
        except (requests_exceptions.Timeout, requests_exceptions.ConnectionError):
            pytest.skip("GLONASS недоступен/перегружен: vehicles")
        attach_response("glonass.vehicles", resp)
        if resp.status_code == 500:
            pytest.xfail("#BUG-GLONASS-VEHICLES-500: Internal Server Error")
        assert_status(
            resp,
            [200],
            context="GET /glonass/vehicles",
            bug_map={
                403: "#BUG-GLONASS-VEHICLES-403",
                404: "#BUG-GLONASS-VEHICLES-404",
            },
        )

    def test_glonass_vehicle_mileage(
        self, authenticated_session: requests.Session
    ):
        """GET /glonass/vehicle-mileage — пробег ТС из GLONASS."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/vehicle-mileage",
            params={
                "glonass_id": 1,
                "from_datetime": "2025-01-01T00:00:00",
                "to_datetime": "2025-12-31T23:59:59",
            },
            timeout=10,
        )
        attach_response("glonass.vehicle_mileage", resp)
        if resp.status_code == 500:
            pytest.xfail("#BUG-GLONASS-MILEAGE-500: Internal Server Error")
        assert_status(
            resp,
            [200, 404],
            context="GET /glonass/vehicle-mileage",
            bug_map={
                403: "#BUG-GLONASS-MILEAGE-403",
                422: "#BUG-GLONASS-MILEAGE-422",
            },
        )

    def test_glonass_vehicle_mileage_excel(
        self, authenticated_session: requests.Session
    ):
        """GET /glonass/vehicle-mileage/excel — экспорт пробега ТС в Excel."""
        try:
            resp = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}/vehicle-mileage/excel",
                params={
                    "glonass_id": 1,
                    "from_datetime": "2025-01-01T00:00:00",
                    "to_datetime": "2025-12-31T23:59:59",
                },
                timeout=8,
            )
        except requests_exceptions.Timeout:
            pytest.skip("GLONASS перегружен: таймаут vehicle-mileage/excel")
        attach_response("glonass.vehicle_mileage_excel", resp)
        if resp.status_code == 500:
            pytest.xfail("#BUG-GLONASS-MILEAGE-EXCEL-500: Internal Server Error")
        assert_status(
            resp,
            [200],
            context="GET /glonass/vehicle-mileage/excel",
            bug_map={
                403: "#BUG-GLONASS-MILEAGE-EXCEL-403",
                404: "#BUG-GLONASS-MILEAGE-EXCEL-404",
                422: "#BUG-GLONASS-MILEAGE-EXCEL-422",
            },
        )

