"""
Страница списка ТС (Page Object) для UI тестов.
"""

from __future__ import annotations

from typing import Iterable

from selenium.webdriver.common.by import By
from selenium.webdriver.remote.webdriver import WebDriver
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait


class VehicleListPage:
    """Page Object для списка транспортных средств."""

    def __init__(self, driver: WebDriver, timeout: int = 15) -> None:
        self.driver = driver
        self.wait = WebDriverWait(driver, timeout)

        # Селекторы для списка ТС
        self.list_selectors = [
            "#c115",  # ID таблицы ТС на dashboard (основной)
            ".q-table__container",  # Контейнер таблицы Quasar
            "#c77",  # ID карточки с ТС на dashboard
            "[data-test='vehicle-list']",
            ".vehicle-list",
            "table",
            ".table",
            "ul.vehicle-list",
            ".q-table",  # quasar table на дашборде/странице ТС
        ]
        self.add_button_selectors = [
            "#c82",  # ID кнопки добавления ТС (основной селектор из DevTools)
            "button#c82",  # Альтернативный формат
            "#c82 > span.q-btn__content",  # Полный путь из DevTools
            "#c82 span.q-btn__content",  # Внутренний span кнопки
            "#c78 #c82",  # Кнопка внутри контейнера #c78
            "[data-test='add-vehicle']",
            "button.add",
            "a.add",
            "//button[@id='c82']",
            "//button[.//span[contains(@class, 'block') and text()='+']]",  # span с классом block и текстом +
            "//button[.//span[text()='+']]",  # q-btn со знаком +
            "//button[.//i[contains(text(),'add')]]",
        ]

    def _find_first(self, selectors: Iterable[str]):
        """Находит первый доступный элемент с коротким таймаутом."""
        # Короткий таймаут (3 сек) для каждого селектора
        short_wait = WebDriverWait(self.driver, 3)
        for selector in selectors:
            try:
                return short_wait.until(
                    EC.presence_of_element_located((By.CSS_SELECTOR, selector))
                )
            except Exception:
                continue
        raise AssertionError(
            f"Не найден элемент по селекторам: {selectors}"
        )

    def is_loaded(self) -> bool:
        """Проверяет, загрузилась ли страница списка ТС."""
        try:
            # Проверяем наличие body
            try:
                body = self.driver.find_element(By.TAG_NAME, "body")
                if not body:
                    return False
            except Exception:
                return False
            # Пытаемся найти таблицу ТС (проверяем первые 3 селектора для быстроты)
            for selector in self.list_selectors[:3]:
                try:
                    element = self.driver.find_element(
                        By.CSS_SELECTOR, selector
                    )
                    if element and element.is_displayed():
                        return True
                except Exception:
                    continue
            # Если не нашли по основным селекторам, возвращаем False
            return False
        except Exception:
            return False

    def wait_loaded(self) -> None:
        """Явное ожидание загрузки списка ТС."""
        self.wait.until(lambda _: self.is_loaded())

    def click_add_vehicle(self) -> None:
        """Нажимает кнопку 'Добавить ТС'."""
        btn = self._find_first(self.add_button_selectors)
        # Если нашли span внутри кнопки, поднимаемся к button
        if btn.tag_name == "span":
            btn = btn.find_element(By.XPATH, "./ancestor::button[1]")
        btn.click()

    def get_vehicle_count(self) -> int:
        """Возвращает количество ТС в списке."""
        try:
            # Оптимизированный поиск: сначала ищем контейнер списка, затем элементы внутри
            list_container = None
            for selector in self.list_selectors:
                try:
                    list_container = self.driver.find_element(By.CSS_SELECTOR, selector)
                    if list_container:
                        break
                except Exception:
                    continue

            if list_container:
                # Ищем строки таблицы в tbody (исключаем заголовок)
                elements = list_container.find_elements(
                    By.CSS_SELECTOR,
                    "tbody tr.cursor-pointer, tbody tr, .vehicle-item, li"
                )
                return len([e for e in elements if e.is_displayed()])
            # Fallback: если контейнер не найден, возвращаем 0
            return 0
        except Exception:
            return 0
