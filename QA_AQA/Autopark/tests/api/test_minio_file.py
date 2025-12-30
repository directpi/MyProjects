"""
@file: test_minio_file.py
@description: API тесты для модуля файлов Minio (minio-file)
@dependencies: pytest, requests
@created: 2025-12-11
@author: Directpi
@test_type: api
"""

import pytest
import requests

from tests.conftest import attach_response, assert_status

BASE_PATH = "/api/v1/minio-file"


@pytest.mark.api
class TestMinioFile:
    """Тесты для работы с файлами Minio."""

    def test_get_minio_file_not_found(
        self, authenticated_session: requests.Session
    ):
        """Негативный: GET /minio-file/{name} с несуществующим файлом → 404."""
        resp = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}/non-existent-file-12345.txt",
            timeout=10,
        )
        attach_response("minio_file.get_not_found", resp)
        assert_status(
            resp,
            [404],
            context="GET /minio-file/{name} несуществующий файл",
            bug_map={
                403: "#BUG-MINIO-FILE-GET-403",
                500: "#BUG-MINIO-FILE-GET-500",
            },
        )

    def test_delete_minio_file_not_found(
        self, authenticated_session: requests.Session
    ):
        """Негативный: DELETE /minio-file/{name} с несуществующим файлом → 404."""
        resp = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}/non-existent-file-12345.txt",
            timeout=10,
        )
        attach_response("minio_file.delete_not_found", resp)
        assert_status(
            resp,
            [404, 204],
            context="DELETE /minio-file/{name} несуществующий файл",
            bug_map={
                403: "#BUG-MINIO-FILE-DELETE-403",
                500: "#BUG-MINIO-FILE-DELETE-500",
            },
        )


@pytest.mark.api
@pytest.mark.skip(reason="Minio file operations требуют Minio/S3; нет доступа в окружении")
class TestMinioFileOperations:
    """Тесты для операций с файлами Minio (требуют Minio/S3)."""

    def test_get_minio_file(self, authenticated_session: requests.Session):
        """GET /minio-file/{name} — получение файла."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")

    def test_delete_minio_file(self, authenticated_session: requests.Session):
        """DELETE /minio-file/{name} — удаление файла."""
        pytest.skip("Нет доступа к Minio/S3 в текущем окружении")





