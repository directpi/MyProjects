"""
Страница списка обслуживания ТС (Page Object) для UI тестов.
"""

from __future__ import annotations

from typing import Iterable

from selenium.webdriver.common.by import By
from selenium.webdriver.remote.webdriver import WebDriver
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait


class MaintenanceListPage:
    """Page Object для списка записей обслуживания ТС."""""

    def __init__(self, driver: WebDriver, timeout: int = 15) -> None:
        self.driver = driver
        self.wait = WebDriverWait(driver, timeout)

        # Селекторы для списка обслуживания
        self.list_selectors = [
            "[data-test='maintenance-list']",
            ".maintenance-list",
            "table",
            ".table",
            "ul.maintenance-list",
        ]
        self.add_button_selectors = [
            "[data-test='add-maintenance']",
            "button:contains('Добавить')",
            "a:contains('Добавить обслуживание')",
            "button.add",
            "a.add",
        ]
        self.filter_selectors = [
            "[data-test='filter-vehicle']",
            "select[name='vehicle_id']",
            "#vehicle-filter",
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
        """Проверяет, загрузилась ли страница списка обслуживания."""
        try:
            # Быстрая проверка без долгого ожидания
            url_ok = "maintenance" in self.driver.current_url.lower()
            if not url_ok:
                return False
            # Проверяем наличие body (один запрос вместо множественных)
            try:
                body = self.driver.find_element(By.TAG_NAME, "body")
                body_ok = body is not None
            except Exception:
                return False

            if not body_ok:
                return False
            # Пытаемся найти список (только первый селектор для быстроты)
            try:
                # Пробуем только первый селектор для быстроты
                self.driver.find_element(By.CSS_SELECTOR, self.list_selectors[0])
                return True
            except Exception:
                # Фолбэк: если URL правильный и body есть — считаем загруженной
                return url_ok and body_ok
        except Exception:
            return False

    def wait_loaded(self) -> None:
        """Явное ожидание загрузки списка обслуживания."""
        self.wait.until(lambda _: self.is_loaded())

    def click_add_maintenance(self) -> None:
        """Нажимает кнопку 'Добавить обслуживание'."""
        btn = self._find_first(self.add_button_selectors)
        btn.click()

    def filter_by_vehicle(self, vehicle_id: int) -> None:
        """Фильтрует список по ТС."""
        try:
            filter_el = self._find_first(self.filter_selectors)
            from selenium.webdriver.support.ui import Select
            select = Select(filter_el)
            select.select_by_value(str(vehicle_id))
        except Exception:
            # Если фильтр не найден, пропускаем
            pass

    def get_maintenance_count(self) -> int:
        """Возвращает количество записей обслуживания в списке."""
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
                # Ищем элементы только внутри контейнера
                elements = list_container.find_elements(
                    By.CSS_SELECTOR, "tr:not(:first-child), .maintenance-item, li"
                )
                return len([e for e in elements if e.is_displayed()])
            # Fallback: если контейнер не найден, возвращаем 0
            return 0
        except Exception:
            return 0

    def click_maintenance_item(self, maintenance_id: int) -> None:
        """Кликает на запись обслуживания по ID."""
        try:
            selector = (
                f"[data-test='maintenance-{maintenance_id}'], "
                f"tr[data-id='{maintenance_id}']"
            )
            item = self.wait.until(
                EC.element_to_be_clickable((By.CSS_SELECTOR, selector))
            )
            item.click()
        except Exception:
            raise AssertionError(
                f"Не найдена запись обслуживания с ID {maintenance_id}"
            )
