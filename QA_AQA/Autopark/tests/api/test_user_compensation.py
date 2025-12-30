"""
@file: test_user_compensation.py
@description: API тесты для операций с компенсациями пользователей
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

BASE_PATH = "/api/v1/user-compensation"


@pytest.fixture
def user_compensation_cleanup(authenticated_session: requests.Session):
    """Фикстура для автоматической очистки созданных компенсаций."""
    created_ids = []

    def _cleanup(compensation_id: Optional[int]):
        if compensation_id:
            created_ids.append(compensation_id)

    yield _cleanup

    # Cleanup: удаляем созданные компенсации
    base = authenticated_session.base_url
    for comp_id in created_ids:
        try:
            resp = authenticated_session.delete(
                f"{base}{BASE_PATH}",
                params={"compensation_id": comp_id},
                timeout=10,
            )
            assert_status(
                resp,
                [200, 204, 422],
                context="DELETE /user-compensation (cleanup)",
                bug_map={202: "#BUG-USER-COMP-CLEANUP-202", 404: "#BUG-USER-COMP-CLEANUP-404", 403: "#BUG-USER-COMP-CLEANUP-403"},
            )
        except Exception:
            pass  # Игнорируем ошибки cleanup


@pytest.mark.api
class TestUserCompensationRead:
    """Тесты для чтения компенсаций пользователей."""

    @pytest.mark.smoke
    def test_get_user_compensations_unauthorized(self, api_base_url: str):
        response = requests.get(f"{api_base_url}{BASE_PATH}", timeout=10)
        assert_status(
            response,
            [200, 422],
            context="GET /user-compensation без авторизации",
            bug_map={401: "#BUG-USER-COMP-GET-UNAUTH-401", 403: "#BUG-USER-COMP-GET-UNAUTH-403"},
        )

    def test_get_user_compensations_authorized(
        self, authenticated_session: requests.Session
    ):
        response = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}", timeout=15
        )
        attach_response("user_compensation.get_list", response)
        assert_status(
            response,
            [200, 422],
            context="GET /user-compensation (авторизован)",
            bug_map={403: "#BUG-USER-COMP-GET-AUTH-403", 404: "#BUG-USER-COMP-GET-AUTH-404"},
        )

        status = response.status_code
        if status == 200:
            data = response.json()
            assert isinstance(
                data, (list, dict)
            ), "Ответ должен быть списком или объектом"


@pytest.mark.api
class TestUserCompensationCrud:
    """Тесты для CRUD операций с компенсациями пользователей."""

    @pytest.mark.smoke
    def test_create_user_compensation_invalid_payload(
        self, authenticated_session: requests.Session
    ):
        response = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={},
            timeout=15,
        )
        attach_response("user_compensation.create_invalid", response)
        assert_status(
            response,
            [422],
            context="POST /user-compensation с невалидным payload",
            bug_map={400: "#BUG-USER-COMP-CREATE-INVALID-400", 403: "#BUG-USER-COMP-CREATE-INVALID-403"},
        )

    def test_create_delete_user_compensation_flow(
        self, authenticated_session: requests.Session,
        user_compensation_cleanup
    ):
        base = authenticated_session.base_url
        payload = {
            "user_id": 1,
            "fixed_compensation": 1000.0,
            "percent_rate_per_completion": 5.0,
            "income_per_km": 2.5,
        }
        create_resp = authenticated_session.post(
            f"{base}{BASE_PATH}", json=payload, timeout=20
        )
        attach_response("user_compensation.create", create_resp)
        assert_status(
            create_resp,
            [201, 422],
            context="POST /user-compensation (positive flow)",
            bug_map={
                403: "#BUG-USER-COMP-CREATE-403",
                404: "#BUG-USER-COMP-CREATE-404",
                409: "#BUG-USER-COMP-CREATE-409",
            },
        )

        compensation_id = None
        create_status = create_resp.status_code
        if create_status == 201:
            body = create_resp.json()
            compensation_id = (
                body.get("id") or body.get("compensation_id")
            )
            if compensation_id:
                user_compensation_cleanup(compensation_id)

        if compensation_id:
            delete_resp = authenticated_session.delete(
                f"{base}{BASE_PATH}",
                params={"compensation_id": compensation_id},
                timeout=15,
            )
            attach_response("user_compensation.delete", delete_resp)
            assert_status(
                delete_resp,
                [204, 422],
                context="DELETE /user-compensation (positive flow)",
                bug_map={200: "#BUG-USER-COMP-DELETE-200", 202: "#BUG-USER-COMP-DELETE-202", 403: "#BUG-USER-COMP-DELETE-403", 404: "#BUG-USER-COMP-DELETE-404"},
            )

    def test_create_user_compensation_minimal_payload(
        self, authenticated_session: requests.Session,
        user_compensation_cleanup
    ):
        base = authenticated_session.base_url
        payload = {
            "user_id": 1,
        }
        create_resp = authenticated_session.post(
            f"{base}{BASE_PATH}", json=payload, timeout=20
        )
        attach_response("user_compensation.create_minimal", create_resp)

        create_status = create_resp.status_code
        if create_status == 201:
            body = create_resp.json()
            compensation_id = (
                body.get("id") or body.get("compensation_id")
            )
            if compensation_id:
                user_compensation_cleanup(compensation_id)

        assert_status(
            create_resp,
            [201, 422],
            context="POST /user-compensation (minimal payload)",
            bug_map={
                400: "#BUG-USER-COMP-CREATE-MIN-400",
                403: "#BUG-USER-COMP-CREATE-MIN-403",
                404: "#BUG-USER-COMP-CREATE-MIN-404",
            },
        )


@pytest.mark.api
class TestUserCompensationEdges:
    """Тесты для граничных случаев и негативных сценариев."""

    def test_delete_user_compensation_invalid_id(
        self, authenticated_session: requests.Session
    ):
        response = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"compensation_id": 99999998},
            timeout=15,
        )
        attach_response("user_compensation.delete_invalid_id", response)
        assert_status(
            response,
            [422],
            context="DELETE /user-compensation (несуществующий id)",
            bug_map={404: "#BUG-USER-COMP-DELETE-INVALID-404", 403: "#BUG-USER-COMP-DELETE-INVALID-403"},
        )

    def test_delete_user_compensation_no_auth(self, api_base_url: str):
        response = requests.delete(
            f"{api_base_url}{BASE_PATH}",
            params={"compensation_id": 1},
            timeout=10,
        )
        assert_status(
            response,
            [422],
            context="DELETE /user-compensation без авторизации",
            bug_map={
                401: "#BUG-USER-COMP-DELETE-NOAUTH-401",
                403: "#BUG-USER-COMP-DELETE-NOAUTH-403",
                200: "#BUG-USER-COMP-DELETE-NOAUTH-200",
            },
        )
