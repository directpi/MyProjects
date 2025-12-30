"""
Страница Dashboard (Page Object) для UI тестов.
"""

from __future__ import annotations

from typing import Iterable

from selenium.webdriver.common.by import By
from selenium.webdriver.remote.webdriver import WebDriver
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait


class DashboardPage:
    """Page Object для Dashboard."""

    def __init__(self, driver: WebDriver, timeout: int = 15) -> None:
        self.driver = driver
        self.wait = WebDriverWait(driver, timeout)

        # Базовый набор селекторов
        # При необходимости можно расширить/переопределить
        self.nav_selectors = [
            "nav",
            "header",
        ]
        self.content_selectors = [
            "main",
            ".dashboard",
            ".card",
        ]
        # Селекторы для иконки аккаунта (открывает меню)
        # Кнопка с классом q-btn, содержащая иконку account_circle
        self.account_icon_selectors = [
            # Кнопка с иконкой account_circle (основной селектор)
            (
                "//button[contains(@class, 'q-btn') "
                "and .//i[contains(@class, 'material-icons') "
                "and text()='account_circle']]"
            ),
            (
                "//button[contains(@class, 'q-btn') "
                "and .//i[text()='account_circle']]"
            ),
            # Иконка account_circle, затем родительская кнопка
            (
                "//i[contains(@class, 'material-icons') "
                "and text()='account_circle']/ancestor::button[1]"
            ),
            (
                "//i[contains(@class, 'q-icon') "
                "and contains(@class, 'material-icons') "
                "and text()='account_circle']/ancestor::button[1]"
            ),
            # Иконка напрямую (fallback)
            (
                "//i[contains(@class, 'material-icons') "
                "and text()='account_circle']"
            ),
            "i.q-icon.notranslate.material-icons",
            "i.q-icon.material-icons",
            # Кнопка q-btn (более общий поиск)
            "button.q-btn",
            "button[class*='q-btn']",
            # Родительский элемент через XPath
            (
                "//i[contains(@class, 'material-icons') "
                "and contains(text(), 'account_circle')]/.."
            ),
            (
                "//i[contains(@class, 'material-icons') "
                "and contains(text(), 'account_circle')]/parent::*"
            ),
        ]
        # Селекторы для пункта "Выйти из системы" в выпадающем меню
        self.logout_menu_item_selectors = [
            # Точный поиск по тексту
            (
                "//span[contains(@class, 'block') "
                "and contains(text(), 'Выйти из системы')]"
            ),
            "//span[contains(text(), 'Выйти из системы')]",
            # Поиск через translate для регистронезависимости
            (
                "//span[contains(translate(text(), "
                "'АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ', "
                "'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'), "
                "'выйти из системы')]"
            ),
            # Fallback: любой элемент с текстом "Выйти"
            (
                "//*[contains(translate(text(), "
                "'АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ', "
                "'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'), 'выйти')]"
            ),
        ]
        # Старые селекторы (fallback для других вариантов UI)
        self.logout_selectors = [
            "[data-test='logout']",
            "button.logout",
            "a.logout",
            # Поиск через текст (XPath) - работает в Selenium
            (
                "//button[contains(translate(text(), "
                "'АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ', "
                "'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'), 'выход')]"
            ),
            (
                "//button[contains(translate(text(), "
                "'АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ', "
                "'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'), 'выйти')]"
            ),
            (
                "//a[contains(translate(text(), "
                "'АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ', "
                "'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'), 'выход')]"
            ),
            (
                "//a[contains(translate(text(), "
                "'АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ', "
                "'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'), 'выйти')]"
            ),
            # Поиск через aria-label (CSS)
            "[aria-label*='выход']",
            "[aria-label*='logout']",
            "[aria-label*='Выход']",
            "[aria-label*='Logout']",
        ]

    def _exists_any(self, selectors: Iterable[str], timeout: int = 2) -> bool:
        """Проверяет наличие хотя бы одного селектора."""
        short_wait = WebDriverWait(self.driver, timeout)
        for selector in selectors:
            try:
                # Поддержка XPath селекторов
                if selector.startswith("//"):
                    short_wait.until(
                        EC.presence_of_element_located((By.XPATH, selector))
                    )
                else:
                    short_wait.until(
                        EC.presence_of_element_located(
                            (By.CSS_SELECTOR, selector)
                        )
                    )
                return True
            except Exception:
                continue
        return False

    def is_loaded(self) -> bool:
        """Быстрая проверка загрузки Dashboard."""
        try:
            url_ok = "login" not in self.driver.current_url.lower()
            if not url_ok:
                return False
            # Оптимизированная проверка: используем find_element вместо find_elements
            try:
                body = self.driver.find_element(By.TAG_NAME, "body")
                body_ok = body is not None
            except Exception:
                return False
            # Дополнительно считаем успехом, если есть nav/main (оптимизировано)
            try:
                has_nav = self.driver.find_element(By.TAG_NAME, "nav") is not None
            except Exception:
                has_nav = False
            try:
                has_main = self.driver.find_element(By.TAG_NAME, "main") is not None
            except Exception:
                has_main = False
            return url_ok and (body_ok or has_nav or has_main)
        except Exception:
            return False

    def wait_loaded(self) -> None:
        """Явное ожидание загрузки Dashboard."""
        # Используем короткий таймаут для быстрой проверки
        short_wait = WebDriverWait(self.driver, 10)
        short_wait.until(lambda _: self.is_loaded())

    def has_logout(self) -> bool:
        """
        Проверяет наличие возможности выхода.

        Проверяет иконку аккаунта или прямую кнопку выхода.
        """
        return (
            self._exists_any(self.account_icon_selectors)
            or self._exists_any(self.logout_selectors)
        )

    def logout(self) -> None:
        """
        Выполняет выход из системы.

        Логика:
        1. Ищет иконку account_circle (открывает меню)
        2. Кликает на неё
        3. Ждёт появления выпадающего меню
        4. Ищет пункт "Выйти из системы"
        5. Кликает на него
        6. Ждёт редирект на /login

        Fallback: если иконка не найдена, пытается найти прямую кнопку выхода.
        """
        import time

        # Вариант 1: Через меню аккаунта (основной путь)
        # Даём время на загрузку страницы
        time.sleep(1)

        account_icon = None
        try:
            account_icon = self._find_first(self.account_icon_selectors)
        except AssertionError:
            pass

        if account_icon:
            # Кликаем на элемент аккаунта
            try:
                # Если это иконка <i>, ищем родительскую кнопку
                if account_icon.tag_name.lower() == "i":
                    # Ищем родительскую кнопку (может быть не сразу родитель)
                    try:
                        # Пробуем найти кнопку среди предков
                        parent = account_icon.find_element(
                            By.XPATH, "./ancestor::button[1]"
                        )
                        if parent:
                            # Кликаем через JS для надёжности
                            self.driver.execute_script(
                                "arguments[0].click();", parent
                            )
                        else:
                            # Fallback: кликаем на иконку
                            self.driver.execute_script(
                                "arguments[0].click();", account_icon
                            )
                    except Exception:
                        # Если не удалось найти кнопку, кликаем на иконку
                        self.driver.execute_script(
                            "arguments[0].click();", account_icon
                        )
                else:
                    # Если это уже кнопка, кликаем на неё
                    self.driver.execute_script(
                        "arguments[0].click();", account_icon
                    )
            except Exception:
                # Если не удалось кликнуть, пробуем обычный клик
                try:
                    account_icon.click()
                except Exception:
                    # Последняя попытка через JS
                    self.driver.execute_script(
                        "arguments[0].click();", account_icon
                    )

            # Ждём появления выпадающего меню
            time.sleep(0.5)  # Небольшая задержка для анимации

            # Ищем пункт "Выйти из системы" в меню
            try:
                logout_item = self._find_first(
                    self.logout_menu_item_selectors
                )
                # Кликаем на пункт меню
                logout_item.click()

                # Ждём редирект на страницу логина
                # После logout мы должны быть на /login
                import time
                time.sleep(1)  # Даём время на редирект

                # Проверяем, что мы на login
                current = self.driver.current_url
                if "login" in current.lower():
                    return

                # Если редирект не произошёл автоматически,
                # пробуем явный переход
                try:
                    self.wait.until(
                        lambda d: "login" in d.current_url.lower()
                    )
                    return
                except Exception:
                    # Если и ожидание не помогло, делаем явный переход
                    from urllib.parse import urlparse, urlunparse
                    parsed = urlparse(current)
                    base_url = urlunparse(
                        (parsed.scheme, parsed.netloc, "", "", "", "")
                    )
                    self.driver.get(f"{base_url}/login")
                    return
            except AssertionError:
                # Если пункт меню не найден, пробуем fallback
                pass

        # Вариант 2: Fallback - прямая кнопка выхода (старая логика)
        if self._exists_any(self.logout_selectors):
            btn = self._find_first(self.logout_selectors)
            btn.click()
            # Ждём редирект на страницу логина
            # После logout мы должны быть на /login
            import time
            time.sleep(1)  # Даём время на редирект

            # Проверяем, что мы на login
            current = self.driver.current_url
            if "login" in current.lower():
                return

            # Если редирект не произошёл автоматически, пробуем ожидание
            try:
                self.wait.until(
                    lambda d: "login" in d.current_url.lower()
                )
                return
            except Exception:
                # Если и ожидание не помогло, делаем явный переход
                from urllib.parse import urlparse, urlunparse
                parsed = urlparse(current)
                base_url = urlunparse(
                    (parsed.scheme, parsed.netloc, "", "", "", "")
                )
                self.driver.get(f"{base_url}/login")
                return

        # Если ничего не найдено
        raise AssertionError(
            "Не найдена иконка аккаунта для открытия меню выхода "
            "или прямая кнопка выхода"
        )

    def navigate_to_vehicles(self) -> None:
        """Переходит на страницу ТС."""
        nav_links = [
            "[data-test='nav-vehicles']",
            "a[href*='vehicle']",
            "nav a:contains('Транспорт')",
        ]
        try:
            link = self._find_first(nav_links)
            link.click()
        except Exception:
            # Если навигация не найдена, используем прямой переход
            base_url = self.driver.current_url.rsplit('/', 1)[0]
            self.driver.get(f"{base_url}/vehicle")

    def navigate_to_maintenance(self) -> None:
        """Переходит на страницу обслуживания."""
        nav_links = [
            "[data-test='nav-maintenance']",
            "a[href*='maintenance']",
            "nav a:contains('Обслуживание')",
        ]
        try:
            link = self._find_first(nav_links)
            link.click()
        except Exception:
            base_url = self.driver.current_url.rsplit('/', 1)[0]
            self.driver.get(f"{base_url}/maintenance")

    def navigate_to_reports(self) -> None:
        """Переходит на страницу отчетов."""
        nav_links = [
            "[data-test='nav-reports']",
            "a[href*='report']",
            "nav a:contains('Отчеты')",
        ]
        try:
            link = self._find_first(nav_links)
            link.click()
        except Exception:
            base_url = self.driver.current_url.rsplit('/', 1)[0]
            self.driver.get(f"{base_url}/reports")

    def _find_first(self, selectors: Iterable[str]):
        """Находит первый доступный элемент с коротким таймаутом."""
        # Короткий таймаут (3 сек) для каждого селектора
        short_wait = WebDriverWait(self.driver, 3)
        for selector in selectors:
            try:
                # Поддержка XPath селекторов
                if selector.startswith("//"):
                    return short_wait.until(
                        EC.presence_of_element_located((By.XPATH, selector))
                    )
                return short_wait.until(
                    EC.presence_of_element_located((By.CSS_SELECTOR, selector))
                )
            except Exception:
                continue
        raise AssertionError(
            f"Не найден элемент по селекторам: {selectors}"
        )
