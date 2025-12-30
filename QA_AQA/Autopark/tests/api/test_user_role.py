"""
@file: test_user_role.py
@description: API тесты для модуля ролей пользователей (user-role)
@dependencies: pytest, requests
@created: 2025-12-11
@author: Directpi
@test_type: api

ВАЖНО: Несоответствие между БД и API:
- В БД (таблица user_roles): empty, logist, driver, mechanic, director (английские)
- В API запросах: 'Не назначен', 'Логист', 'Водитель', 'Механик', 'Директор' (русские)
API делает маппинг между русскими названиями в запросах и английскими в БД.
"""

from __future__ import annotations

from typing import Optional

import pytest
import requests

from tests.conftest import attach_response, assert_status

BASE_PATH = "/api/v1/user-role"


def _get_me_id(session: requests.Session) -> Optional[int]:
    """Получить ID текущего пользователя."""
    resp = session.get(f"{session.base_url}/api/v1/user/me", timeout=10)
    attach_response("user_role.me", resp)
    if resp.status_code == 200:
        return resp.json().get("id")
    return None


def _get_first_user_id(session: requests.Session) -> Optional[int]:
    """Получить первый доступный user_id для тестов."""
    try:
        resp = session.get(f"{session.base_url}/api/v1/user/all", timeout=10)
        attach_response("user_role.get_all_users", resp)
        if resp.status_code == 200:
            items = resp.json() or []
            if isinstance(items, list) and items:
                first = items[0]
                return first.get("id")
    except Exception:
        pass
    return None


@pytest.mark.api
class TestUserRoleRead:
    def test_get_user_roles(
        self, authenticated_session: requests.Session
    ):
        """GET /user-role — получение ролей пользователя."""
        user_id = _get_me_id(authenticated_session)
        if not user_id:
            user_id = _get_first_user_id(authenticated_session)
        if not user_id:
            pytest.skip("Нет доступа к user_id для теста")

        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"user_id": user_id},
            timeout=10,
        )
        attach_response("user_role.get", resp)
        assert_status(
            resp,
            [200],
            context="GET /user-role",
            bug_map={
                403: "#BUG-USER-ROLE-GET-403",
                404: "#BUG-USER-ROLE-GET-404",
                422: "#BUG-USER-ROLE-GET-422",
            },
        )

    def test_get_user_roles_missing_user_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: GET /user-role без user_id → 422."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            timeout=10,
        )
        attach_response("user_role.get_missing_user_id", resp)
        assert_status(
            resp,
            [422],
            context="GET /user-role без user_id",
            bug_map={
                200: "#BUG-USER-ROLE-GET-MISSING-ID-200: Возвращает 200 вместо 422",
                400: "#BUG-USER-ROLE-GET-MISSING-ID-400",
            },
        )


@pytest.mark.api
class TestUserRoleUpdate:
    def test_set_user_role(
        self, authenticated_session: requests.Session
    ):
        """PATCH /user-role — назначение роли пользователю."""
        user_id = _get_me_id(authenticated_session)
        if not user_id:
            user_id = _get_first_user_id(authenticated_session)
        if not user_id:
            pytest.skip("Нет доступа к user_id для теста")

        # Пробуем назначить роль
        # API ожидает русские названия: 'Не назначен', 'Логист', 'Водитель', 'Механик', 'Директор'
        # В БД роли хранятся на английском: empty, logist, driver, mechanic, director
        # Если роль уже назначена, это нормально
        payload = {
            "user_id": user_id,
            "role": "Водитель",
        }
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=payload,
            timeout=10,
        )
        attach_response("user_role.set", resp)
        assert_status(
            resp,
            [201],
            context="PATCH /user-role",
            bug_map={
                200: "#BUG-USER-ROLE-SET-200",
                403: "#BUG-USER-ROLE-SET-403",
                404: "#BUG-USER-ROLE-SET-404",
                409: "#BUG-USER-ROLE-SET-409",
            },
        )

    def test_set_user_role_missing_fields(
        self, authenticated_session: requests.Session
    ):
        """Негативный: PATCH /user-role без обязательных полей → 422."""
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={},
            timeout=10,
        )
        attach_response("user_role.set_missing_fields", resp)
        assert_status(
            resp,
            [422],
            context="PATCH /user-role без обязательных полей",
            bug_map={
                201: "#BUG-USER-ROLE-SET-INVALID-201",
                400: "#BUG-USER-ROLE-SET-INVALID-400",
            },
        )

    def test_set_user_role_invalid_role(
        self, authenticated_session: requests.Session
    ):
        """Негативный: PATCH /user-role с невалидной ролью → 422."""
        user_id = _get_me_id(authenticated_session)
        if not user_id:
            user_id = _get_first_user_id(authenticated_session)
        if not user_id:
            pytest.skip("Нет доступа к user_id для теста")

        payload = {
            "user_id": user_id,
            "role": "invalid_role_12345",
        }
        resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=payload,
            timeout=10,
        )
        attach_response("user_role.set_invalid_role", resp)
        assert_status(
            resp,
            [422],
            context="PATCH /user-role с невалидной ролью",
            bug_map={
                201: "#BUG-USER-ROLE-SET-INVALID-ROLE-201",
                400: "#BUG-USER-ROLE-SET-INVALID-ROLE-400",
            },
        )


@pytest.mark.api
class TestUserRoleDelete:
    def test_delete_user_role(
        self, authenticated_session: requests.Session
    ):
        """DELETE /user-role — удаление роли у пользователя."""
        user_id = _get_me_id(authenticated_session)
        if not user_id:
            user_id = _get_first_user_id(authenticated_session)
        if not user_id:
            pytest.skip("Нет доступа к user_id для теста")

        # Сначала назначаем роль, чтобы потом удалить
        # API ожидает русские названия в запросах
        payload = {
            "user_id": user_id,
            "role": "Водитель",
        }
        set_resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=payload,
            timeout=10,
        )
        # Если роль уже была назначена, это нормально

        # Удаляем роль (в DELETE тоже русские названия)
        resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"user_id": user_id, "role": "Водитель"},
            timeout=10,
        )
        attach_response("user_role.delete", resp)
        assert_status(
            resp,
            [200],
            context="DELETE /user-role",
            bug_map={
                204: "#BUG-USER-ROLE-DELETE-204",
                403: "#BUG-USER-ROLE-DELETE-403",
                404: "#BUG-USER-ROLE-DELETE-404",
            },
        )

    def test_delete_user_role_missing_params(
        self, authenticated_session: requests.Session
    ):
        """Негативный: DELETE /user-role без параметров → 422."""
        resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            timeout=10,
        )
        attach_response("user_role.delete_missing_params", resp)
        assert_status(
            resp,
            [422],
            context="DELETE /user-role без параметров",
            bug_map={
                200: "#BUG-USER-ROLE-DELETE-MISSING-200",
                400: "#BUG-USER-ROLE-DELETE-MISSING-400",
            },
        )

    def test_delete_user_role_missing_role(
        self, authenticated_session: requests.Session
    ):
        """Негативный: DELETE /user-role без role → 422."""
        user_id = _get_me_id(authenticated_session)
        if not user_id:
            user_id = _get_first_user_id(authenticated_session)
        if not user_id:
            pytest.skip("Нет доступа к user_id для теста")

        resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"user_id": user_id},
            timeout=10,
        )
        attach_response("user_role.delete_missing_role", resp)
        assert_status(
            resp,
            [422],
            context="DELETE /user-role без role",
            bug_map={
                200: "#BUG-USER-ROLE-DELETE-MISSING-ROLE-200",
                400: "#BUG-USER-ROLE-DELETE-MISSING-ROLE-400",
            },
        )

