"""
Страница формы ТС (Page Object) для UI тестов.
"""

from __future__ import annotations

from typing import Iterable

from selenium.webdriver.common.by import By
from selenium.webdriver.remote.webdriver import WebDriver
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait


class VehicleFormPage:
    """Page Object для формы создания/редактирования ТС."""

    def __init__(self, driver: WebDriver, timeout: int = 15) -> None:
        self.driver = driver
        self.wait = WebDriverWait(driver, timeout)

        # Контейнеры формы
        self.form_selectors = [
            '#c77',  # Контейнер модального окна (из DevTools)
            '.q-dialog',
            '.q-dialog .q-card',
            '#c219',
            '#c220',
            '[data-test="vehicle-form"]',
            'form',
        ]

        # Селекторы для полей формы (обновлены по правильным ID из HTML)
        # Марка: input #c134 (name="c134", placeholder="Введите марку")
        self.brand_selectors = [
            '#c134',  # Актуальный ID из HTML
            'input[id="c134"]',
            'input[name="c134"]',
            'input[placeholder="Введите марку"]',
            'input[placeholder*="марку"]',  # Из E2E тестов
            'input[name*="c134"]',
            'input.q-field__native[placeholder*="марку"]',  # noqa: E501
            # Fallback селекторы (старые ID)
            '#c302',  # Старый ID (fallback)
            'input[name*="c302"]',
            'input[name="brand"]',
            'input[name="марка"]',
            '#brand',
            '[data-test="brand"]',
        ]
        # Название (Модель): input #c136 (name="c136", placeholder="Введите название")
        self.model_selectors = [
            '#c136',  # Реальный ID из HTML (поле "Название")
            'input[id="c136"]',
            'input[name="c136"]',
            '#c304',  # Альтернативный ID
            'input[id="c304"]',
            'input[name="c304"]',
            'input[placeholder="Введите название"]',
            'input[placeholder*="название"]',  # Из E2E тестов
            'input[name*="c304"]',
            'input.q-field__native[placeholder*="название"]',  # noqa: E501
            # Fallback селекторы
            'input[name="model"]',
            'input[name="модель"]',
            '#model',
            '[data-test="model"]',
        ]
        # Гос. номер: input #c138 (name="c138", placeholder="Введите гос. номер")
        self.license_plate_selectors = [
            '#c138',  # Реальный ID из HTML (поле "Гос. номер")
            'input[id="c138"]',
            'input[name="c138"]',
            '#c306',  # Альтернативный ID
            'input[id="c306"]',
            'input[name="c306"]',
            'input[placeholder="Введите гос. номер"]',
            'input[placeholder*="номер"]',  # Из E2E тестов
            'input[name*="c306"]',
            'input.q-field__native[placeholder*="номер"]',  # noqa: E501
            # Fallback селекторы
            'input[name="license_plate"]',
            'input[name="гос_номер"]',
            '#license_plate',
            '[data-test="license-plate"]',
        ]
        self.save_button_selectors = [
            '#c154',  # ID кнопки "Создать" из DevTools (основной)
            '#c154 span.q-btn__content',  # Внутренний span кнопки
            'button#c154',  # Альтернативный формат
            '#c235',  # Альтернативный ID
            '#c235 span.q-btn__content',
            'button#c235',
            '#c295',  # Старый ID (fallback)
            'button[type="submit"]',
            'button.save',
            '[data-test="save"]',
            'button.q-btn',
            '#c256',
            'button:contains("Создать")',
            '.q-btn:contains("Создать")',
        ]

    def _find_first(self, selectors: Iterable[str]):
        """Находит первый доступный элемент из списка селекторов."""
        for selector in selectors:
            try:
                # Обработка XPath селекторов
                if selector.startswith("//"):
                    return self.wait.until(
                        EC.presence_of_element_located((By.XPATH, selector))
                    )
                else:
                    return self.wait.until(
                        EC.presence_of_element_located((By.CSS_SELECTOR, selector))
                    )
            except Exception:
                continue
        raise AssertionError(f"Не найден элемент по селекторам: {selectors}")

    def _exists_any(self, selectors: Iterable[str], timeout: int = 2) -> bool:
        """Быстрая проверка наличия хотя бы одного элемента (для is_loaded)."""
        short_wait = WebDriverWait(self.driver, timeout)
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
        """Проверяет, загрузилась ли форма (быстрая проверка с коротким таймаутом)."""
        try:
            # Быстрая проверка: используем короткий таймаут (2 сек) для каждого селектора
            # Загружена, если видим контейнер формы и поле или кнопку
            if not self._exists_any(self.form_selectors, timeout=2):
                return False
            # Проверяем наличие поля марки или кнопки сохранения
            return (
                self._exists_any(self.brand_selectors, timeout=1)
                or self._exists_any(self.save_button_selectors, timeout=1)
            )
        except Exception:
            return False

    def wait_loaded(self) -> None:
        """Явное ожидание загрузки формы ТС."""
        self.wait.until(lambda _: self.is_loaded())

    def set_brand(self, brand: str) -> None:
        """Заполняет поле 'Марка'."""
        el = self._find_first(self.brand_selectors)
        el.clear()
        el.send_keys(brand)

    def set_model(self, model: str) -> None:
        """Заполняет поле 'Модель'."""
        el = self._find_first(self.model_selectors)
        el.clear()
        el.send_keys(model)

    def set_license_plate(self, plate: str) -> None:
        """Заполняет поле 'Гос. номер'."""
        el = self._find_first(self.license_plate_selectors)
        el.clear()
        el.send_keys(plate)

    def click_save(self) -> None:
        """Нажимает кнопку 'Сохранить'."""
        btn = self._find_first(self.save_button_selectors)
        # Если нашли span внутри кнопки, поднимаемся к button
        if btn.tag_name == "span":
            btn = btn.find_element(By.XPATH, "./ancestor::button[1]")
        btn.click()

    def fill_form(self, brand: str, model: str, license_plate: str) -> None:
        """Заполняет всю форму."""
        self.set_brand(brand)
        self.set_model(model)
        self.set_license_plate(license_plate)
