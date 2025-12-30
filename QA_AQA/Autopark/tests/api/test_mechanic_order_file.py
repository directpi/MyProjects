"""
@file: test_mechanic_order_file.py
@description: API тесты для модуля файлов заказов механика (mechanic-order-file)
@dependencies: pytest, requests
@created: 2025-12-11
@author: Directpi
@test_type: api
"""

import pytest
import requests

from tests.conftest import attach_response, assert_status

BASE_PATH = "/api/v1/mechanic-order-file"


@pytest.mark.api
@pytest.mark.skip(reason="Mechanic order file upload требует Minio/S3; нет доступа в окружении")
class TestMechanicOrderFile:
    """Тесты для загрузки файлов к заказам механика (требуют Minio/S3)."""

    def test_upload_image(
        self, authenticated_session: requests.Session
    ):
        """POST /mechanic-order-file/image — загрузка изображения."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")


@pytest.mark.api
class TestMechanicOrderFileValidation:
    """Тесты валидации для mechanic-order-file (без загрузки файлов)."""

    def test_upload_image_missing_order_id(
        self, authenticated_session: requests.Session
    ):
        """Негативный: POST /mechanic-order-file/image без order_id → 422."""
        resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}/image",
            timeout=10,
        )
        attach_response("mechanic_order_file.image_missing_id", resp)
        assert_status(
            resp,
            [422, 400],
            context="POST /mechanic-order-file/image без order_id",
            bug_map={
                403: "#BUG-MECHANIC-ORDER-FILE-IMAGE-MISSING-ID-403",
                404: "#BUG-MECHANIC-ORDER-FILE-IMAGE-MISSING-ID-404",
            },
        )





