"""
@file: test_report_file.py
@description: API тесты для модуля файлов отчетов (report-file)
@dependencies: pytest, requests
@created: 2025-12-11
@author: Directpi
@test_type: api
"""

import pytest
import requests

from tests.conftest import attach_response, assert_status

BASE_PATH = "/api/v1/report-file"


@pytest.mark.api
@pytest.mark.skip(reason="Report file upload требует Minio/S3; нет доступа в окружении")
class TestReportFile:
    """Тесты для загрузки файлов к отчетам (требуют Minio/S3)."""

    def test_upload_image(
        self, authenticated_session: requests.Session
    ):
        """POST /report-file/image — загрузка изображения к отчету."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")

    def test_upload_documents(
        self, authenticated_session: requests.Session
    ):
        """POST /report-file/documents — загрузка документов к отчету."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")


@pytest.mark.api
class TestReportFileValidation:
    """Тесты валидации для report-file (без загрузки файлов)."""

    def test_upload_image_missing_report_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: POST /report-file/image без report_id → 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}/image",
            timeout=10,
        )
        attach_response("report_file.image_missing_id", resp)
        assert_status(
            resp,
            [422, 400],
            context="POST /report-file/image без report_id",
            bug_map={
                403: "#BUG-REPORT-FILE-IMAGE-MISSING-ID-403",
                404: "#BUG-REPORT-FILE-IMAGE-MISSING-ID-404",
            },
        )

    def test_upload_documents_missing_report_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: POST /report-file/documents без report_id → 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}/documents",
            timeout=10,
        )
        attach_response("report_file.documents_missing_id", resp)
        assert_status(
            resp,
            [422, 400],
            context="POST /report-file/documents без report_id",
            bug_map={
                403: "#BUG-REPORT-FILE-DOCS-MISSING-ID-403",
                404: "#BUG-REPORT-FILE-DOCS-MISSING-ID-404",
            },
        )

