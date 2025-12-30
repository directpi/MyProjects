"""
@file: test_ui_logout.py
@description: UI тест для выхода из системы через меню аккаунта
@dependencies: pytest, selenium
@created: 2025-11-25
@author: Directpi
@test_type: ui

ИСПРАВЛЕНО: Использует DashboardPage.logout() для правильного выхода
через UI (иконка аккаунта → меню → "Выйти из системы").
Это предотвращает перегрузку платформы.
"""

from __future__ import annotations

import os

import pytest

from tests.ui.pages.dashboard_page import DashboardPage
from tests.ui.pages.login_page import LoginPage


@pytest.mark.ui
def test_logout(authenticated_browser, platform_url):
    """
    Проверка выхода из системы через UI.

    Использует session-scoped authenticated_browser для снижения нагрузки на Grid.
    Логин выполняется один раз в начале сессии через фикстуру authenticated_browser.
    Тест проверяет только процесс logout.
    """
    browser = authenticated_browser
    
    # 1. Проверяем, что мы залогинены (authenticated_browser уже выполнил логин)
    current_url = browser.current_url.lower()
    if "login" in current_url:
        pytest.skip(
            f"authenticated_browser не залогинен. "
            f"URL: {browser.current_url}. "
            f"Возможно, логин не прошел при создании фикстуры."
        )
    
    print(f"[test_logout] Начало теста. Текущий URL: {browser.current_url}")
    print("[test_logout] authenticated_browser уже залогинен, переходим к logout")

    # 2. Logout через DashboardPage (правильный способ через UI)
    dashboard = DashboardPage(browser)
    dashboard.logout()

    # 3. Проверяем что на /login (после logout мы ДОЛЖНЫ вернуться на login)
    assert "login" in browser.current_url.lower(), (
        f"После logout не произошёл переход на /login. "
        f"Текущий URL: {browser.current_url}"
    )
    
    print(f"[test_logout] ✅ Logout успешен, вернулись на /login. URL: {browser.current_url}")
    
    # 4. ВАЖНО: Перелогиниваемся, чтобы восстановить состояние для следующих тестов,
    # использующих authenticated_browser (session-scoped)
    # Это необходимо, так как authenticated_browser используется другими тестами
    phone = os.getenv("TEST_USER") or os.getenv("TEST_USER_EMAIL")
    password = os.getenv("TEST_USER_PASSWORD")
    if phone and password:
        try:
            print("[test_logout] Перелогиниваемся для восстановления сессии...")
            login_page = LoginPage(browser, platform_url)
            login_page.login(phone, password)
            # Проверяем, что логин прошел
            if "login" not in browser.current_url.lower():
                print(f"[test_logout] ✅ Перелогинивание успешно. URL: {browser.current_url}")
            else:
                print(f"[test_logout] ⚠️ Перелогинивание не удалось, остались на /login")
        except Exception as e:
            print(f"[test_logout] ⚠️ Ошибка при перелогинивании: {e}")
            # Не падаем, так как основной тест (logout) уже прошел
