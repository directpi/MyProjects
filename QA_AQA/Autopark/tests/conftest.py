"""
Конфигурация pytest для API тестов
"""

import os
import re
import pytest
import requests
from typing import Dict, Optional
from dotenv import load_dotenv, find_dotenv

# Загружаем переменные окружения из .env
# Приоритет: корень проекта -> tests/config/.env -> auto-discovery
tests_dir = os.path.dirname(__file__)
project_root = os.path.abspath(os.path.join(tests_dir, ".."))

root_env_path = os.path.join(project_root, ".env")
tests_env_path = os.path.join(tests_dir, "config", ".env")

loaded = False
if os.path.exists(root_env_path):
    load_dotenv(root_env_path)
    loaded = True
elif os.path.exists(tests_env_path):
    load_dotenv(tests_env_path)
    loaded = True
else:
    auto_env = find_dotenv(usecwd=True)
    if auto_env:
        load_dotenv(auto_env)
        loaded = True


def _get_first_env(*keys: str) -> str | None:
    for key in keys:
        val = os.getenv(key)
        if val:
            return val
    return None


# Базовые настройки из переменных окружения
# SECURITY: do not provide an external default URL to avoid accidental runs
# against non-intended (including production) environments.
API_BASE_URL = _get_first_env("API_BASE_URL", "BASE_URL", "API_URL")
TEST_USER = _get_first_env(
    "TEST_USER", "USER_EMAIL", "AUTH_USERNAME", "AUTH_EMAIL", "USERNAME", "EMAIL"
)
TEST_PASSWORD = _get_first_env(
    "TEST_PASSWORD", "TEST_USER_PASSWORD", "USER_PASSWORD", "AUTH_PASSWORD", "PASSWORD"
)

_called_endpoints = set()


@pytest.fixture(autouse=True)
def _log_requests(monkeypatch):
    orig_request = requests.sessions.Session.request

    def wrapped(self, method, url, *args, **kwargs):
        m = re.search(r"/api/v1/[a-zA-Z0-9_/-]+", url)
        if m:
            _called_endpoints.add(f"{method.upper()} {m.group(0)}")
        return orig_request(self, method, url, *args, **kwargs)

    monkeypatch.setattr(requests.sessions.Session, "request", wrapped)
    yield


@pytest.hookimpl(trylast=True)
def pytest_sessionfinish(session, exitstatus):
    if _called_endpoints:
        print(f"\nУникальных эндпоинтов вызвано: {len(_called_endpoints)}")
        for ep in sorted(_called_endpoints):
            print("  ", ep)


@pytest.fixture(scope="session")
def api_base_url() -> str:
    """Базовый URL API"""
    if not API_BASE_URL:
        pytest.skip("API_BASE_URL не задан в .env — пропускаю API тесты")
    return API_BASE_URL


@pytest.fixture(scope="session")
def test_credentials() -> Dict[str, str]:
    """Тестовые учетные данные"""
    if not TEST_USER or not TEST_PASSWORD:
        pytest.skip(
            "Отсутствуют TEST_USER/TEST_PASSWORD в .env — пропускаю тесты, требующие авторизации"
        )
    return {"username": TEST_USER, "password": TEST_PASSWORD}


@pytest.fixture(scope="function")
def auth_token(api_base_url: str, test_credentials: Dict[str, str]) -> Optional[str]:
    """
    Получает токен авторизации для тестов.
    Возвращает None если авторизация не удалась (для негативных тестов).
    """
    try:
        response = requests.post(
            f"{api_base_url}/api/v1/auth/login",
            data={
                "username": test_credentials["username"],
                "password": test_credentials["password"],
            },
            headers={"Content-Type": "application/x-www-form-urlencoded"},
            timeout=10,
        )
        if response.status_code == 202:
            token_data = response.json()
            return token_data.get("access_token")
    except Exception:
        pass
    return None


@pytest.fixture(scope="function")
def authenticated_session(
    api_base_url: str, auth_token: Optional[str]
) -> requests.Session:
    """
    Создает сессию с авторизацией для тестовых запросов
    """
    session = requests.Session()
    session.base_url = api_base_url

    if auth_token:
        session.headers.update({"Authorization": f"Bearer {auth_token}"})

    return session


@pytest.fixture
def user_cleanup(authenticated_session):
    created_ids = []
    def register(user_id):
        created_ids.append(user_id)
    yield register
    # Teardown: удаляем всех созданных пользователей (id)
    for uid in created_ids:
        try:
            resp = authenticated_session.delete(
                f"{authenticated_session.base_url}/api/v1/user",
                params={"user_id": uid},
                timeout=10,
            )
            if resp.status_code not in (200, 202, 204):
                print("user_cleanup WARN:", uid, resp.status_code, resp.text)
        except Exception as e:
            print("user_cleanup ERROR:", uid, str(e))

@pytest.fixture
def vehicle_cleanup(authenticated_session):
    created_ids = []
    def register(vehicle_id):
        created_ids.append(vehicle_id)
    yield register
    # Teardown
    for vid in created_ids:
        try:
            resp = authenticated_session.delete(
                f"{authenticated_session.base_url}/api/v1/vehicle",
                params={"vehicle_id": vid},
                timeout=10,
            )
            if resp.status_code not in (200, 202, 204):
                print("vehicle_cleanup WARN:", vid, resp.status_code, resp.text)
        except Exception as e:
            print("vehicle_cleanup ERROR:", vid, str(e))
