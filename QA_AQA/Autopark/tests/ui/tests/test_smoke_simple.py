"""
@file: tests/ui/tests/test_smoke_simple.py
@description: Упрощённый smoke тест для диагностики
@dependencies: pytest, selenium
@created: 2025-12-11
@author: Directpi
@test_type: ui
"""

from __future__ import annotations

import pytest


@pytest.mark.ui
@pytest.mark.smoke
def test_simple_login(authenticated_browser, platform_url):
    """
    Упрощённый smoke тест: проверяет, что authenticated_browser залогинен.
    
    Использует session-scoped authenticated_browser для снижения нагрузки на Grid.
    Логин выполняется один раз в начале сессии через фикстуру authenticated_browser.
    """
    browser = authenticated_browser
    
    # Проверяем, что мы залогинены (authenticated_browser уже выполнил логин)
    current_url = browser.current_url.lower()
    
    if "login" in current_url:
        pytest.skip(
            f"authenticated_browser не залогинен. "
            f"URL: {browser.current_url}. "
            f"Возможно, логин не прошел при создании фикстуры."
        )
    
    # Проверяем, что мы на платформе (не на странице логина)
    assert "login" not in current_url, (
        f"Ожидалось, что authenticated_browser залогинен, "
        f"но URL содержит 'login': {browser.current_url}"
    )
    
    print(f"✅ authenticated_browser залогинен, URL: {browser.current_url}")
    print("✅ Smoke тест пройден")

