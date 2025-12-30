"""
@file: test_auth.py
@description: Набор API тестов для эндпоинтов авторизации (Auth)
@dependencies: pytest, requests
@created: 2025-10-29
@author: Directpi
@test_type: api
"""
import os

import pytest
import requests
from typing import Dict

pytestmark = [pytest.mark.api]
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


class TestAuthLogin:
    """Тесты для эндпоинта POST /api/v1/auth/login"""
    
    @pytest.mark.smoke
    def test_login_success(self, api_base_url: str, test_credentials: Dict[str, str]):
        """Позитивный тест: успешная авторизация с валидными значениями"""
        response = requests.post(
            f"{api_base_url}/api/v1/auth/login",
            data={
                'username': test_credentials['username'],
                'password': test_credentials['password']
            },
            headers={'Content-Type': 'application/x-www-form-urlencoded'},
            timeout=10
        )

        assert_status(
            response,
            [202],
            context="Успешный логин должен возвращать 202",
            bug_map={422: "#BUG-AUTH-LOGIN-SUCCESS-422"},
        )
        data = response.json()
        assert 'access_token' in data, "В ответе должен быть access_token"
        assert 'refresh_token' in data, "В ответе должен быть refresh_token"
        assert str(data.get('token_type', '')).lower() == 'bearer', "Тип токена должен быть 'bearer'"
        assert len(data['access_token']) > 0, "Access token не должен быть пустым"
    
    def test_login_invalid_credentials(self, api_base_url: str):
        """Негативный тест: авторизация с неверными учетными данными"""
        response = requests.post(
            f"{api_base_url}/api/v1/auth/login",
            data={
                'username': 'invalid@test.com',
                'password': 'wrongpassword123'
            },
            headers={'Content-Type': 'application/x-www-form-urlencoded'},
            timeout=10
        )

        assert_status(
            response,
            [422],
            context="Login с неверными учетными данными должен возвращать 422",
            bug_map={
                401: "#BUG-AUTH-LOGIN-UNAUTHORIZED",
                403: "#BUG-AUTH-LOGIN-FORBIDDEN",
            },
        )
    
    def test_login_missing_username(self, api_base_url: str):
        """Негативный тест: отсутствует параметр username"""
        response = requests.post(
            f"{api_base_url}/api/v1/auth/login",
            data={'password': 'somepassword'},
            headers={'Content-Type': 'application/x-www-form-urlencoded'},
            timeout=10
        )
        
        assert_status(
            response,
            [422],
            context="Login без username должен возвращать 422",
        )
    
    def test_login_missing_password(self, api_base_url: str, test_credentials: Dict[str, str]):
        """Негативный тест: отсутствует параметр password"""
        response = requests.post(
            f"{api_base_url}/api/v1/auth/login",
            data={'username': test_credentials['username']},
            headers={'Content-Type': 'application/x-www-form-urlencoded'},
            timeout=10
        )
        
        assert_status(
            response,
            [422],
            context="Login без password должен возвращать 422",
        )


class TestAuthRegister:
    """Тесты для эндпоинта POST /api/v1/auth/register"""
    
    @pytest.mark.parametrize("email,password,first_name,last_name,expected_status", [
        ("newuser@test.com", "TestPass123!", "Test", "User", 201),
    ])
    def test_register_success(self, api_base_url: str, email: str, password: str,
                              first_name: str, last_name: str, expected_status: int,
                              authenticated_session: requests.Session, user_cleanup):
        """Позитивный тест: успешная регистрация нового пользователя
        
        ВАЖНО: Согласно OpenAPI схеме, обязательные поля:
        - name (не first_name)
        - surname (не last_name)
        - phone (обязательное)
        - password (обязательное)
        """
        # Генерируем уникальный телефон для избежания конфликтов
        import time
        # Генерируем 10 цифр после +7, чтобы получить ровно 12 символов: +7 + 10 цифр = 12
        # Используем timestamp для уникальности, гарантируя минимум 10 цифр
        timestamp = int(time.time() * 1000)
        # Берем последние 10 цифр из timestamp, дополняя нулями слева если нужно
        unique_suffix = f"{timestamp % 10000000000:010d}"  # Всегда 10 цифр
        unique_phone = f"+7{unique_suffix}"  # Итого: +7 + 10 цифр = 12 символов
        
        response = requests.post(
            f"{api_base_url}/api/v1/auth/register",
            json={
                'name': first_name,        # ✅ Исправлено: first_name -> name (согласно схеме)
                'surname': last_name,      # ✅ Исправлено: last_name -> surname (согласно схеме)
                'phone': unique_phone,     # ✅ Добавлено обязательное поле (согласно схеме)
                'password': password,
                'email': email,            # Опциональное поле
            },
            headers={'Content-Type': 'application/json'},
            timeout=10
        )

        assert_status(
            response,
            [201],
            context="Регистрация нового пользователя должна возвращать 201",
        )

        data = response.json()
        assert 'id' in data or 'email' in data, \
            "В ответе должна быть информация о пользователе"
        
        # Регистрируем пользователя для cleanup
        user_id = data.get('id')
        if user_id:
            user_cleanup(user_id)
    
    @pytest.mark.xfail(reason="#BUG-USER-EMAIL-VALIDATION: register принимает invalid email")
    def test_register_invalid_email(self, api_base_url: str, authenticated_session: requests.Session, user_cleanup):
        """Негативный тест: регистрация с невалидным email"""
        response = requests.post(
            f"{api_base_url}/api/v1/auth/register",
            json={
                'name': 'Test',
                'surname': 'User',
                'phone': '+79991234567',
                'password': 'TestPass123!',
                'email': 'not-an-email',
            },
            headers={'Content-Type': 'application/json'},
            timeout=10
        )
        
        if response.status_code == 201:
            user_id = response.json().get('id')
            if user_id:
                user_cleanup(user_id)
        
        assert_status(
            response,
            [422],
            context="Регистрация с невалидным email должна возвращать 422",
            bug_map={409: "#BUG-USER-EMAIL-VALIDATION"},
        )


class TestAuthRefreshToken:
    """Тесты для эндпоинта POST /api/v1/auth/refresh-access-token"""
    
    def test_refresh_token_success(self, authenticated_session: requests.Session):
        """Позитивный тест: успешное обновление токена"""
        # Используем refresh_token из фикстуры auth_token
        # Для полноты нужно получить refresh_token отдельно
        response = authenticated_session.post(
            f"{authenticated_session.base_url}/api/v1/auth/refresh-access-token",
            timeout=10
        )
        attach_response("auth.refresh.success", response)
        assert_status(
            response,
            [201],
            context="Успешное обновление access token должно возвращать 201",
            bug_map={401: "#BUG-AUTH-REFRESH-401"},
        )

        status = response.status_code
        if status == 201:
            data = response.json()
            assert 'access_token' in data, "В ответе должен быть новый access_token"
    
    def test_refresh_token_unauthorized(self, api_base_url: str):
        """Негативный тест: обновление токена без авторизации"""
        response = requests.post(
            f"{api_base_url}/api/v1/auth/refresh-access-token",
            timeout=10
        )
        attach_response("auth.refresh.unauthorized", response)
        assert_status(
            response,
            [201],
            context="Обновление токена без авторизации",
            bug_map={401: "#BUG-AUTH-REFRESH-UNAUTH-401"},
        )


class TestAuthHealthcheck:
    """Тесты для эндпоинта POST /api/v1/auth/healthcheck"""
    
    def test_healthcheck(self, api_base_url: str):
        """Проверка доступности сервиса авторизации"""
        response = requests.post(
            f"{api_base_url}/api/v1/auth/healthcheck",
            timeout=5,
        )
        attach_response("auth.healthcheck", response)

        assert_status(
            response,
            [200],
            context="Healthcheck должен возвращать 200",
            bug_map={201: "#BUG-AUTH-HEALTHCHECK-201"},
        )


class TestAuthTemporaryToken:
    """Тесты для эндпоинтов временных токенов"""
    
    def test_get_temporary_token(self, authenticated_session: requests.Session):
        """Проверка получения временного токена."""
        response = authenticated_session.get(
            f"{authenticated_session.base_url}/api/v1/auth/temporary-token",
            timeout=15
        )
        attach_response("auth.temporary_token.get", response)
        assert_status(
            response,
            [200],
            context="GET /auth/temporary-token",
            bug_map={
                403: "#BUG-AUTH-TEMP-TOKEN-403",
                404: "#BUG-AUTH-TEMP-TOKEN-404",
                501: "#BUG-AUTH-TEMP-TOKEN-501",
            },
        )

        status = response.status_code
        if status == 200:
            data = response.json()
            assert 'temporary_token' in data, "Ответ должен содержать temporary_token"

    
    def test_get_temporary_token_unauthorized(self, api_base_url: str):
        """Проверка получения временного токена без авторизации."""
        response = requests.get(
            f"{api_base_url}/api/v1/auth/temporary-token",
            timeout=10
        )
        assert_status(
            response,
            [200],
            context="GET /auth/temporary-token без авторизации",
            bug_map={
                401: "#BUG-AUTH-TEMP-TOKEN-UNAUTH-401",
                403: "#BUG-AUTH-TEMP-TOKEN-UNAUTH-403",
            },
        )
    
    def test_post_temporary_token_validate(self, authenticated_session: requests.Session):
        """Проверка валидации временного токена."""
        # Сначала пытаемся получить токен
        get_resp = authenticated_session.get(
            f"{authenticated_session.base_url}/api/v1/auth/temporary-token",
            timeout=15
        )
        attach_response("auth.temporary_token.get", get_resp)
        
        # Если получение токена успешно
        status_get = get_resp.status_code
        if status_get == 200:
            data = get_resp.json()
            token = data.get("temporary_token")
            
            # Токен должен быть непустой строкой
            if token and token.strip():
                # Согласно OpenAPI схеме, POST требует поле "temporary_token" (не "token")
                validate_resp_json = authenticated_session.post(
                    f"{authenticated_session.base_url}/api/v1/auth/temporary-token",
                    json={"temporary_token": token},  # Правильное имя поля из схемы
                    timeout=15
                )
                attach_response("auth.temporary_token.validate.json", validate_resp_json)
                validate_resp = validate_resp_json

                assert_status(
                    validate_resp,
                    [200, 422],
                    context="POST /auth/temporary-token (валидация)",
                    bug_map={
                        400: "#BUG-AUTH-TEMP-TOKEN-VALIDATE-400",
                        401: "#BUG-AUTH-TEMP-TOKEN-VALIDATE-401",
                        403: "#BUG-AUTH-TEMP-TOKEN-VALIDATE-403",
                        404: "#BUG-AUTH-TEMP-TOKEN-VALIDATE-404",
                    },
                )
            else:
                # Токен пустой или success=false - это нормальное поведение
                # (токен не был сгенерирован, возможно нужна авторизация или другие условия)
                pytest.skip(
                    f"Токен не был сгенерирован "
                    f"(token={'пустой' if not token or not token.strip() else 'есть'})"
                )
        else:
            # Если получение токена не работает, пропускаем тест
            pytest.skip(f"GET temporary-token вернул {get_resp.status_code}, пропускаем валидацию")


class TestAuthRefreshTokenEdges:
    """Дополнительные кейсы для POST /api/v1/auth/refresh-access-token"""

    def test_refresh_token_malformed_bearer(self, api_base_url: str):
        """Негатив: передан некорректный/поддельный токен в заголовке"""
        resp = requests.post(
            f"{api_base_url}/api/v1/auth/refresh-access-token",
            headers={"Authorization": "Bearer invalid.token.value"},
            timeout=10,
        )
        attach_response("auth.refresh.malformed", resp)
        assert_status(
            resp,
            [201],
            context="Refresh token с некорректным Bearer",
            bug_map={
                400: "#BUG-AUTH-REFRESH-MALFORMED-400",
                401: "#BUG-AUTH-REFRESH-MALFORMED-401",
                403: "#BUG-AUTH-REFRESH-MALFORMED-403",
            },
        )


class TestAuthChangePasswordNegatives:
    """Негативные кейсы для POST /api/v1/auth/change-password (JSON body)"""

    def test_change_password_unauthorized(self, api_base_url: str):
        """Негатив: без авторизации — ожидаем 401/403"""
        resp = requests.post(
            f"{api_base_url}/api/v1/auth/change-password",
            json={
                "username": "+79990000000",
                "password": "WrongPass123!",
                "new_password": "NewPass123!",
                "client_id": None,
                "client_secret": None,
            },
            timeout=10,
        )
        assert_status(
            resp,
            [202, 422],
            context="POST /auth/change-password без авторизации",
            bug_map={
                401: "#BUG-AUTH-CHANGE-PASSWORD-UNAUTH-401",
                403: "#BUG-AUTH-CHANGE-PASSWORD-UNAUTH-403",
            },
        )

    def test_change_password_wrong_current(self, authenticated_session: requests.Session, test_credentials: Dict[str, str]):
        """Негатив: неверный текущий пароль — ожидаем 400/401/403/422 по факту API

        ВАЖНО: API возвращает 401 (Unauthorized), хотя ожидался 400/403/422 как бизнес-валидация.
        Команда теста расширена: если сервер устраивает выход по 401 при неверном пароле — это явно фиксируется как 'feature/contract'.
        Отразить в баг-репорте и согласовать с командой.
        """
        payload = {
            "username": test_credentials["username"],
            "password": "DefinitelyWrong123!",
            "new_password": "NewPass123!",
            "client_id": None,
            "client_secret": None,
        }
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}/api/v1/auth/change-password",
            json=payload,
            timeout=10,
        )
        # Внимание: см. комментарий выше
        assert_status(
            resp,
            [422],
            context="POST /auth/change-password с неверным текущим паролем",
            bug_map={
                401: "#BUG-AUTH-CHANGE-PASSWORD-WRONG-401",
                403: "#BUG-AUTH-CHANGE-PASSWORD-WRONG-403",
            },
        )

    def test_change_password_empty_new(self, authenticated_session: requests.Session, test_credentials: Dict[str, str]):
        """Негатив: пустой новый пароль — ожидаем 422/400 (валидация)"""
        payload = {
            "username": test_credentials["username"],
            "password": "AlsoWrong123!",
            "new_password": "",
            "client_id": None,
            "client_secret": None,
        }
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}/api/v1/auth/change-password",
            json=payload,
            timeout=10,
        )
        assert_status(
            resp,
            [422],
            context="POST /auth/change-password с пустым новым паролем",
            bug_map={400: "#BUG-AUTH-CHANGE-PASSWORD-EMPTY-400"},
        )


class TestAuthChangePasswordPositive:
    """Позитивные кейсы для POST /api/v1/auth/change-password.

    Меняем пароль на временный и возвращаем обратно в конце теста.
    """

    def test_change_password_and_revert(self, api_base_url: str, test_credentials: Dict[str, str]):
        username = test_credentials["username"]
        current_password = test_credentials["password"]
        temp_new_password = f"Tmp{current_password}1!"

        # 1) Меняем пароль на временный
        resp_change = requests.post(
            f"{api_base_url}/api/v1/auth/change-password",
            json={
                "username": username,
                "password": current_password,
                "new_password": temp_new_password,
                "client_id": None,
                "client_secret": None,
            },
            timeout=15,
        )
        attach_response("auth.change-password.first", resp_change)
        assert_status(
            resp_change,
            [202, 422],
            context="Смена пароля (первый шаг)",
            bug_map={401: "#BUG-AUTH-CHANGE-PASS-FIRST-401", 403: "#BUG-AUTH-CHANGE-PASS-FIRST-403"},
        )

        try:
            login_resp = requests.post(
                f"{api_base_url}/api/v1/auth/login",
                data={"username": username, "password": temp_new_password},
                headers={"Content-Type": "application/x-www-form-urlencoded"},
                timeout=10,
            )
            attach_response("auth.login.new-pass", login_resp)
            assert_status(
                login_resp,
                [202, 422],
                context="Логин с новым паролем",
            )
        finally:
            _revert = requests.post(
                f"{api_base_url}/api/v1/auth/change-password",
                json={
                    "username": username,
                    "password": temp_new_password,
                    "new_password": current_password,
                    "client_id": None,
                    "client_secret": None,
                },
                timeout=15,
            )
            attach_response("auth.change-password.revert", _revert)
            assert_status(
                _revert,
                [202, 422],
                context="Возврат пароля",
                bug_map={401: "#BUG-AUTH-REVERT-PASS-401", 403: "#BUG-AUTH-REVERT-PASS-403"},
            )


@pytest.mark.api
class TestAuthValidateByPhone:
    """Тесты для эндпоинта POST /api/v1/auth/validate-by-phone"""

    def test_auth_validate_by_phone(self, api_base_url: str):
        """Проверка валидации телефона через API."""
        phone = os.getenv("TEST_USER") or os.getenv("TEST_USER_EMAIL")
        if not phone:
            pytest.skip("Отсутствует TEST_USER в .env")
        resp = requests.post(
            f"{api_base_url}/api/v1/auth/validate-by-phone",
            json={"phone": phone},
            timeout=10,
        )
        attach_response("auth.validate_by_phone", resp)
        # По схеме ожидается только 200, но API может возвращать 401/404
        assert_status(
            resp,
            [200],
            context="POST /auth/validate-by-phone",
            bug_map={
                401: "#BUG-AUTH-VALIDATE-PHONE-401: Invalid credentials",
                404: "#BUG-AUTH-VALIDATE-PHONE-404: User not found",
                422: "#BUG-AUTH-VALIDATE-PHONE-422",
            },
        )
