"""
@file: test_vehicle_file.py
@description: API тесты для модуля файлов ТС (vehicle-file)
@dependencies: pytest, requests
@created: 2025-12-11
@author: Directpi
@test_type: api
"""

import pytest
import requests

from tests.conftest import attach_response, assert_status

BASE_PATH = "/api/v1/vehicle-file"


@pytest.mark.api
@pytest.mark.skip(reason="Vehicle file upload требует Minio/S3; нет доступа в окружении")
class TestVehicleFile:
    """Тесты для загрузки файлов ТС (требуют Minio/S3)."""

    def test_upload_vehicle_image(
        self, authenticated_session: requests.Session, vehicle_cleanup
    ):
        """POST /vehicle-file/image — загрузка изображения ТС."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")

    def test_upload_vehicle_documents(
        self, authenticated_session: requests.Session, vehicle_cleanup
    ):
        """POST /vehicle-file/documents — загрузка документов ТС."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")

    def test_upload_maintenance_image(
        self, authenticated_session: requests.Session, vehicle_cleanup
    ):
        """POST /vehicle-file/maintenance-image — загрузка изображения ТО."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")

    def test_upload_maintenance_documents(
        self, authenticated_session: requests.Session, vehicle_cleanup
    ):
        """POST /vehicle-file/maintenance-documents — загрузка документов ТО."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")


@pytest.mark.api
class TestVehicleFileValidation:
    """Тесты валидации для vehicle-file (без загрузки файлов)."""

    def test_upload_vehicle_image_missing_vehicle_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: POST /vehicle-file/image без vehicle_id → 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}/image",
            timeout=10,
        )
        attach_response("vehicle_file.image_missing_id", resp)
        assert_status(
            resp,
            [422, 400],
            context="POST /vehicle-file/image без vehicle_id",
            bug_map={
                403: "#BUG-VEHICLE-FILE-IMAGE-MISSING-ID-403",
                404: "#BUG-VEHICLE-FILE-IMAGE-MISSING-ID-404",
            },
        )

    def test_upload_vehicle_documents_missing_vehicle_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: POST /file/vehicle-file/documents без vehicle_id → 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}/documents",
            timeout=10,
        )
        attach_response("vehicle_file.documents_missing_id", resp)
        assert_status(
            resp,
            [422, 400],
            context="POST /vehicle-file/documents без vehicle_id",
            bug_map={
                403: "#BUG-VEHICLE-FILE-DOCS-MISSING-ID-403",
                404: "#BUG-VEHICLE-FILE-DOCS-MISSING-ID-404",
            },
        )

    def test_upload_maintenance_image_missing_vehicle_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: POST /vehicle-file/maintenance-image без vehicle_id → 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}/maintenance-image",
            timeout=10,
        )
        attach_response("vehicle_file.maintenance_image_missing_id", resp)
        assert_status(
            resp,
            [422, 400],
            context="POST /vehicle-file/maintenance-image без vehicle_id",
            bug_map={
                403: "#BUG-VEHICLE-FILE-MAINT-IMAGE-MISSING-ID-403",
                404: "#BUG-VEHICLE-FILE-MAINT-IMAGE-MISSING-ID-404",
            },
        )

    def test_upload_maintenance_documents_missing_vehicle_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: POST /vehicle-file/maintenance-documents без vehicle_id → 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}/maintenance-documents",
            timeout=10,
        )
        attach_response("vehicle_file.maintenance_documents_missing_id", resp)
        assert_status(
            resp,
            [422, 400],
            context="POST /vehicle-file/maintenance-documents без vehicle_id",
            bug_map={
                403: "#BUG-VEHICLE-FILE-MAINT-DOCS-MISSING-ID-403",
                404: "#BUG-VEHICLE-FILE-MAINT-DOCS-MISSING-ID-404",
            },
        )

