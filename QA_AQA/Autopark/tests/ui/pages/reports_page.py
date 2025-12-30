"""
Страница отчетов (Page Object) для UI тестов.
"""

from __future__ import annotations

from typing import Iterable

from selenium.webdriver.common.by import By
from selenium.webdriver.remote.webdriver import WebDriver
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait


class ReportsPage:
    """Page Object для страницы отчетов."""

    def __init__(self, driver: WebDriver, timeout: int = 15) -> None:
        self.driver = driver
        self.wait = WebDriverWait(driver, timeout)

        # Селекторы для элементов отчетов
        self.reports_container_selectors = [
            "[data-test='reports']",
            ".reports",
            "main",
            ".report-container",
        ]
        self.vehicle_report_selectors = [
            "[data-test='vehicle-report']",
            ".vehicle-report",
            "#vehicle-report",
        ]
        self.maintenance_report_selectors = [
            "[data-test='maintenance-report']",
            ".maintenance-report",
            "#maintenance-report",
        ]

    def _exists_any(self, selectors: Iterable[str]) -> bool:
        """Проверяет наличие хотя бы одного элемента с коротким таймаутом."""
        # Короткий таймаут (3 сек) для каждого селектора
        short_wait = WebDriverWait(self.driver, 3)
        for selector in selectors:
            try:
                short_wait.until(
                    EC.presence_of_element_located((By.CSS_SELECTOR, selector))
                )
                return True
            except Exception:
                continue
        return False

    def is_loaded(self) -> bool:
        """Проверяет, загрузилась ли страница отчетов."""
        return self._exists_any(self.reports_container_selectors)

    def wait_loaded(self) -> None:
        """Явное ожидание загрузки страницы отчетов."""
        self.wait.until(lambda _: self.is_loaded())

    def has_vehicle_report(self) -> bool:
        """Проверяет наличие отчета по ТС."""
        return self._exists_any(self.vehicle_report_selectors)

    def has_maintenance_report(self) -> bool:
        """Проверяет наличие отчета по обслуживанию."""
        return self._exists_any(self.maintenance_report_selectors)
