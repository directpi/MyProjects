"""
@file: test_driver_order_file.py
@description: API тесты для модуля файлов заказов водителя (driver-order-file)
@dependencies: pytest, requests
@created: 2025-12-11
@author: Directpi
@test_type: api
"""

import pytest
import requests

from tests.conftest import attach_response, assert_status

BASE_PATH = "/api/v1/driver-order-file"


@pytest.mark.api
@pytest.mark.skip(reason="Driver order file upload требует Minio/S3; нет доступа в окружении")
class TestDriverOrderFile:
    """Тесты для загрузки файлов к заказам водителя (требуют Minio/S3)."""

    def test_upload_image_of_load(
        self, authenticated_session: requests.Session
    ):
        """POST /driver-order-file/image-of-load — загрузка изображения загрузки."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")

    def test_upload_image_of_unload(
        self, authenticated_session: requests.Session
    ):
        """POST /driver-order-file/image-of-unload — загрузка изображения разгрузки."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")

    def test_upload_documents(
        self, authenticated_session: requests.Session
    ):
        """POST /driver-order-file/documents — загрузка документов."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")


@pytest.mark.api
class TestDriverOrderFileValidation:
    """Тесты валидации для driver-order-file (без загрузки файлов)."""

    def test_upload_image_missing_order_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: POST /driver-order-file/image-of-load без order_id → 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}/image-of-load",
            timeout=10,
        )
        attach_response("driver_order_file.image_missing_id", resp)
        assert_status(
            resp,
            [422, 400],
            context="POST /driver-order-file/image-of-load без order_id",
            bug_map={
                403: "#BUG-DRIVER-ORDER-FILE-IMAGE-MISSING-ID-403",
                404: "#BUG-DRIVER-ORDER-FILE-IMAGE-MISSING-ID-404",
            },
        )

    def test_upload_unload_image_missing_order_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: POST /driver-order-file/image-of-unload без order_id → 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}/image-of-unload",
            timeout=10,
        )
        attach_response("driver_order_file.image_unload_missing_id", resp)
        assert_status(
            resp,
            [422, 400],
            context="POST /driver-order-file/image-of-unload без order_id",
            bug_map={
                403: "#BUG-DRIVER-ORDER-FILE-IMAGE-UNLOAD-MISSING-ID-403",
                404: "#BUG-DRIVER-ORDER-FILE-IMAGE-UNLOAD-MISSING-ID-404",
            },
        )

    def test_upload_documents_missing_order_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: POST /driver-order-file/documents без order_id → 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}/documents",
            timeout=10,
        )
        attach_response("driver_order_file.documents_missing_id", resp)
        assert_status(
            resp,
            [422, 400],
            context="POST /driver-order-file/documents без order_id",
            bug_map={
                403: "#BUG-DRIVER-ORDER-FILE-DOCS-MISSING-ID-403",
                404: "#BUG-DRIVER-ORDER-FILE-DOCS-MISSING-ID-404",
            },
        )

