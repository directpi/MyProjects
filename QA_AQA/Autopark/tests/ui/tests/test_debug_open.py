"""Минимальный тест для отладки - просто открыть страницу."""
import pytest
import time

@pytest.mark.ui
def test_just_open(browser, platform_url):
    """Просто открываем страницу и смотрим URL."""
    print(f"\n🔍 Opening: {platform_url}")
    browser.get(platform_url)
    print(f"✅ Page loaded")
    print(f"📍 Current URL: {browser.current_url}")
    print(f"📄 Page title: {browser.title}")
    time.sleep(2)
    print("✅ Test completed")









