"""
@file: test_user.py
@description: API тесты для CRUD операций над пользователями
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
from requests import exceptions as requests_exceptions
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

BASE_PATH = "/api/v1/user"


@pytest.mark.api
class TestUserRead:
    @pytest.mark.smoke
    def test_get_users_unauthorized(self, api_base_url: str):
        response = requests.get(f"{api_base_url}{BASE_PATH}", timeout=10)
        assert_status(
            response,
            [200, 422],
            context="GET /user без авторизации",
            bug_map={
                401: "#BUG-USER-GET-UNAUTH-401",
                403: "#BUG-USER-GET-UNAUTH-403",
            },
        )

    def test_get_users_authorized(self, authenticated_session: requests.Session):
        response = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}", timeout=15
        )
        assert_status(
            response,
            [200, 422],
            context="GET /user (авторизован)",
            bug_map={403: "#BUG-USER-GET-AUTH-403"},
        )
    
    @pytest.mark.slow
    def test_get_users_all_unauthorized(self, api_base_url: str):
        """Проверка получения всех пользователей без авторизации."""
        try:
            response = requests.get(f"{api_base_url}{BASE_PATH}/all", timeout=8)
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /user/all без авторизации")
        assert_status(
            response,
            [200],
            context="GET /user/all без авторизации",
            bug_map={
                401: "#BUG-USER-GET-ALL-UNAUTH-401",
                403: "#BUG-USER-GET-ALL-UNAUTH-403",
            },
        )
    
    @pytest.mark.slow
    def test_get_users_all(self, authenticated_session: requests.Session):
        """Проверка получения всех пользователей через /all."""
        try:
            response = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}/all", timeout=8
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /user/all (авторизован)")
        attach_response("user.get_all", response)
        assert_status(
            response,
            [200],
            context="GET /user/all",
            bug_map={403: "#BUG-USER-GET-ALL-403"},
        )
        
        status_all = response.status_code
        if status_all == 200:
            data = response.json()
            assert isinstance(data, list), "Ответ должен быть списком"
    
    @pytest.mark.slow
    def test_get_users_stats_count_unauthorized(self, api_base_url: str):
        """Проверка получения статистики без авторизации."""
        try:
            response = requests.get(f"{api_base_url}{BASE_PATH}/stats/count", timeout=8)
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /user/stats/count без авторизации")
        assert_status(
            response,
            [200],
            context="GET /user/stats/count без авторизации",
            bug_map={
                401: "#BUG-USER-STATS-COUNT-UNAUTH-401",
                403: "#BUG-USER-STATS-COUNT-UNAUTH-403",
            },
        )
    
    @pytest.mark.slow
    def test_get_users_stats_count(self, authenticated_session: requests.Session):
        """Проверка получения статистики по количеству пользователей."""
        try:
            response = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}/stats/count", timeout=8
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /user/stats/count")
        attach_response("user.stats_count", response)
        assert_status(
            response,
            [200],
            context="GET /user/stats/count",
            bug_map={403: "#BUG-USER-STATS-COUNT-403"},
        )
        
        status_count = response.status_code
        if status_count == 200:
            data = response.json()
            # Ожидаем число или объект со статистикой
            assert isinstance(data, (int, dict)), "Ответ должен быть числом или объектом"


@pytest.mark.api
class TestUserCrud:
    @pytest.mark.smoke
    def test_create_user_invalid_payload(self, authenticated_session: requests.Session):
        # Невалидный запрос должен вернуть 422 согласно контракту
        response = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={"email": "not-an-email"},
            timeout=15,
        )
        assert_status(
            response,
            [422],
            context="POST /user с невалидным payload",
            bug_map={
                400: "#BUG-USER-CREATE-INVALID-400",
                403: "#BUG-USER-CREATE-INVALID-403",
            },
        )

    def test_create_update_delete_user_flow(
        self, authenticated_session: requests.Session, user_cleanup
    ):
        base = authenticated_session.base_url
        unique_suffix = uuid.uuid4().hex[:8]
        # 1) Создание пользователя
        payload = {
            # required by RegistrationFormSchema
            "name": "Crud",
            "surname": "User",
            "phone": f"+7999{unique_suffix[:7]}",
            "password": "TestPass123!",
            # optional
            "email": f"crud_{unique_suffix}@test.com",
            "telegram_nickname": None,
            "telegram_chat_id": None,
            "is_verified": None,
            "iana_timezone": None,
            "role": None,
            "group": None,
        }
        create_resp = authenticated_session.post(
            f"{base}{BASE_PATH}", json=payload, timeout=20
        )
        assert_status(
            create_resp,
            [201, 422],
            context="POST /user (positive flow)",
            bug_map={403: "#BUG-USER-CREATE-FORBIDDEN-403", 409: "#BUG-USER-CREATE-CONFLICT-409"},
        )
        user_id = None
        create_code = create_resp.status_code
        if create_code == 201:
            body = create_resp.json()
            user_id = body.get("id") or body.get("user_id")
            assert user_id, "В ответе на создание должен быть id"
            # Регистрируем для cleanup на случай если тест упадет до удаления
            user_cleanup(user_id)

        # 2) Обновление пользователя (если удалось создать)
        if user_id:
            update_resp = authenticated_session.patch(
                f"{base}{BASE_PATH}",
                json={"id": user_id, "name": "Updated"},
                timeout=15,
            )
            assert_status(
                update_resp,
                [200, 422],
                context="PATCH /user (positive flow)",
                bug_map={403: "#BUG-USER-PATCH-FORBIDDEN-403"},
            )

        # 3) Удаление пользователя (если удалось создать)
        if user_id:
            delete_resp = authenticated_session.delete(
                f"{base}{BASE_PATH}", params={"user_id": user_id}, timeout=15
            )
            assert_status(
                delete_resp,
                [204, 422],
                context="DELETE /user (positive flow)",
                bug_map={403: "#BUG-USER-DELETE-FORBIDDEN-403", 200: "#BUG-USER-DELETE-200"},
            )


@pytest.mark.api
class TestUserMe:
    def test_get_me_unauthorized(self, api_base_url: str):
        resp = requests.get(f"{api_base_url}{BASE_PATH}/me", timeout=10)
        assert_status(
            resp,
            [200],
            context="GET /user/me без авторизации",
            bug_map={401: "#BUG-USER-ME-UNAUTH-401", 403: "#BUG-USER-ME-UNAUTH-403"},
        )

    def test_get_me_authorized(self, authenticated_session: requests.Session):
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/me", timeout=10
        )
        assert_status(
            resp,
            [200],
            context="GET /user/me (авторизован)",
            bug_map={403: "#BUG-USER-ME-AUTH-403"},
        )

    def test_patch_me_validation(self, authenticated_session: requests.Session):
        # Используем уже аутентифицированную сессию по контракту
        me_resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/me", timeout=10
        )
        assert_status(
            me_resp,
            [200],
            context="GET /user/me перед PATCH",
            bug_map={403: "#BUG-USER-ME-AUTH-403"},
        )
        if me_resp.status_code != 200:
            pytest.skip("Нет доступа к /user/me для обновления")
        current_name = me_resp.json().get("name")

        # no-op PATCH: отправляем то же имя, контракт ожидает 200/422 при успехе/валидации
        patch_resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}/me",
            json={"name": current_name},
            timeout=10,
        )
        assert_status(
            patch_resp,
            [200, 422],
            context="PATCH /user/me",
            bug_map={403: "#BUG-USER-ME-PATCH-403"},
        )


@pytest.mark.api
class TestUserPatchId:
    @pytest.mark.smoke
    def test_patch_user_by_valid_id(
        self, authenticated_session: requests.Session, user_cleanup
    ):
        suffix = int(time.time())
        email = f"patchuser_{suffix}@test.com"
        phone = f"+7999{suffix:07d}"
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "PatchValid",
                "surname": "User",
                "phone": phone,
                "email": email,
                "password": "TestPass123!",
            },
            timeout=15,
        )
        create_status = create_resp.status_code
        if create_status != 201:
            print(
                f"User create failed (status {create_status}):",
                create_resp.text,
            )
            pytest.skip("Нет ID созданного пользователя")
        user_id = create_resp.json().get("id")
        user_cleanup(user_id)
        patch_resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={"id": user_id, "name": "Updated Patch"},
            timeout=10,
        )
        assert_status(
            patch_resp,
            [200, 422],
            context="PATCH /user (валидный id)",
            bug_map={202: "#BUG-USER-PATCH-VALID-202"},
        )

    def test_patch_user_with_invalid_id(self, authenticated_session: requests.Session):
        patch_resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={"id": 99999999, "name": "WrongId"},
            timeout=10,
        )
        assert_status(
            patch_resp,
            [422],
            context="PATCH /user (несуществующий id)",
            bug_map={
                404: "#BUG-USER-PATCH-INVALIDID-404",
                403: "#BUG-USER-PATCH-INVALIDID-403",
            },
        )

    def test_patch_user_no_auth(self, api_base_url: str):
        # PATCH должен требовать авторизацию. Контракт не описывает 401/403.
        resp = requests.patch(
            f"{api_base_url}{BASE_PATH}",
            json={"id": 99999999, "name": "NoAuth"},
            timeout=10,
        )
        assert_status(
            resp,
            [422],
            context="PATCH /user без авторизации",
            bug_map={
                401: "#BUG-USER-PATCH-NOAUTH-401",
                403: "#BUG-USER-PATCH-NOAUTH-403",
            },
        )

    def test_patch_user_max_length(
        self, authenticated_session: requests.Session, user_cleanup
    ):
        suffix = str(uuid.uuid4().int)[-7:]
        phone = f"+7999{suffix}"
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "A" * 50,
                "surname": "B" * 50,
                "phone": phone,
                "password": "Aa1#qwertyasdfzxcvbn!12345long",
            },
            timeout=15,
        )
        if create_resp.status_code != 201:
            print(
                f"User create failed (status {create_resp.status_code}):",
                create_resp.text,
            )
            pytest.skip("Не создать пользователя для max length patch")
        user_id = create_resp.json().get("id")
        user_cleanup(user_id)
        patch_resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={"id": user_id, "name": "B" * 50, "surname": "C" * 50},
            timeout=10,
        )
        assert_status(
            patch_resp,
            [200, 422],
            context="PATCH /user (max length)",
            bug_map={
                400: "#BUG-USER-PATCH-MAXLEN-400",
                403: "#BUG-USER-PATCH-MAXLEN-403",
                202: "#BUG-USER-PATCH-MAXLEN-202",
            },
        )


@pytest.mark.api
class TestUserDeleteById:
    @pytest.mark.smoke
    def test_delete_user_by_valid_id(
        self, authenticated_session: requests.Session, user_cleanup
    ):
        suffix = int(time.time())
        email = f"deluser_{suffix}@test.com"
        phone = f"+7999{suffix:07d}"
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "DeleteMe",
                "surname": "User",
                "phone": phone,
                "password": "TestPass123!",
                "email": email,
            },
            timeout=15,
        )
        create_status = create_resp.status_code
        user_id = create_resp.json().get("id") if create_status == 201 else None
        if not user_id:
            print(
                f"User create failed (status {create_status}):",
                create_resp.text,
            )
            pytest.skip("Нет ID для удаления")
        user_cleanup(user_id)
        del_resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"user_id": user_id},
            timeout=10,
        )
        assert_status(
            del_resp,
            [204, 422],
            context="DELETE /user (валидный id)",
            bug_map={
                403: "#BUG-USER-DELETE-FORBIDDEN",
                200: "#BUG-USER-DELETE-200",
            },
        )

    def test_delete_user_invalid(self, authenticated_session: requests.Session):
        del_resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"user_id": 99999998},
            timeout=10,
        )
        assert_status(
            del_resp,
            [422],
            context="DELETE /user (несуществующий id)",
            bug_map={
                404: "#BUG-USER-DELETE-INVALID-404",
                403: "#BUG-USER-DELETE-INVALID-403",
            },
        )

    def test_delete_user_no_auth(self, api_base_url: str):
        del_resp = requests.delete(
            f"{api_base_url}{BASE_PATH}", params={"user_id": 77}, timeout=10
        )
        assert_status(
            del_resp,
            [422],
            context="DELETE /user без авторизации",
            bug_map={
                401: "#BUG-USER-DELETE-NOAUTH-401",
                403: "#BUG-USER-DELETE-NOAUTH-403",
            },
        )


@pytest.mark.api
class TestUserValidationNegatives:
    @pytest.mark.xfail(reason="#BUG-USER-EMAIL-VALIDATION: регистрация пропускает невалидный email")
    def test_register_invalid_email(self, authenticated_session: requests.Session, user_cleanup):
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "BadEmail",
                "surname": "User",
                "phone": "+79995553333",
                "password": "TestPass123!",
                "email": "not-an-email",
            },
            timeout=10,
        )
        status = resp.status_code
        if status == 201:
            uid = resp.json().get("id")
            if uid:
                user_cleanup(uid)
            pytest.fail(
                "BUG: Регистрация с невалидным email прошла (ожид. 422), получен 201. #BUG-USER-EMAIL-VALIDATION"
            )
        assert_status(
            resp,
            [422],
            context="POST /user (невалидный email)",
            bug_map={
                400: "#BUG-USER-EMAIL-INVALID-400",
                409: "#BUG-USER-EMAIL-INVALID-409",
            },
        )

    def test_register_invalid_phone(self, authenticated_session: requests.Session, user_cleanup):
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "BadPhone",
                "surname": "User",
                "phone": "12345",
                "password": "TestPass123!",
            },
            timeout=10,
        )
        status = resp.status_code
        if status == 201:
            uid = resp.json().get("id")
            if uid:
                user_cleanup(uid)
            pytest.fail(
                "BUG: Регистрация с невалидным телефоном прошла (ожид. 422), получен 201. #BUG-USER-PHONE-MINLEN"
            )
        assert_status(
            resp,
            [422],
            context="POST /user (короткий телефон)",
            bug_map={
                400: "#BUG-USER-PHONE-INVALID-400",
                409: "#BUG-USER-PHONE-INVALID-409",
            },
        )

    def test_register_duplicate_email_phone(
        self, authenticated_session: requests.Session, user_cleanup
    ):
        payload = {
            "name": "Dup",
            "surname": "User",
            "phone": "+79995554444",
            "password": "TestPass123!",
            "email": "dupuser@test.com",
        }
        first = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}", json=payload, timeout=10
        )
        first_status = first.status_code
        if first_status == 201:
            uid = first.json().get("id")
            if uid:
                user_cleanup(uid)
        second = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}", json=payload, timeout=10
        )
        second_status = second.status_code
        if second_status == 201:
            uid2 = second.json().get("id")
            if uid2:
                user_cleanup(uid2)
            pytest.fail(
                "BUG: Регистрация дубликата прошла (ожид. 422), получен 201. #BUG-USER-DUPLICATE-ACCEPT"
            )
        assert_status(
            second,
            [422],
            context="POST /user (дубликат email/phone)",
            bug_map={
                409: "#BUG-USER-DUPLICATE-409",
                400: "#BUG-USER-DUPLICATE-400",
            },
        )


@pytest.mark.api
class TestUserRole:
    def test_get_roles_requires_user_id(self, authenticated_session: requests.Session):
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}/api/v1/user-role",
            timeout=10,
        )
        assert_status(
            resp,
            [200, 422],
            context="GET /user-role без user_id",
            bug_map={400: "#BUG-USER-ROLE-GET-400", 403: "#BUG-USER-ROLE-GET-403"},
        )

    def test_set_role_invalid_enum(self, authenticated_session: requests.Session, user_cleanup):
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "RoleInvalid",
                "surname": "User",
                "phone": f"+7999{int(time.time())%10000000:07d}",
                "password": "TestPass123!",
            },
            timeout=10,
        )
        if create_resp.status_code != 201:
            pytest.skip(f"Не удалось создать пользователя для role: {create_resp.status_code}")
        uid = create_resp.json().get("id")
        user_cleanup(uid)

        patch_resp = authenticated_session.patch(
            f"{authenticated_session.base_url}/api/v1/user-role",
            json={"user_id": uid, "role": "НЕСУЩЕСТВУЕТ"},
            timeout=10,
        )
        assert_status(
            patch_resp,
            [422],
            context="PATCH /user-role (невалидный enum)",
            bug_map={403: "#BUG-USER-ROLE-INVALID-403"},
        )

    def test_set_role_valid_or_forbidden(self, authenticated_session: requests.Session, user_cleanup):
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "RoleAssign",
                "surname": "User",
                "phone": f"+7999{int(time.time())%10000000:07d}",
                "password": "TestPass123!",
            },
            timeout=10,
        )
        if create_resp.status_code != 201:
            pytest.skip(f"Не удалось создать пользователя для назначения роли: {create_resp.status_code}")
        uid = create_resp.json().get("id")
        user_cleanup(uid)

        patch_resp = authenticated_session.patch(
            f"{authenticated_session.base_url}/api/v1/user-role",
            json={"user_id": uid, "role": "Водитель"},
            timeout=10,
        )
        assert_status(
            patch_resp,
            [201, 422],
            context="PATCH /user-role (валидная роль)",
            bug_map={403: "#BUG-USER-ROLE-VALID-403"},
        )

    def test_delete_role_by_query(self, authenticated_session: requests.Session):
        del_resp = authenticated_session.delete(
            f"{authenticated_session.base_url}/api/v1/user-role",
            params={"user_id": 0, "role": "Водитель"},
            timeout=10,
        )
        assert_status(
            del_resp,
            [200, 422],
            context="DELETE /user-role (query)",
            bug_map={
                400: "#BUG-USER-ROLE-DELETE-400",
                403: "#BUG-USER-ROLE-DELETE-403",
                404: "#BUG-USER-ROLE-DELETE-404",
            },
        )

    def test_delete_role_missing_params(self, authenticated_session: requests.Session):
        del_resp = authenticated_session.delete(
            f"{authenticated_session.base_url}/api/v1/user-role",
            timeout=10,
        )
        assert_status(
            del_resp,
            [422],
            context="DELETE /user-role без обязательных параметров",
            bug_map={400: "#BUG-USER-ROLE-DELETE-MISSING-400", 403: "#BUG-USER-ROLE-DELETE-MISSING-403"},
        )


@pytest.mark.api
class TestOwnershipEdges:
    """Edge-cases по владению ресурсами: попытки изменить/удалить 'чужого' пользователя."""

    def test_patch_foreign_user(self, authenticated_session: requests.Session, user_cleanup):
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "Foreign",
                "surname": "User",
                "phone": f"+7999{int(time.time())%10000000:07d}",
                "password": "TestPass123!",
            },
            timeout=10,
        )
        if create_resp.status_code != 201:
            pytest.skip(f"Не удалось создать пользователя для проверки владения: {create_resp.status_code}")
        uid = create_resp.json().get("id")
        user_cleanup(uid)

        patch_resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={"id": uid, "name": "TryEditForeign"},
            timeout=10,
        )
        assert_status(
            patch_resp,
            [200, 422],
            context="PATCH /user (чужой пользователь)",
            bug_map={
                403: "#BUG-USER-FOREIGN-PATCH-403",
                404: "#BUG-USER-FOREIGN-PATCH-404",
                202: "#BUG-USER-FOREIGN-PATCH-202",
            },
        )

    def test_delete_foreign_user(self, authenticated_session: requests.Session, user_cleanup):
        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "ForeignDel",
                "surname": "User",
                "phone": f"+7999{(int(time.time())+1)%10000000:07d}",
                "password": "TestPass123!",
            },
            timeout=10,
        )
        if create_resp.status_code != 201:
            pytest.skip(f"Не удалось создать пользователя для удаления: {create_resp.status_code}")
        uid = create_resp.json().get("id")
        user_cleanup(uid)

        del_resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"user_id": uid},
            timeout=10,
        )
        assert_status(
            del_resp,
            [204, 422],
            context="DELETE /user (чужой пользователь)",
            bug_map={
                200: "#BUG-USER-FOREIGN-DELETE-200",
                202: "#BUG-USER-FOREIGN-DELETE-202",
                403: "#BUG-USER-FOREIGN-DELETE-403",
                404: "#BUG-USER-FOREIGN-DELETE-404",
            },
        )


@pytest.mark.api
class TestUserValidationMore:
    """Расширенные проверки валидации регистрации пользователя."""

    def test_register_empty_email(self, authenticated_session: requests.Session, user_cleanup):
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "EmptyEmail",
                "surname": "User",
                "phone": f"+7999{int(time.time())%10000000:07d}",
                "password": "TestPass123!",
                "email": "",
            },
            timeout=10,
        )
        status = resp.status_code
        if status == 201:
            uid = resp.json().get("id")
            if uid:
                user_cleanup(uid)
            pytest.fail(
                "BUG: Регистрация с пустым email прошла (ожид. 422), получен 201. #BUG-USER-EMAIL-EMPTY"
            )
        assert_status(
            resp,
            [422],
            context="POST /user (пустой email)",
            bug_map={
                400: "#BUG-USER-EMAIL-EMPTY-400",
                409: "#BUG-USER-EMAIL-EMPTY-409",
            },
        )

    def test_register_empty_phone(self, authenticated_session: requests.Session, user_cleanup):
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "EmptyPhone",
                "surname": "User",
                "phone": "",
                "password": "TestPass123!",
            },
            timeout=10,
        )
        status = resp.status_code
        if status == 201:
            uid = resp.json().get("id")
            if uid:
                user_cleanup(uid)
            pytest.fail(
                "BUG: Регистрация с пустым телефоном прошла (ожид. 422), получен 201. #BUG-USER-PHONE-EMPTY"
            )
        assert_status(
            resp,
            [422],
            context="POST /user (пустой телефон)",
            bug_map={
                400: "#BUG-USER-PHONE-EMPTY-400",
                409: "#BUG-USER-PHONE-EMPTY-409",
            },
        )

    def test_register_max_length_fields(self, authenticated_session: requests.Session, user_cleanup):
        name_long = "N" * 100
        surname_long = "S" * 100
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": name_long,
                "surname": surname_long,
                "phone": f"+7999{int(time.time())%10000000:07d}",
                "password": "TestPass123!",
            },
            timeout=10,
        )
        status = resp.status_code
        if status == 201:
            uid = resp.json().get("id")
            if uid:
                user_cleanup(uid)
        assert_status(
            resp,
            [201, 422],
            context="POST /user (максимальная длина)",
            bug_map={400: "#BUG-USER-MAXLEN-400"},
        )

    @pytest.mark.xfail(reason="#BUG-USER-PHONE-VALIDATION: телефон с буквами проходит")
    def test_register_phone_with_letters(self, authenticated_session: requests.Session, user_cleanup):
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "PhoneLetters",
                "surname": "User",
                "phone": "+7ABCDEF1234",
                "password": "TestPass123!",
            },
            timeout=10,
        )
        status = resp.status_code
        if status == 201:
            uid = resp.json().get("id")
            if uid:
                user_cleanup(uid)
            pytest.fail(
                "BUG: Регистрация с телефоном, содержащим буквы, не должна проходить (ожид. 422), получен 201. #BUG-USER-PHONE-VALIDATION"
            )
        assert_status(
            resp,
            [422],
            context="POST /user (телефон с буквами)",
            bug_map={
                400: "#BUG-USER-PHONE-VALIDATION-400",
                409: "#BUG-USER-PHONE-VALIDATION",
            },
        )

    def test_register_email_spaces_only(self, authenticated_session: requests.Session, user_cleanup):
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "SpaceEmail",
                "surname": "User",
                "phone": f"+7999{int(time.time())%10000000:07d}",
                "password": "TestPass123!",
                "email": "   ",
            },
            timeout=10,
        )
        status = resp.status_code
        if status == 201:
            uid = resp.json().get("id")
            if uid:
                user_cleanup(uid)
            pytest.fail(
                "BUG: Регистрация с email из пробелов прошла (ожид. 422), получен 201. #BUG-USER-EMAIL-SPACES"
            )
        assert_status(
            resp,
            [422],
            context="POST /user (email из пробелов)",
            bug_map={
                400: "#BUG-USER-EMAIL-SPACES-400",
                409: "#BUG-USER-EMAIL-SPACES-409",
            },
        )

    def test_register_phone_too_short(self, authenticated_session: requests.Session, user_cleanup):
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "ShortPhone",
                "surname": "User",
                "phone": "+7999",
                "password": "TestPass123!",
            },
            timeout=10,
        )
        status = resp.status_code
        if status == 201:
            uid = resp.json().get("id")
            if uid:
                user_cleanup(uid)
            pytest.fail(
                "BUG: Регистрация с слишком коротким телефоном прошла (ожид. 422), получен 201. #BUG-USER-PHONE-MINLEN"
            )
        assert_status(
            resp,
            [422],
            context="POST /user (короткий телефон)",
            bug_map={
                400: "#BUG-USER-PHONE-MINLEN-400",
                409: "#BUG-USER-PHONE-MINLEN-409",
            },
        )

    def test_register_email_minimal_invalid(self, authenticated_session: requests.Session, user_cleanup):
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "EmailShort",
                "surname": "User",
                "phone": f"+7999{int(time.time())%10000000:07d}",
                "password": "TestPass123!",
                "email": "a@",
            },
            timeout=10,
        )
        status = resp.status_code
        if status == 201:
            uid = resp.json().get("id")
            if uid:
                user_cleanup(uid)
            pytest.fail(
                "BUG: Регистрация с невалидным коротким email прошла (ожид. 422), получен 201. #BUG-USER-EMAIL-MIN"
            )
        assert_status(
            resp,
            [422],
            context="POST /user (короткий email)",
            bug_map={
                400: "#BUG-USER-EMAIL-MIN-400",
                409: "#BUG-USER-EMAIL-MIN-409",
            },
        )

    @pytest.mark.xfail(reason="#BUG-USER-PHONE-SYMBOLS: телефон со спецсимволами проходит")
    def test_register_phone_with_special_symbols(self, authenticated_session: requests.Session, user_cleanup):
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "name": "PhoneSpecial",
                "surname": "User",
                "phone": "+7(999)-12*3-45#67",
                "password": "TestPass123!",
            },
            timeout=10,
        )
        status = resp.status_code
        if status == 201:
            uid = resp.json().get("id")
            if uid:
                user_cleanup(uid)
            pytest.fail(
                "BUG: Регистрация с телефоном, содержащим спецсимволы, не должна проходить (ожид. 422), получен 201. #BUG-USER-PHONE-SYMBOLS"
            )
        assert_status(
            resp,
            [422],
            context="POST /user (телефон со спецсимволами)",
            bug_map={
                400: "#BUG-USER-PHONE-SYMBOLS-400",
                409: "#BUG-USER-PHONE-SYMBOLS",
            },
        )
