"""
@file: test_user_salary.py
@description: API тесты для модуля зарплат пользователей (user-salary)
@dependencies: pytest, requests
@created: 2025-12-11
@author: Directpi
@test_type: api
"""

from __future__ import annotations

import pytest
import requests

from tests.conftest import attach_response, assert_status

BASE_PATH = "/api/v1/user-salary"


@pytest.mark.api
class TestUserSalary:
    def test_user_salary(self, authenticated_session: requests.Session):
        """GET /user-salary — получение зарплаты пользователя."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            timeout=10,
        )
        attach_response("user_salary.get", resp)
        assert_status(
            resp,
            [200],
            context="GET /user-salary",
            bug_map={403: "#BUG-USER-SALARY-403"},
        )

    def test_user_salary_all(self, authenticated_session: requests.Session):
        """GET /user-salary/all — получение всех зарплат."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/all",
            timeout=10,
        )
        attach_response("user_salary.all", resp)
        assert_status(
            resp,
            [200],
            context="GET /user-salary/all",
            bug_map={403: "#BUG-USER-SALARY-ALL-403"},
        )

    def test_user_salary_my(self, authenticated_session: requests.Session):
        """GET /user-salary/my — зарплата текущего пользователя."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/my",
            timeout=10,
        )
        attach_response("user_salary.my", resp)
        assert_status(
            resp,
            [200, 404],
            context="GET /user-salary/my",
            bug_map={403: "#BUG-USER-SALARY-MY-403"},
        )

    def test_user_salary_drivers_excel(
        self, authenticated_session: requests.Session
    ):
        """GET /user-salary/drivers/excel — экспорт зарплат водителей в Excel."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/drivers/excel",
            timeout=10,
        )
        attach_response("user_salary.drivers_excel", resp)
        assert_status(
            resp,
            [200],
            context="GET /user-salary/drivers/excel",
            bug_map={
                403: "#BUG-USER-SALARY-DRIVERS-EXCEL-403",
                404: "#BUG-USER-SALARY-DRIVERS-EXCEL-404",
            },
        )









