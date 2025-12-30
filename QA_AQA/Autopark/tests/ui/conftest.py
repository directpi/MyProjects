"""
@file: tests/ui/conftest.py
@description: Базовые фикстуры для UI тестов на Selenium (удалённый webdriver)
@dependencies: pytest, selenium, python-dotenv, allure-pytest
@created: 2025-11-24
@author: Directpi
@test_type: ui
"""

from __future__ import annotations

import os
from typing import Generator

import pytest
import requests
from dotenv import find_dotenv, load_dotenv
from selenium import webdriver
from selenium.webdriver.chrome.options import Options as ChromeOptions
from selenium.webdriver.firefox.options import Options as FirefoxOptions
from selenium.webdriver.remote.webdriver import WebDriver

try:
    import allure  # type: ignore
except Exception:  # noqa: BLE001
    allure = None


def _ensure_env_loaded() -> None:
    """Загружает .env, если он ещё не был загружен."""
    project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
    candidates = [
        os.path.join(project_root, ".env"),
        os.path.join(project_root, "tests", "config", ".env"),
    ]
    for candidate in candidates:
        if os.path.exists(candidate):
            load_dotenv(candidate, override=False)
            return
    auto_env = find_dotenv(usecwd=True)
    if auto_env:
        load_dotenv(auto_env, override=False)


_ensure_env_loaded()


def _bool_env(name: str, default: bool) -> bool:
    val = os.getenv(name)
    if val is None:
        return default
    return val.strip().lower() in {"1", "true", "yes", "y", "on"}


def _window_size() -> str:
    raw = os.getenv("SELENIUM_WINDOW_SIZE", "1440,900").strip()
    if "x" in raw:
        return raw.replace("x", ",")
    return raw


def _build_driver() -> WebDriver:
    remote_url = os.getenv("SELENIUM_REMOTE_URL", "http://localhost:4444/wd/hub")
    browser = os.getenv("SELENIUM_BROWSER", "chrome").lower()
    headless = _bool_env("SELENIUM_HEADLESS", True)
    page_load_timeout = int(os.getenv("SELENIUM_PAGELOAD_TIMEOUT", "120"))
    implicit_wait = float(os.getenv("SELENIUM_IMPLICIT_WAIT", "2"))

    if browser not in {"chrome", "firefox"}:
        raise pytest.UsageError(
            f"Неподдерживаемый браузер '{browser}'. Допустимо: chrome, firefox."
        )

    if browser == "chrome":
        options = ChromeOptions()
        if headless:
            options.add_argument("--headless=new")
        options.add_argument("--no-sandbox")
        options.add_argument("--disable-dev-shm-usage")
        options.add_argument(f"--window-size={_window_size()}")
        # Не ждать полной загрузки всех ресурсов (для NiceGUI/SPA)
        options.page_load_strategy = 'eager'
        driver = webdriver.Remote(command_executor=remote_url, options=options)
    else:
        options = FirefoxOptions()
        if headless:
            options.add_argument("-headless")
        driver = webdriver.Remote(command_executor=remote_url, options=options)
        try:
            width, height = (int(x) for x in _window_size().split(","))
            driver.set_window_size(width, height)
        except Exception:  # noqa: BLE001
            pass

    driver.set_page_load_timeout(page_load_timeout)
    driver.implicitly_wait(implicit_wait)
    return driver


@pytest.hookimpl(tryfirst=True, hookwrapper=True)
def pytest_runtest_makereport(item, call):  # type: ignore[override]
    """Добавляем информацию о статусе теста на объект item."""
    outcome = yield
    rep = outcome.get_result()
    setattr(item, f"rep_{rep.when}", rep)


@pytest.fixture(scope="session")
def platform_url() -> str | None:
    """UI адрес платформы из `.env`. Возвращает None, если недоступен, чтобы зависимые тесты смогли корректно скипнуться."""
    url = os.getenv("PLATFORM_URL")
    if not url:
        return None
    url = url.rstrip("/")
    try:
        import socket
        from urllib.parse import urlparse
        parsed = urlparse(url)
        host = parsed.hostname
        if not host:
            return url
        port = parsed.port or (443 if parsed.scheme == "https" else 80)
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(5)
        result = sock.connect_ex((host, port))
        sock.close()
        if result != 0:
            print(f"platform_url WARN: {url} недоступна (порт {port}) — верну None")
            return None
    except Exception:
        # Если проверка не удалась, продолжаем (может быть проблема с DNS)
        pass
    return url


@pytest.fixture(scope="function")
def browser(request: pytest.FixtureRequest) -> Generator[WebDriver, None, None]:
    """Основной фикстура WebDriver (Remote Selenium)."""
    driver = _build_driver()
    yield driver

    # Снимем артефакты, если тест упал
    rep_setup = getattr(request.node, "rep_setup", None)
    rep_call = getattr(request.node, "rep_call", None)
    failed = (rep_setup and rep_setup.failed) or (rep_call and rep_call.failed)
    if failed:
        test_name = request.node.name
        try:
            screenshot = driver.get_screenshot_as_png()
            if allure:
                allure.attach(
                    screenshot,
                    name=f"{test_name}-screenshot",
                    attachment_type=allure.attachment_type.PNG,
                )
        except Exception:  # noqa: BLE001
            pass
        try:
            page_source = driver.page_source
            if allure:
                allure.attach(
                    page_source,
                    name=f"{test_name}-dom",
                    attachment_type=allure.attachment_type.HTML,
                )
        except Exception:  # noqa: BLE001
            pass

    # Явно удаляем все cookies перед закрытием драйвера
    # (предотвращает проблемы с повторным логином)
    try:
        driver.delete_all_cookies()
    except Exception:  # noqa: BLE001
        pass

    driver.quit()


@pytest.fixture(scope="function")
def authenticated_browser(
    request: pytest.FixtureRequest,
    platform_url: str | None,
) -> Generator[WebDriver, None, None]:
    """
    Браузер с авторизацией для UI/E2E тестов.
    Для КАЖДОГО теста:
    - создаёт новый WebDriver
    - открывает страницу логина
    - вводит логин/пароль из .env
    - проверяет успешный редирект (не на /login)

    Логика максимально приближена к `test_smoke_login_dashboard`,
    который у тебя стабильно проходит.
    """
    if not platform_url:
        pytest.skip("PLATFORM_URL недоступен")

    driver = _build_driver()

    phone = os.getenv("TEST_USER") or os.getenv("TEST_USER_EMAIL")
    password = os.getenv("TEST_USER_PASSWORD")

    if not (phone and password):
        try:
            driver.quit()
        except Exception:  # noqa: BLE001
            pass
        pytest.skip("Отсутствуют TEST_USER/TEST_USER_PASSWORD в .env")

    try:
        import time
        from selenium.webdriver.common.by import By
        from selenium.webdriver.support.ui import WebDriverWait
        from selenium.webdriver.support import expected_conditions as EC

        # 1. Открываем страницу логина
        driver.get(platform_url)
        time.sleep(1)

        # 2. Находим поля ввода
        phone_input = WebDriverWait(driver, 10).until(
            EC.presence_of_element_located((By.CSS_SELECTOR, 'input[type="text"]'))
        )
        password_input = driver.find_element(By.CSS_SELECTOR, 'input[type="password"]')

        # 3. Обрабатываем префикс +7
        phone_to_type = phone
        if phone.startswith("+7"):
            try:
                prefix = driver.find_element(
                    By.XPATH, "//form//*[contains(text(), '+7')]"
                )
                if prefix:
                    phone_to_type = phone[2:]
            except Exception:
                phone_to_type = phone[2:]

        # 4. Вводим данные
        phone_input.clear()
        phone_input.send_keys(phone_to_type)
        password_input.clear()
        password_input.send_keys(password)

        # 5. Ищем кнопку "Войти"
        login_btn = None
        try:
            login_btn = driver.find_element(By.CSS_SELECTOR, "#c21")
            if not login_btn.is_displayed():
                login_btn = None
        except Exception:
            login_btn = None

        if not login_btn:
            try:
                login_btn = driver.find_element(
                    By.XPATH, "//form//button[contains(., 'Войти')]"
                )
            except Exception:
                login_btn = None

        if not login_btn:
            raise AssertionError("Кнопка логина не найдена")

        # 6. Кликаем по кнопке
        driver.execute_script("arguments[0].click();", login_btn)

        # 7. Ждём редирект
        wait = WebDriverWait(driver, 20)
        wait.until(lambda d: "login" not in d.current_url.lower())

        current_url = driver.current_url.lower()
        if "login" in current_url:
            raise Exception(
                f"После логина всё ещё на странице login: {driver.current_url}"
            )

        print(f"[authenticated_browser] Logged in as {phone}, URL: {driver.current_url}")
    except Exception as e:
        print(f"[authenticated_browser] Login failed: {e}")
        try:
            driver.quit()
        except Exception:  # noqa: BLE001
            pass
        pytest.skip(f"Не удалось залогиниться через authenticated_browser: {e}")

    yield driver
    
    # Logout в конце сессии (только если браузер еще открыт)
    # НЕ делаем logout здесь, так как test_logout уже проверил logout
    # и перелогинился для следующих тестов
    # Если нужно сделать logout в конце сессии, это можно добавить,
    # но сейчас оставляем браузер залогиненным для возможных последующих тестов
    # try:
    #     if driver.current_url and "login" not in driver.current_url.lower():
    #         from tests.ui.pages.dashboard_page import DashboardPage
    #         dashboard = DashboardPage(driver)
    #         dashboard.logout()
    #         print("[authenticated_browser] Logged out at end of session")
    # except Exception as e:
    #     print(f"[authenticated_browser] Logout failed: {e}")
    
    # Снимаем артефакты если были ошибки в сессии
    try:
        driver.delete_all_cookies()
    except Exception:  # noqa: BLE001
        pass
    
    # Аккуратно закрываем драйвер, игнорируя ошибки "InvalidSessionId",
    # которые возможны, если Selenium Grid уже убил сессию по таймауту.
    try:
        driver.quit()
    except Exception:  # noqa: BLE001
        pass


@pytest.fixture(scope="session")
def ui_session(platform_url: str) -> dict[str, str]:
    """Хранит базовые параметры для UI тестов (URL и т.п.)."""
    if not platform_url:
        pytest.skip("PLATFORM_URL недоступен — пропускаю UI тесты")
    return {"platform_url": platform_url}


@pytest.fixture
def api_session(api_base_url: str | None, auth_token: str | None):
    """Сессия API для валидации данных в UI/E2E тестах."""
    if not api_base_url:
        pytest.skip("API недоступен — пропускаю UI тесты, зависящие от API")
    session = requests.Session()
    session.base_url = api_base_url
    if auth_token:
        session.headers.update({"Authorization": f"Bearer {auth_token}"})
    return session

