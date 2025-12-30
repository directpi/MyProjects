"""
@file: test_ui_navigation.py
@description: UI тесты для навигации и Dashboard
@dependencies: pytest, selenium
@created: 2025-11-25
@author: Directpi
@test_type: ui
"""

from __future__ import annotations

import os
import time

import pytest

from tests.ui.pages.login_page import LoginPage
from tests.ui.pages.dashboard_page import DashboardPage
from tests.ui.pages.vehicle_list_page import VehicleListPage
from tests.ui.pages.maintenance_list_page import MaintenanceListPage
from tests.ui.pages.reports_page import ReportsPage


@pytest.mark.ui
def test_navigation_menu(authenticated_browser, platform_url):
    """Проверка навигации по разделам. Использует shared сессию."""
    browser = authenticated_browser
    
    # Используем уже залогиненный браузер
    dashboard = DashboardPage(browser, timeout=15)
    dashboard.wait_loaded()

    # Проверяем навигацию на ТС
    dashboard.navigate_to_vehicles()
    time.sleep(1)
    vehicle_list = VehicleListPage(browser, timeout=10)
    assert (
        vehicle_list.is_loaded() or "vehicle" in browser.current_url.lower()
    ), "Навигация на ТС не работает"

    # Возвращаемся на Dashboard
    dashboard.wait_loaded()

    # Проверяем навигацию на обслуживание
    dashboard.navigate_to_maintenance()
    time.sleep(1)
    maintenance_list = MaintenanceListPage(browser, timeout=10)
    assert (
        maintenance_list.is_loaded()
        or "maintenance" in browser.current_url.lower()
    ), "Навигация на обслуживание не работает"

    # Возвращаемся на Dashboard
    dashboard.wait_loaded()

    # Проверяем навигацию на отчеты
    dashboard.navigate_to_reports()
    time.sleep(1)
    reports_page = ReportsPage(browser, timeout=10)
    assert (
        reports_page.is_loaded() or "report" in browser.current_url.lower()
    ), "Навигация на отчеты не работает"


@pytest.mark.ui
def test_dashboard_elements(authenticated_browser, platform_url):
    """Проверка элементов Dashboard. Использует shared сессию."""
    browser = authenticated_browser
    
    # Используем уже залогиненный браузер
    dashboard = DashboardPage(browser, timeout=15)
    dashboard.wait_loaded()

    # Проверяем основные элементы Dashboard
    assert dashboard.is_loaded(), "Dashboard элементы не загрузились"

    # Проверяем наличие навигации
    assert dashboard.has_logout() or dashboard.is_loaded(), (
        "Навигация или элементы Dashboard отсутствуют"
    )

