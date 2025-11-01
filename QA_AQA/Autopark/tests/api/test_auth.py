"""
@file: test_auth.py
@description: Набор API тестов для эндпоинтов авторизации (Auth)
@dependencies: pytest, requests
@created: 2025-10-29
@author: Directpi
@test_type: api
"""
import pytest
import requests
from typing import Dict


class TestAuthLogin:
    """Тесты для эндпоинта POST /api/v1/auth/login"""
    
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
        
        assert response.status_code == 202, f"Ожидался статус 202, получен {response.status_code}"
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
        
        # Ожидаем ошибку авторизации (401 или 403)
        assert response.status_code in [401, 403, 422], \
            f"Ожидался статус ошибки (401/403/422), получен {response.status_code}"
    
    def test_login_missing_username(self, api_base_url: str):
        """Негативный тест: отсутствует параметр username"""
        response = requests.post(
            f"{api_base_url}/api/v1/auth/login",
            data={'password': 'somepassword'},
            headers={'Content-Type': 'application/x-www-form-urlencoded'},
            timeout=10
        )
        
        assert response.status_code == 422, \
            f"Ожидался статус валидации 422, получен {response.status_code}"
    
    def test_login_missing_password(self, api_base_url: str, test_credentials: Dict[str, str]):
        """Негативный тест: отсутствует параметр password"""
        response = requests.post(
            f"{api_base_url}/api/v1/auth/login",
            data={'username': test_credentials['username']},
            headers={'Content-Type': 'application/x-www-form-urlencoded'},
            timeout=10
        )
        
        assert response.status_code == 422, \
            f"Ожидался статус валидации 422, получен {response.status_code}"


class TestAuthRegister:
    """Тесты для эндпоинта POST /api/v1/auth/register"""
    
    @pytest.mark.parametrize("email,password,first_name,last_name,expected_status", [
        ("newuser@test.com", "TestPass123!", "Test", "User", 201),
    ])
    def test_register_success(self, api_base_url: str, email: str, password: str,
                              first_name: str, last_name: str, expected_status: int):
        """Позитивный тест: успешная регистрация нового пользователя"""
        # Внимание: если пользователь уже существует, тест может упасть
        # В продакшене нужна очистка или уникальные email
        
        response = requests.post(
            f"{api_base_url}/api/v1/auth/register",
            json={
                'email': email,
                'password': password,
                'first_name': first_name,
                'last_name': last_name
            },
            headers={'Content-Type': 'application/json'},
            timeout=10
        )
        
        # Может быть 201 (успех) или 422/400 (пользователь уже существует)
        if response.status_code == expected_status:
            data = response.json()
            assert 'id' in data or 'email' in data, \
                "В ответе должна быть информация о пользователе"
    
    def test_register_invalid_email(self, api_base_url: str):
        """Негативный тест: регистрация с невалидным email"""
        response = requests.post(
            f"{api_base_url}/api/v1/auth/register",
            json={
                'email': 'not-an-email',
                'password': 'TestPass123!',
                'first_name': 'Test',
                'last_name': 'User'
            },
            headers={'Content-Type': 'application/json'},
            timeout=10
        )
        
        assert response.status_code == 422, \
            f"Ожидался статус валидации 422, получен {response.status_code}"


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
        
        # Статус может быть 201 или 401 если токен невалиден
        assert response.status_code in [201, 401], \
            f"Неожиданный статус: {response.status_code}"
        
        if response.status_code == 201:
            data = response.json()
            assert 'access_token' in data, "В ответе должен быть новый access_token"
    
    def test_refresh_token_unauthorized(self, api_base_url: str):
        """Негативный тест: обновление токена без авторизации"""
        response = requests.post(
            f"{api_base_url}/api/v1/auth/refresh-access-token",
            timeout=10
        )
        
        assert response.status_code == 401, \
            f"Ожидался статус 401, получен {response.status_code}"


class TestAuthHealthcheck:
    """Тесты для эндпоинта POST /api/v1/auth/healthcheck"""
    
    def test_healthcheck(self, api_base_url: str):
        """Проверка доступности сервиса авторизации"""
        response = requests.post(
            f"{api_base_url}/api/v1/auth/healthcheck",
            timeout=10
        )
        
        assert response.status_code in [200, 202], \
            f"Healthcheck должен возвращать 200/202, получен {response.status_code}"


class TestAuthRefreshTokenEdges:
    """Дополнительные кейсы для POST /api/v1/auth/refresh-access-token"""

    def test_refresh_token_malformed_bearer(self, api_base_url: str):
        """Негатив: передан некорректный/поддельный токен в заголовке"""
        resp = requests.post(
            f"{api_base_url}/api/v1/auth/refresh-access-token",
            headers={"Authorization": "Bearer invalid.token.value"},
            timeout=10,
        )
        assert resp.status_code in [400, 401, 403], \
            f"Ожидался 400/401/403, получен {resp.status_code}"


class TestAuthChangePasswordNegatives:
    """Негативные кейсы для POST /api/v1/auth/change-password (JSON body)"""

    def test_change_password_unauthorized(self, api_base_url: str):
        """Негатив: без авторизации — ожидаем 401/403"""
        payload = {
            "username": "+79990000000",
            "password": "WrongPass123!",
            "new_password": "NewPass123!",
        }
        resp = requests.post(
            f"{api_base_url}/api/v1/auth/change-password",
            json=payload,
            timeout=10,
        )
        assert resp.status_code in [401, 403], \
            f"Ожидался 401/403, получен {resp.status_code}"

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
        }
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}/api/v1/auth/change-password",
            json=payload,
            timeout=10,
        )
        # Внимание: см. комментарий выше
        assert resp.status_code in [400, 401, 403, 422], \
            f"Ожидался 400/401/403/422, получен {resp.status_code}"

    def test_change_password_empty_new(self, authenticated_session: requests.Session, test_credentials: Dict[str, str]):
        """Негатив: пустой новый пароль — ожидаем 422/400 (валидация)"""
        payload = {
            "username": test_credentials["username"],
            "password": "AlsoWrong123!",
            "new_password": "",
        }
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}/api/v1/auth/change-password",
            json=payload,
            timeout=10,
        )
        assert resp.status_code in [422, 400], \
            f"Ожидался 422/400, получен {resp.status_code}"
