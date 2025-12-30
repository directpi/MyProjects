"""
Страница формы обслуживания ТС (Page Object) для UI тестов.
"""

from __future__ import annotations

from typing import Iterable

from selenium.webdriver.common.by import By
from selenium.webdriver.remote.webdriver import WebDriver
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait, Select


class MaintenanceFormPage:
    """Page Object для формы создания/редактирования обслуживания ТС."""

    def __init__(self, driver: WebDriver, timeout: int = 15) -> None:
        self.driver = driver
        self.wait = WebDriverWait(driver, timeout)

        # Селекторы для полей формы
        self.vehicle_selectors = [
            'select[name="vehicle_id"]',
            '#vehicle_id',
            '[data-test="vehicle-select"]',
            'select.vehicle',
        ]
        self.description_selectors = [
            'textarea[name="description"]',
            'input[name="description"]',
            '#description',
            '[data-test="description"]',
        ]
        self.costs_selectors = [
            'input[name="costs"]',
            'input[name="cost"]',
            '#costs',
            '#cost',
            '[data-test="costs"]',
        ]
        self.status_selectors = [
            'select[name="status"]',
            '#status',
            '[data-test="status"]',
        ]
        self.save_button_selectors = [
            'button[type="submit"]',
            'button:contains("Сохранить")',
            'button.save',
            '[data-test="save"]',
        ]
        self.delete_button_selectors = [
            '[data-test="delete"]',
            'button.delete',
            'button:contains("Удалить")',
        ]

    def _find_first(self, selectors: Iterable[str]):
        for selector in selectors:
            try:
                return self.wait.until(EC.presence_of_element_located((By.CSS_SELECTOR, selector)))
            except Exception:
                continue
        raise AssertionError(f"Не найден элемент по селекторам: {selectors}")

    def is_loaded(self) -> bool:
        """Проверяет, загрузилась ли форма."""
        try:
            self._find_first(self.vehicle_selectors)
            return True
        except AssertionError:
            return False

    def wait_loaded(self) -> None:
        """Явное ожидание загрузки формы."""
        self.wait.until(lambda _: self.is_loaded())

    def set_vehicle(self, vehicle_id: int) -> None:
        """Выбирает ТС из списка."""
        el = self._find_first(self.vehicle_selectors)
        select = Select(el)
        select.select_by_value(str(vehicle_id))

    def set_description(self, description: str) -> None:
        """Заполняет поле 'Описание'."""
        el = self._find_first(self.description_selectors)
        el.clear()
        el.send_keys(description)

    def set_costs(self, costs: float) -> None:
        """Заполняет поле 'Затраты'."""
        el = self._find_first(self.costs_selectors)
        el.clear()
        el.send_keys(str(costs))

    def set_status(self, status: str) -> None:
        """Выбирает статус обслуживания."""
        try:
            el = self._find_first(self.status_selectors)
            select = Select(el)
            select.select_by_visible_text(status)
        except Exception:
            # Если статус не найден, пропускаем
            pass

    def click_save(self) -> None:
        """Нажимает кнопку 'Сохранить'."""
        btn = self._find_first(self.save_button_selectors)
        btn.click()

    def click_delete(self) -> None:
        """Нажимает кнопку 'Удалить'."""
        btn = self._find_first(self.delete_button_selectors)
        btn.click()

    def fill_form(self, vehicle_id: int, description: str, costs: float, status: str | None = None) -> None:
        """Заполняет всю форму."""
        self.set_vehicle(vehicle_id)
        self.set_description(description)
        self.set_costs(costs)
        if status:
            self.set_status(status)









