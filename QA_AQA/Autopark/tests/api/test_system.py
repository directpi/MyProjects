"""
@file: test_system.py
@description: API тесты для системных эндпоинтов (system)
@dependencies: pytest, requests
@created: 2025-12-11
@author: Directpi
@test_type: api
"""

import pytest
import requests

from tests.conftest import attach_response, assert_status

BASE_PATH = "/api/v1/system"


@pytest.mark.api
class TestSystemEndpoints:
    """Тесты для системных эндпоинтов согласно OpenAPI схеме."""

    @pytest.mark.xfail(reason="Эндпоинт GET /system/healthcheck отсутствует в OpenAPI схеме")
    def test_system_healthcheck(self, api_base_url: str):
        """GET /system/healthcheck — проверка здоровья системы.
        
        ПРИМЕЧАНИЕ: Эндпоинт отсутствует в OpenAPI схеме, тест помечен как xfail.
        """
        resp = requests.get(
            f"{api_base_url}{BASE_PATH}/healthcheck",
            timeout=10,
        )
        attach_response("system.healthcheck", resp)
        assert_status(
            resp,
            [200, 404],
            context="GET /system/healthcheck (не в схеме)",
            bug_map={},
        )

    @pytest.mark.xfail(reason="Эндпоинт GET /system/info отсутствует в OpenAPI схеме")
    def test_system_info(self, authenticated_session: requests.Session):
        """GET /system/info — информация о системе.
        
        ПРИМЕЧАНИЕ: Эндпоинт отсутствует в OpenAPI схеме, тест помечен как xfail.
        """
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/info",
            timeout=10,
        )
        attach_response("system.info", resp)
        assert_status(
            resp,
            [200, 404],
            context="GET /system/info (не в схеме)",
            bug_map={},
        )

    @pytest.mark.xfail(reason="Эндпоинт GET /system/info отсутствует в OpenAPI схеме")
    def test_system_info_unauthorized(self, api_base_url: str):
        """GET /system/info без авторизации.
        
        ПРИМЕЧАНИЕ: Эндпоинт отсутствует в OpenAPI схеме, тест помечен как xfail.
        """
        resp = requests.get(
            f"{api_base_url}{BASE_PATH}/info",
            timeout=10,
        )
        attach_response("system.info_unauthorized", resp)
        assert_status(
            resp,
            [200, 401, 404],
            context="GET /system/info без авторизации (не в схеме)",
            bug_map={},
        )

    def test_system_integration(self, authenticated_session: requests.Session):
        """GET /system/integration — информация об интеграциях."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/integration",
            timeout=10,
        )
        attach_response("system.integration", resp)
        assert_status(
            resp,
            [200],
            context="GET /system/integration",
            bug_map={
                403: "#BUG-SYSTEM-INTEGRATION-403",
                404: "#BUG-SYSTEM-INTEGRATION-404",
            },
        )

    def test_system_integration_post(self, authenticated_session: requests.Session):
        """POST /system/integration — создание/обновление интеграции.
        
        По схеме: POST возвращает 201 (успешное создание) или 422 (ошибка валидации).
        """
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}/integration",
            json={},
            timeout=10,
        )
        attach_response("system.integration_post", resp)
        # По схеме: 201 (успех) или 422 (валидация)
        assert_status(
            resp,
            [201, 422],
            context="POST /system/integration",
            bug_map={
                403: "#BUG-SYSTEM-INTEGRATION-POST-403",
                404: "#BUG-SYSTEM-INTEGRATION-POST-404",
                200: "#BUG-SYSTEM-INTEGRATION-POST-200",
            },
        )

    def test_system_integration_check(self, authenticated_session: requests.Session):
        """POST /system/integration/check — проверка интеграции."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}/integration/check",
            json={},
            timeout=10,
        )
        attach_response("system.integration_check", resp)
        assert_status(
            resp,
            [200, 422],
            context="POST /system/integration/check",
            bug_map={
                403: "#BUG-SYSTEM-INTEGRATION-CHECK-403",
                404: "#BUG-SYSTEM-INTEGRATION-CHECK-404",
            },
        )

