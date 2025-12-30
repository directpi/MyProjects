"""
Диагностический тест для отладки проблемы с формой создания ТС.
"""

from __future__ import annotations

import time

import pytest
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait

from tests.ui.pages.vehicle_list_page import VehicleListPage
from tests.ui.pages.vehicle_form_page import VehicleFormPage


@pytest.mark.ui
def test_debug_vehicle_form_loading(
    authenticated_browser,
    platform_url,
):
    """Диагностика загрузки формы создания ТС."""
    browser = authenticated_browser

    print("\n" + "=" * 80)
    print("ШАГ 1: Переход на страницу списка ТС")
    print("=" * 80)
    browser.get(f"{platform_url}/vehicle")
    print(f"✓ URL после перехода: {browser.current_url}")
    time.sleep(2)  # Даем время на загрузку

    print("\n" + "=" * 80)
    print("ШАГ 2: Проверка загрузки списка ТС")
    print("=" * 80)
    vehicle_list = VehicleListPage(browser, timeout=10)
    try:
        vehicle_list.wait_loaded()
        print("✓ Список ТС загружен")
    except Exception as e:
        print(f"✗ Ошибка загрузки списка: {e}")
        return

    print("\n" + "=" * 80)
    print("ШАГ 3: Поиск кнопки 'Добавить ТС'")
    print("=" * 80)
    
    # Сначала пробуем стандартные селекторы
    add_button = None
    for selector in vehicle_list.add_button_selectors:
        try:
            # Пропускаем XPath селекторы для CSS_SELECTOR
            if selector.startswith("//") or ":contains(" in selector:
                continue
            element = browser.find_element(By.CSS_SELECTOR, selector)
            if element.is_displayed():
                add_button = element
                print(f"✓ Кнопка найдена по селектору: {selector}")
                print(f"  - Текст: {element.text}")
                print(f"  - ID: {element.get_attribute('id')}")
                print(f"  - Видима: {element.is_displayed()}")
                break
        except Exception as e:
            print(f"✗ Селектор '{selector}' не найден: {type(e).__name__}")
            continue
    
    # Пробуем XPath селекторы
    if not add_button:
        xpath_selectors = [
            "//button[@id='c107']",
            "//button[.//span[text()='+']]",
            "//button[contains(., '+') and not(contains(., '×'))]",
        ]
        for xpath in xpath_selectors:
            try:
                element = browser.find_element(By.XPATH, xpath)
                if element.is_displayed():
                    add_button = element
                    print(f"✓ Кнопка найдена по XPath: {xpath}")
                    print(f"  - Текст: {element.text}")
                    print(f"  - ID: {element.get_attribute('id')}")
                    break
            except Exception:
                continue
    
    # Если не нашли, ищем все кнопки на странице
    if not add_button:
        print("\n--- Поиск всех кнопок на странице ---")
        try:
            all_buttons = browser.find_elements(By.TAG_NAME, "button")
            print(f"Найдено кнопок: {len(all_buttons)}")
            for i, btn in enumerate(all_buttons[:10]):  # Показываем первые 10
                try:
                    if btn.is_displayed():
                        btn_id = btn.get_attribute('id')
                        btn_text = btn.text
                        btn_class = btn.get_attribute('class')
                        print(f"  Кнопка {i+1}: id='{btn_id}', text='{btn_text[:30]}', class='{btn_class[:50]}'")
                        # Проверяем, похожа ли на кнопку добавления
                        if btn_id == "c107" or "+" in btn_text or "добав" in btn_text.lower():
                            add_button = btn
                            print(f"  → Потенциальная кнопка добавления!")
                except Exception:
                    pass
        except Exception as e:
            print(f"Ошибка при поиске кнопок: {e}")

    if not add_button:
        print("\n✗ Кнопка 'Добавить ТС' не найдена ни по одному селектору!")
        print("Пробуем прямой переход на /vehicle/new")
        browser.get(f"{platform_url}/vehicle/new")
        time.sleep(2)
    else:
        print("\n" + "=" * 80)
        print("ШАГ 4: Клик на кнопку 'Добавить ТС'")
        print("=" * 80)
        try:
            add_button.click()
            print("✓ Клик выполнен")
            time.sleep(2)  # Даем время на открытие формы
            print(f"✓ URL после клика: {browser.current_url}")
        except Exception as e:
            print(f"✗ Ошибка при клике: {e}")
            print("Пробуем прямой переход на /vehicle/new")
            browser.get(f"{platform_url}/vehicle/new")
            time.sleep(2)

    print("\n" + "=" * 80)
    print("ШАГ 5: Проверка наличия элементов формы")
    print("=" * 80)
    print(f"Текущий URL: {browser.current_url}")

    # Проверяем контейнеры формы
    form_page = VehicleFormPage(browser, timeout=10)
    print("\n--- Проверка контейнеров формы ---")
    for selector in form_page.form_selectors:
        try:
            element = browser.find_element(By.CSS_SELECTOR, selector)
            print(f"✓ Найден контейнер: {selector}")
            print(f"  - Видим: {element.is_displayed()}")
            print(f"  - Текст: {element.text[:50] if element.text else 'нет текста'}")
        except Exception as e:
            print(f"✗ Контейнер '{selector}' не найден: {type(e).__name__}")

    # Проверяем поля формы
    print("\n--- Проверка полей формы (марка) ---")
    found_brand = False
    for selector in form_page.brand_selectors[:5]:  # Проверяем первые 5
        try:
            element = browser.find_element(By.CSS_SELECTOR, selector)
            if element.is_displayed():
                found_brand = True
                print(f"✓ Найдено поле марки: {selector}")
                print(f"  - Видим: {element.is_displayed()}")
                print(f"  - Placeholder: {element.get_attribute('placeholder')}")
                break
        except Exception as e:
            print(f"✗ Поле '{selector}' не найдено: {type(e).__name__}")
    if not found_brand:
        print("✗ Поле марки не найдено ни по одному селектору")

    # Проверяем кнопки сохранения
    print("\n--- Проверка кнопок сохранения ---")
    found_save = False
    for selector in form_page.save_button_selectors[:5]:  # Проверяем первые 5
        try:
            element = browser.find_element(By.CSS_SELECTOR, selector)
            if element.is_displayed():
                found_save = True
                print(f"✓ Найдена кнопка сохранения: {selector}")
                print(f"  - Текст: {element.text}")
                print(f"  - Видим: {element.is_displayed()}")
                break
        except Exception as e:
            print(f"✗ Кнопка '{selector}' не найдена: {type(e).__name__}")
    if not found_save:
        print("✗ Кнопка сохранения не найдена ни по одному селектору")

    print("\n" + "=" * 80)
    print("ШАГ 6: Проверка метода is_loaded()")
    print("=" * 80)
    try:
        is_loaded_result = form_page.is_loaded()
        print(f"✓ is_loaded() вернул: {is_loaded_result}")
    except Exception as e:
        print(f"✗ Ошибка в is_loaded(): {e}")

    print("\n" + "=" * 80)
    print("ШАГ 7: Попытка wait_loaded()")
    print("=" * 80)
    try:
        form_page.wait_loaded()
        print("✓ wait_loaded() успешно завершен")
    except Exception as e:
        print(f"✗ Ошибка в wait_loaded(): {type(e).__name__}: {e}")
        print("\n--- Дополнительная диагностика ---")
        print(f"Текущий URL: {browser.current_url}")
        print(f"Заголовок страницы: {browser.title}")
        try:
            body = browser.find_element(By.TAG_NAME, "body")
            print(f"Содержимое body (первые 500 символов):")
            print(body.text[:500])
        except Exception:
            print("Не удалось получить содержимое body")

    print("\n" + "=" * 80)
    print("ДИАГНОСТИКА ЗАВЕРШЕНА")
    print("=" * 80)

