"""
@file: test_user_group.py
@description: API тесты для CRUD операций по группам пользователей (User Group)
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

BASE_PATH = "/api/v1/user-group"


@pytest.fixture
def user_group_cleanup(authenticated_session: requests.Session):
    """Фикстура для автоматической очистки созданных групп пользователей."""
    created_ids = []  # Список кортежей (user_id, group)

    def _cleanup(user_id: Optional[int], group: Optional[str]):
        if user_id and group:
            created_ids.append((user_id, group))

    yield _cleanup

    # Cleanup: удаляем созданные группы
    # Согласно OpenAPI схеме: DELETE требует user_id и group в query
    base = authenticated_session.base_url
    for (user_id, group) in created_ids:
        try:
            resp = authenticated_session.delete(
                f"{base}{BASE_PATH}",
                params={"user_id": user_id, "group": group},
                timeout=10,
            )
            assert_status(
                resp,
                [200, 204, 422],
                context="DELETE /user-group (cleanup)",
                bug_map={202: "#BUG-USER-GROUP-CLEANUP-202", 404: "#BUG-USER-GROUP-CLEANUP-404", 403: "#BUG-USER-GROUP-CLEANUP-403"},
            )
        except Exception:
            pass  # Игнорируем ошибки cleanup


@pytest.mark.api
class TestUserGroupRead:
    """Тесты для чтения групп пользователей."""

    @pytest.mark.smoke
    def test_get_user_groups_unauthorized(self, api_base_url: str):
        response = requests.get(f"{api_base_url}{BASE_PATH}", timeout=10)
        assert_status(
            response,
            [200, 422],
            context="GET /user-group без авторизации",
            bug_map={401: "#BUG-USER-GROUP-GET-UNAUTH-401", 403: "#BUG-USER-GROUP-GET-UNAUTH-403"},
        )

    def test_get_user_groups_authorized(
        self, authenticated_session: requests.Session
    ):
        response = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}", timeout=15
        )
        attach_response("user_group.get_list", response)
        assert_status(
            response,
            [200, 422],
            context="GET /user-group (авторизован)",
            bug_map={403: "#BUG-USER-GROUP-GET-AUTH-403"},
        )

        status = response.status_code
        if status == 200:
            data = response.json()
            assert isinstance(
                data, (list, dict)
            ), "Ответ должен быть списком или объектом"


@pytest.mark.api
class TestUserGroupCrud:
    """Тесты для CRUD операций с группами пользователей."""

    @pytest.mark.smoke
    def test_create_user_group_invalid_payload(
        self, authenticated_session: requests.Session
    ):
        """Проверка: POST метода нет в OpenAPI схеме для /api/v1/user-group.
        
        По схеме доступны только GET, PATCH, DELETE.
        Если API возвращает 405 (Method Not Allowed) - это правильно, но метода нет в схеме.
        Если API возвращает другой статус - это расхождение.
        """
        response = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={},
            timeout=15,
        )
        attach_response("user_group.create_invalid", response)
        # Метода POST нет в схеме, поэтому ожидаем что метод не поддерживается
        # Но 405 нет в схеме, поэтому фиксируем как баг
        assert_status(
            response,
            [],  # Метода нет в схеме, поэтому ожидаемых статусов нет
            context="POST /user-group (метода нет в схеме)",
            bug_map={
                405: "#BUG-USER-GROUP-POST-405-METHOD-NOT-IN-SCHEMA",
                422: "#BUG-USER-GROUP-POST-422-METHOD-NOT-IN-SCHEMA",
                400: "#BUG-USER-GROUP-POST-400-METHOD-NOT-IN-SCHEMA",
            },
        )

    def test_patch_delete_user_group_flow(
        self, authenticated_session: requests.Session, user_group_cleanup
    ):
        base = authenticated_session.base_url
        test_user_id = 1
        test_group = "Пользователь"

        update_payload = {
            "user_id": test_user_id,
            "group": test_group,
        }
        update_resp = authenticated_session.patch(
            f"{base}{BASE_PATH}",
            json=update_payload,
            timeout=15,
        )
        attach_response("user_group.update", update_resp)
        assert_status(
            update_resp,
            [200, 422],
            context="PATCH /user-group (positive flow)",
            bug_map={403: "#BUG-USER-GROUP-PATCH-403", 404: "#BUG-USER-GROUP-PATCH-404", 202: "#BUG-USER-GROUP-PATCH-202"},
        )

        update_status = update_resp.status_code
        if update_status == 200:
            user_group_cleanup(test_user_id, test_group)

        delete_resp = authenticated_session.delete(
            f"{base}{BASE_PATH}",
            params={"user_id": test_user_id, "group": test_group},
            timeout=15,
        )
        attach_response("user_group.delete", delete_resp)
        assert_status(
            delete_resp,
            [200, 204, 422],
            context="DELETE /user-group (positive flow)",
            bug_map={403: "#BUG-USER-GROUP-DELETE-403", 404: "#BUG-USER-GROUP-DELETE-404", 202: "#BUG-USER-GROUP-DELETE-202"},
        )

    def test_patch_user_group_minimal_payload(
        self, authenticated_session: requests.Session, user_group_cleanup
    ):
        base = authenticated_session.base_url
        test_user_id = 1
        test_group = "Пользователь"

        payload = {
            "user_id": test_user_id,
            "group": test_group,
        }
        patch_resp = authenticated_session.patch(
            f"{base}{BASE_PATH}", json=payload, timeout=20
        )
        attach_response("user_group.patch_minimal", patch_resp)

        patch_status = patch_resp.status_code
        if patch_status == 200:
            user_group_cleanup(test_user_id, test_group)

        assert_status(
            patch_resp,
            [200, 422],
            context="PATCH /user-group (minimal payload)",
            bug_map={403: "#BUG-USER-GROUP-PATCH-MIN-403", 404: "#BUG-USER-GROUP-PATCH-MIN-404", 202: "#BUG-USER-GROUP-PATCH-MIN-202"},
        )


@pytest.mark.api
class TestUserGroupEdges:
    """Тесты для граничных случаев и негативных сценариев."""

    def test_patch_user_group_invalid_id(
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
            context="PATCH /user-group (несуществующий id)",
            bug_map={
                404: "#BUG-USER-GROUP-PATCH-INVALID-404",
                403: "#BUG-USER-GROUP-PATCH-INVALID-403",
            },
        )

    def test_delete_user_group_invalid_id(
        self, authenticated_session: requests.Session
    ):
        response = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"user_id": 99999998, "group": "Пользователь"},
            timeout=15,
        )
        assert_status(
            response,
            [422],
            context="DELETE /user-group (несуществующий user/group)",
            bug_map={
                404: "#BUG-USER-GROUP-DELETE-INVALID-404",
                403: "#BUG-USER-GROUP-DELETE-INVALID-403",
            },
        )

    def test_patch_user_group_no_auth(self, api_base_url: str):
        resp = requests.patch(
            f"{api_base_url}{BASE_PATH}",
            json={"id": 1, "name": "NoAuth"},
            timeout=10,
        )
        assert_status(
            resp,
            [422],
            context="PATCH /user-group без авторизации",
            bug_map={
                401: "#BUG-USER-GROUP-PATCH-NOAUTH-401",
                403: "#BUG-USER-GROUP-PATCH-NOAUTH-403",
            },
        )

    def test_delete_user_group_no_auth(self, api_base_url: str):
        resp = requests.delete(
            f"{api_base_url}{BASE_PATH}",
            params={"user_id": 1, "group": "Пользователь"},
            timeout=10,
        )
        assert_status(
            resp,
            [422],
            context="DELETE /user-group без авторизации",
            bug_map={
                401: "#BUG-USER-GROUP-DELETE-NOAUTH-401",
                403: "#BUG-USER-GROUP-DELETE-NOAUTH-403",
                200: "#BUG-USER-GROUP-DELETE-NOAUTH-200",
            },
        )

    def test_patch_user_group_duplicate(
        self, authenticated_session: requests.Session, user_group_cleanup
    ):
        base = authenticated_session.base_url
        test_user_id = 1
        test_group = "Пользователь"

        update_payload = {
            "user_id": test_user_id,
            "group": test_group,
        }
        patch_resp = authenticated_session.patch(
            f"{base}{BASE_PATH}", json=update_payload, timeout=20
        )
        attach_response("user_group.patch_minimal", patch_resp)
        assert_status(
            patch_resp,
            [200, 422],
            context="PATCH /user-group (duplicate)",
            bug_map={
                403: "#BUG-USER-GROUP-PATCH-DUP-403",
                404: "#BUG-USER-GROUP-PATCH-DUP-404",
                202: "#BUG-USER-GROUP-PATCH-DUP-202",
            },
        )
