"""Проверка кредов и ошибок на странице логина."""
import os
import time
import pytest
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC


@pytest.mark.ui
def test_check_login_error(browser, platform_url):
    """Проверяем ошибки после попытки логина."""
    phone = os.getenv("TEST_USER") or os.getenv("TEST_USER_EMAIL")
    password = os.getenv("TEST_USER_PASSWORD")
    
    if not phone or not password:
        pytest.skip("No creds")
    
    print(f"\n📞 Creds: phone={phone}, password={'*' * len(password)}")
    
    browser.get(platform_url)
    time.sleep(2)
    
    # Вводим данные
    phone_input = browser.find_element(By.CSS_SELECTOR, 'input[type="text"]')
    phone_input.clear()
    phone_input.send_keys(phone)
    
    password_input = browser.find_element(By.CSS_SELECTOR, 'input[type="password"]')
    password_input.clear()
    password_input.send_keys(password)
    
    time.sleep(1)
    
    # Нажимаем Enter
    password_input.send_keys(Keys.RETURN)
    
    print("⏳ Waiting 3 seconds...")
    time.sleep(3)
    
    # Проверяем URL
    print(f"📍 URL after login attempt: {browser.current_url}")
    
    # Оптимизированный поиск ошибок: ищем только в контексте формы, не читаем весь body.text
    error_elements = []
    error_selectors = [
        '.error',
        '.q-notification',
        '[role="alert"]',
        '.q-banner--negative',  # Quasar error banner
    ]
    for selector in error_selectors:
        try:
            elements = browser.find_elements(By.CSS_SELECTOR, selector)
            error_elements.extend(elements)
        except Exception:
            continue
    
    if error_elements:
        print(f"\n❌ Found {len(error_elements)} error elements:")
        for el in error_elements[:3]:  # Ограничиваем до 3 элементов
            try:
                if el.text.strip():
                    print(f"  - {el.text}")
            except Exception:
                pass
    
    # Оптимизированная проверка: ищем ошибки только в контексте формы
    try:
        form_errors = browser.find_elements(
            By.XPATH, "//form//*[contains(text(), 'неверн') or contains(text(), 'incorrect') or contains(text(), 'ошибка')]"
        )
        if form_errors:
            print("\n⚠️ Possible error message found in form")
    except Exception:
        pass
    
    print("\n✅ Check complete")









