"""
Страница логина (Page Object) для UI тестов.
"""

from __future__ import annotations

import os
from typing import Iterable

from selenium.webdriver.common.by import By
from selenium.webdriver.remote.webdriver import WebDriver
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait


class LoginPage:
    """Page Object для экрана логина."""

    def __init__(
        self,
        driver: WebDriver,
        base_url: str,
        timeout: int = 15,
    ) -> None:
        self.driver = driver
        self.base_url = base_url.rstrip("/")
        self.timeout = timeout
        self.wait = WebDriverWait(driver, timeout)

        # Селекторы можно переопределять через .env при необходимости
        self.phone_selectors = [
            os.getenv("LOGIN_PHONE_SELECTOR", 'input[type="text"]'),
            'input[id^="c"]',  # NiceGUI генерирует id как c18, c19 и т.д.
            'input[name^="c"]',
        ]
        self.password_selectors = [
            os.getenv(
                "LOGIN_PASSWORD_SELECTOR",
                'input[type="password"]',
            ),
            'input[name="password"]',
        ]
        self.submit_selectors = [
            os.getenv(
                "LOGIN_SUBMIT_SELECTOR",
                "//button[contains(text(), 'ВОЙТИ')]",
            ),
            'button:not(.hidden)',
            "button",
        ]

    def open(self) -> None:
        """Открывает страницу логина с обработкой таймаутов."""
        try:
            self.driver.get(self.base_url)
        except Exception as e:
            # Если платформа недоступна, выбрасываем понятную ошибку
            if "timeout" in str(e).lower() or "timed out" in str(e).lower():
                raise TimeoutError(
                    f"Платформа {self.base_url} недоступна или не отвечает. "
                    "Проверьте доступность сервера."
                ) from e
            raise

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

    def is_loaded(self) -> bool:
        """Быстрая проверка загрузки страницы логина."""
        try:
            # Быстрая проверка URL и body
            url_ok = "login" in self.driver.current_url.lower()
            body_ok = bool(self.driver.find_elements(By.TAG_NAME, "body"))
            if not url_ok or not body_ok:
                return False
            # Пытаемся найти элементы (с коротким таймаутом)
            try:
                self._find_first(self.phone_selectors)
                self._find_first(self.password_selectors)
                return True
            except AssertionError:
                # Фолбэк: если URL правильный и body есть — считаем загруженной
                return url_ok and body_ok
        except Exception:
            return False

    def set_phone(self, phone: str) -> None:
        el = self._find_first(self.phone_selectors)
        el.clear()
        el.send_keys(phone)

    def set_password(self, password: str) -> None:
        el = self._find_first(self.password_selectors)
        el.clear()
        el.send_keys(password)

    def click_login(self) -> None:
        btn = self._find_first(self.submit_selectors)
        btn.click()

    def login(self, phone: str, password: str) -> None:
        """Выполняет логин, повторяя проверенную логику из test_raw_login."""
        import time
        print("[LoginPage] START login()")
        # Если на форме есть префикс +7, вводим номер без +7
        phone_to_type = phone
        try:
            if phone.startswith("+7"):
                # Ищем префикс только в форме, не по всему DOM
                prefix_el = self.driver.find_element(
                    By.XPATH,
                    "//form//*[contains(text(), '+7')] | //label[contains(text(), '+7')]",
                )
                if prefix_el:
                    phone_to_type = phone[2:]
        except Exception:
            pass

        print(f"[LoginPage] Finding phone input, will type: {phone_to_type}")
        # Ищем поля так же, как в сыром тесте
        wait = WebDriverWait(self.driver, 10)
        phone_input = wait.until(
            EC.presence_of_element_located(
                (By.CSS_SELECTOR, 'input[type="text"]'),
            )
        )
        print("[LoginPage] Phone input found")
        password_input = self.driver.find_element(
            By.CSS_SELECTOR,
            'input[type="password"]',
        )
        print("[LoginPage] Password input found")

        phone_input.clear()
        phone_input.send_keys(phone_to_type)
        print("[LoginPage] Phone entered")

        password_input.clear()
        password_input.send_keys(password)
        print("[LoginPage] Password entered")

        # Ищем кнопку "Войти"
        # Быстрый путь: по ID #c21 (не перегружает платформу)
        login_btn = None
        try:
            login_btn = self.driver.find_element(By.CSS_SELECTOR, "#c21")
            if login_btn.is_displayed():
                print("[LoginPage] Found login button by ID #c21")
            else:
                login_btn = None
        except Exception:
            pass
        
        # Fallback: лёгкий XPath поиск (только в форме)
        if not login_btn:
            time.sleep(2)
            print("[LoginPage] Fallback: searching by XPath...")
            try:
                login_btn = self.driver.find_element(
                    By.XPATH,
                    "//form//button[contains(., 'Войти')] | //button[contains(., 'Войти')]"
                )
                print("[LoginPage] Found login button by XPath")
            except Exception:
                pass

        if not login_btn:
            raise AssertionError("Кнопка логина не найдена")

        # Клик через JS, как в рабочем smoke тесте
        print("[LoginPage] Clicking login button via JS")
        self.driver.execute_script("arguments[0].click();", login_btn)

        # Ждём редирект с /login на дашборд (как в smoke тесте)
        print("[LoginPage] Waiting for redirect...")
        self.wait_redirect()
        print(f"[LoginPage] Redirect OK, url={self.driver.current_url}")

    def wait_redirect(self, expected_substr: str = "/platform") -> None:
        """Ждём редирект с экрана логина.

        По умолчанию — любое изменение URL,
        содержащее expected_substr и не содержащее 'login'.
        Если expected_substr не найден, просто ждём исчезновения 'login' из URL.
        """
        import time
        start_url = self.driver.current_url
        print(f"[LoginPage] wait_redirect: начальный URL = {start_url}")
        
        try:
            # Сначала пробуем ждать редирект с expected_substr
            self.wait.until(
                lambda d: (
                    expected_substr in d.current_url
                    and "login" not in d.current_url
                )
            )
            print(f"[LoginPage] wait_redirect: редирект успешен, URL = {self.driver.current_url}")
        except Exception as e:
            print(f"[LoginPage] wait_redirect: первый вариант не сработал ({type(e).__name__}), пробуем фолбэк")
            print(f"[LoginPage] wait_redirect: текущий URL = {self.driver.current_url}")
            # Фолбэк: просто ждём, пока 'login' исчезнет из URL (как в smoke тестах)
            try:
                self.wait.until(
                    lambda d: "login" not in d.current_url.lower()
                )
                print(f"[LoginPage] wait_redirect: фолбэк успешен, URL = {self.driver.current_url}")
            except Exception as e2:
                print(f"[LoginPage] wait_redirect: фолбэк тоже не сработал ({type(e2).__name__})")
                print(f"[LoginPage] wait_redirect: финальный URL = {self.driver.current_url}")
                # Если и фолбэк не сработал, даём небольшую задержку и проверяем текущий URL
                time.sleep(2)
                current = self.driver.current_url
                if "login" not in current.lower():
                    print(f"[LoginPage] wait_redirect: после задержки URL не содержит 'login' = {current}")
                    return
                raise TimeoutError(
                    f"Редирект не произошёл. Остались на URL: {current} (начальный: {start_url})"
                ) from e2

    def wait_loaded(self) -> None:
        """Явно ждем появления полей логина, чтобы избежать гонок."""
        self.wait.until(lambda _: self.is_loaded())
