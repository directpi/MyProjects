"""Проверка кредов и ошибок на странице логина."""
import os
import time

import pytest
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys


@pytest.mark.ui
def test_check_login_error(browser, platform_url):
    """Проверяем ошибки после попытки логина."""
    phone = os.getenv("TEST_USER") or os.getenv("TEST_USER_EMAIL")
    password = os.getenv("TEST_USER_PASSWORD")

    if not phone or not password:
        pytest.skip("No creds")

    # SECURITY: do not print PII (phone/email) to logs
    if len(phone) >= 4:
        masked_phone = phone[:2] + "***" + phone[-2:]
    else:
        masked_phone = "***"
    print(f"\nCreds: phone={masked_phone}, password={'*' * len(password)}")

    browser.get(platform_url)
    time.sleep(2)

    # Вводим данные
    phone_input = browser.find_element(By.CSS_SELECTOR, 'input[type="text"]')
    phone_input.clear()
    phone_input.send_keys(phone)

    password_css = 'input[type="password"]'
    password_input = browser.find_element(By.CSS_SELECTOR, password_css)
    password_input.clear()
    password_input.send_keys(password)

    time.sleep(1)

    # Нажимаем Enter
    password_input.send_keys(Keys.RETURN)

    print("⏳ Waiting 3 seconds...")
    time.sleep(3)

    # Проверяем URL
    print(f"📍 URL after login attempt: {browser.current_url}")

    # Ищем ошибки в контексте формы, не читаем весь body.text
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
        form_error_xpath = (
            "//form//*[contains(text(), 'неверн') "
            "or contains(text(), 'incorrect') "
            "or contains(text(), 'ошибка')]"
        )
        form_errors = browser.find_elements(
            By.XPATH, form_error_xpath
        )
        if form_errors:
            print("\n⚠️ Possible error message found in form")
    except Exception:
        pass

    print("\n✅ Check complete")
