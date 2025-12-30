"""
@file: test_report.py
@description: API тесты для модуля Report (создание, получение, обновление)
@dependencies: pytest, requests
@created: 2025-12-11
@autor: Directpi
@test_type: api
"""

from __future__ import annotations

from typing import Optional

import pytest
import requests

from tests.conftest import attach_response, assert_status

BASE_PATH = "/api/v1/report"

# Значения из ReportTypeEnum / ReportStatus
REPORT_TYPE_BUG = "Баг"
REPORT_STATUS_IN_PROGRESS = "В работе"


def _get_me_id(session: requests.Session) -> Optional[int]:
    resp = session.get(f"{session.base_url}/api/v1/user/me", timeout=10)
    attach_response("report.me", resp)
    if resp.status_code == 200:
        return resp.json().get("id")
    return None


@pytest.mark.api
class TestReportCrud:
    def test_create_report(
        self,
        authenticated_session: requests.Session,
    ):
        """Позитивный: создание репорта с обязательными полями."""
        me_id = _get_me_id(authenticated_session)
        if not me_id:
            pytest.skip("Нет доступа к /user/me для получения user_id")

        payload = {
            "user_id": me_id,
            "report_type": REPORT_TYPE_BUG,
            "message": "Автотест: баг отчет",
        }
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=payload,
            timeout=15,
        )
        attach_response("report.create", resp)
        assert_status(
            resp,
            [201],
            context="POST /report",
            bug_map={403: "#BUG-REPORT-CREATE-403"},
        )

        report_id = None
        if resp.status_code == 201:
            report_id = resp.json().get("id")

        # GET по report_id
        if report_id:
            get_resp = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}",
                params={"report_id": report_id},
                timeout=10,
            )
            attach_response("report.get_by_id", get_resp)
            assert_status(
                get_resp,
                [200],
                context="GET /report c report_id",
                bug_map={403: "#BUG-REPORT-GET-403"},
            )

    def test_update_report_status(
        self,
        authenticated_session: requests.Session,
    ):
        """Позитивный (по контракту): обновление статуса существующего репорта."""
        me_id = _get_me_id(authenticated_session)
        if not me_id:
            pytest.skip("Нет доступа к /user/me для получения user_id")

        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "user_id": me_id,
                "report_type": REPORT_TYPE_BUG,
                "message": "Обновление статуса",
            },
            timeout=15,
        )
        assert_status(
            create_resp,
            [201],
            context="POST /report перед PATCH",
            bug_map={403: "#BUG-REPORT-CREATE-403"},
        )
        report_id = create_resp.json().get("id") if create_resp.status_code == 201 else None
        if not report_id:
            pytest.skip("Не удалось создать репорт для обновления")

        patch_resp = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={
                "id": report_id,
                "status": REPORT_STATUS_IN_PROGRESS,
            },
            timeout=15,
        )
        attach_response("report.patch", patch_resp)
        assert_status(
            patch_resp,
            [200],
            context="PATCH /report",
            bug_map={403: "#BUG-REPORT-PATCH-403"},
        )


@pytest.mark.api
class TestReportRead:
    def test_get_reports_authorized(self, authenticated_session: requests.Session):
        """GET /report без фильтров (авторизованный пользователь)."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            timeout=10,
        )
        attach_response("report.get", resp)
        assert_status(
            resp,
            [200, 422],
            context="GET /report",
            bug_map={403: "#BUG-REPORT-GET-403"},
        )

    def test_get_reports_all(self, authenticated_session: requests.Session):
        """GET /report/all — по контракту 200, но может требовать роль директора."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/all",
            timeout=10,
        )
        attach_response("report.all", resp)
        assert_status(
            resp,
            [200],
            context="GET /report/all",
            bug_map={403: "#BUG-REPORT-ALL-403"},
        )


@pytest.mark.api
class TestReportValidation:
    def test_create_report_missing_required(
        self, authenticated_session: requests.Session
    ):
        """Негативный: отсутствие обязательных полей (user_id/report_type) -> 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json={"message": "no required"},
            timeout=10,
        )
        attach_response("report.create_invalid", resp)
        assert_status(
            resp,
            [422],
            context="POST /report без обязательных полей",
            bug_map={201: "#BUG-REPORT-CREATE-INVALID-201"},
        )


@pytest.mark.api
class TestReportAdditional:
    def test_delete_report_missing_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: DELETE /report без report_id → 422."""
        resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            timeout=10,
        )
        attach_response("report.delete_missing_id", resp)
        if resp.status_code == 403:
            pytest.xfail(
                "#BUG-REPORT-DELETE-403: нет прав на удаление без id"
            )
        assert_status(
            resp,
            [422],
            context="DELETE /report без report_id",
            bug_map={404: "#BUG-REPORT-DELETE-404"},
        )

    def test_report_logist_my(self, authenticated_session: requests.Session):
        """GET /report/logist/my — отчеты логиста."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/logist/my",
            timeout=10,
        )
        attach_response("report.logist_my", resp)
        assert_status(
            resp,
            [200, 404],
            context="GET /report/logist/my",
            bug_map={403: "#BUG-REPORT-LOGIST-MY-403"},
        )

    def test_report_stats_status_count(
        self, authenticated_session: requests.Session
    ):
        """GET /report/stats/status/count — статистика по статусам."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/stats/status/count",
            params={"period_days": 30},
            timeout=10,
        )
        attach_response("report.stats_status_count", resp)
        assert_status(
            resp,
            [200],
            context="GET /report/stats/status/count",
            bug_map={403: "#BUG-REPORT-STATUS-COUNT-403"},
        )

    def test_report_stats_status_count_logist(
        self, authenticated_session: requests.Session
    ):
        """GET /report/stats/status/count/logist — статистика логиста."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/stats/status/count/logist",
            params={"period_days": 30},
            timeout=10,
        )
        attach_response("report.stats_status_count_logist", resp)
        assert_status(
            resp,
            [200],
            context="GET /report/stats/status/count/logist",
            bug_map={403: "#BUG-REPORT-STATUS-COUNT-LOGIST-403"},
        )


@pytest.mark.api
@pytest.mark.skip(reason="Report file upload требует Minio/S3; нет доступа в окружении")
class TestReportFiles:
    def test_upload_report_document(self, authenticated_session: requests.Session):
        """Заглушка: загрузка документа к репорту (требуются креды Minio/S3)."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")

