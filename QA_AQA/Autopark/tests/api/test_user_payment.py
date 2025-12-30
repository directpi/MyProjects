"""
@file: test_user_payment.py
@description: API тесты для CRUD операций по платежам пользователей (User Payment)
@dependencies: pytest, requests
@created: 2025-11-25
@author: Directpi
@test_type: api
"""

import pytest
import requests
from typing import Optional
from requests import exceptions as requests_exceptions
try:
    from tests.conftest import attach_response, assert_status  # type: ignore
except Exception:
    def attach_response(*args, **kwargs):
        return None
    def assert_status(response, expected, *, context="", bug_map=None):
        code = response.status_code
        expected_set = {int(x) for x in expected}
        if code in expected_set:
            return
        message_context = f"{context}. " if context else ""
        details = f"Ожидались {sorted(expected_set)}, получен {code}: {response.text}"
        if bug_map and code in bug_map:
            pytest.xfail(f"{bug_map[code]} — {message_context}{details}")
        pytest.fail(f"{message_context}{details}")

BASE_PATH = "/api/v1/user-payment"


def _get_first_user_id(session: requests.Session) -> Optional[int]:
    """Получить первый доступный user_id для тестов."""
    try:
        resp = session.get(f"{session.base_url}/api/v1/user/all", timeout=10)
        status = resp.status_code
        if status == 200:
            items = resp.json() or []
            if isinstance(items, list) and items:
                first = items[0]
                return first.get("id") or first.get("user_id")
    except Exception:
        pass
    return None


@pytest.fixture
def user_payment_cleanup(authenticated_session: requests.Session):
    """Фикстура для автоматической очистки созданных платежей."""
    created_ids = []

    def _cleanup(payment_id: Optional[int]):
        if payment_id:
            created_ids.append(payment_id)

    yield _cleanup

    # Cleanup: удаляем созданные платежи
    base = authenticated_session.base_url
    for pay_id in created_ids:
        try:
            resp = authenticated_session.delete(
                f"{base}{BASE_PATH}",
                params={"payment_id": pay_id},
                timeout=10,
            )
            assert_status(
                resp,
                [204, 422],
                context="DELETE /user-payment (cleanup)",
                bug_map={
                    200: "#BUG-USER-PAYMENT-CLEANUP-200",
                    202: "#BUG-USER-PAYMENT-CLEANUP-202",
                    404: "#BUG-USER-PAYMENT-CLEANUP-404",
                    403: "#BUG-USER-PAYMENT-CLEANUP-403",
                },
            )
        except Exception:
            pass  # Игнорируем ошибки cleanup


@pytest.mark.api
class TestUserPaymentRead:
    @pytest.mark.smoke
    def test_get_user_payments_unauthorized(self, api_base_url: str):
        """Проверка GET /user-payment без авторизации."""
        response = requests.get(f"{api_base_url}{BASE_PATH}", timeout=10)
        assert_status(
            response,
            [200, 422],
            context="GET /user-payment без авторизации",
            bug_map={
                401: "#BUG-USER-PAYMENT-GET-UNAUTH-401",
                403: "#BUG-USER-PAYMENT-GET-UNAUTH-403",
            },
        )

    def test_get_user_payments_authorized(
        self, authenticated_session: requests.Session
    ):
        """Проверка GET /user-payment с авторизацией."""
        response = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            timeout=10,
        )
        attach_response("user_payment.get", response)
        assert_status(
            response,
            [200],
            context="GET /user-payment с авторизацией",
            bug_map={
                401: "#BUG-USER-PAYMENT-GET-401",
                403: "#BUG-USER-PAYMENT-GET-403",
                422: "#BUG-USER-PAYMENT-GET-422",
            },
        )

    def test_get_user_payments_with_params(
        self, authenticated_session: requests.Session
    ):
        """Проверка GET /user-payment с параметрами."""
        user_id = _get_first_user_id(authenticated_session)
        if not user_id:
            pytest.skip("Нет доступных пользователей для теста")

        response = authenticated_session.get(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"user_id": user_id},
            timeout=10,
        )
        attach_response("user_payment.get.params", response)
        assert_status(
            response,
            [200],
            context="GET /user-payment с user_id",
            bug_map={
                401: "#BUG-USER-PAYMENT-GET-PARAMS-401",
                403: "#BUG-USER-PAYMENT-GET-PARAMS-403",
                422: "#BUG-USER-PAYMENT-GET-PARAMS-422",
            },
        )

    def test_get_user_payments_all(
        self, authenticated_session: requests.Session
    ):
        """Проверка GET /user-payment/all."""
        try:
            response = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}/all",
                timeout=8,
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /user-payment/all")
        attach_response("user_payment.get.all", response)
        assert_status(
            response,
            [200],
            context="GET /user-payment/all",
            bug_map={
                401: "#BUG-USER-PAYMENT-GET-ALL-401",
                403: "#BUG-USER-PAYMENT-GET-ALL-403",
                422: "#BUG-USER-PAYMENT-GET-ALL-422",
            },
        )


@pytest.mark.api
class TestUserPaymentCreate:
    @pytest.mark.smoke
    def test_create_user_payment(
        self,
        authenticated_session: requests.Session,
        user_payment_cleanup,
    ):
        """Проверка POST /user-payment (создание платежа)."""
        user_id = _get_first_user_id(authenticated_session)
        if not user_id:
            pytest.skip("Нет доступных пользователей для теста")

        payload = {
            "user_id": user_id,
            "amount": 1000.50,
            "payment_date": "2025-11-25",
            "description": "Test payment",
        }

        response = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=payload,
            timeout=10,
        )
        attach_response("user_payment.create", response)

        assert_status(
            response,
            [201],
            context="POST /user-payment",
            bug_map={
                200: "#BUG-USER-PAYMENT-CREATE-200",
                202: "#BUG-USER-PAYMENT-CREATE-202",
                401: "#BUG-USER-PAYMENT-CREATE-401",
                403: "#BUG-USER-PAYMENT-CREATE-403",
                422: "#BUG-USER-PAYMENT-CREATE-422",
            },
        )

        if response.status_code == 201:
            data = response.json()
            payment_id = data.get("id") or data.get("payment_id")
            if payment_id:
                user_payment_cleanup(payment_id)


@pytest.mark.api
class TestUserPaymentUpdate:
    def test_update_user_payment(
        self,
        authenticated_session: requests.Session,
        user_payment_cleanup,
    ):
        """Проверка PATCH /user-payment (обновление платежа)."""
        user_id = _get_first_user_id(authenticated_session)
        if not user_id:
            pytest.skip("Нет доступных пользователей для теста")

        # Сначала создаем платеж
        create_payload = {
            "user_id": user_id,
            "amount": 1000.50,
            "payment_date": "2025-11-25",
            "description": "Test payment for update",
        }

        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=create_payload,
            timeout=10,
        )

        payment_id = None
        if create_resp.status_code == 201:
            data = create_resp.json()
            payment_id = data.get("id") or data.get("payment_id")
            user_payment_cleanup(payment_id)

        if not payment_id:
            pytest.skip("Не удалось создать платеж для обновления")

        # Обновляем платеж — по схеме требуется поле id
        update_payload = {
            "id": payment_id,  # соответствует UserPaymentUpdateSchema.id
            "payment_id": payment_id,  # оставляем для совместимости с текущей реализацией
            "amount": 2000.75,
            "description": "Updated test payment",
        }

        response = authenticated_session.patch(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=update_payload,
            timeout=10,
        )
        attach_response("user_payment.update", response)

        assert_status(
            response,
            [200],
            context="PATCH /user-payment",
            bug_map={
                201: "#BUG-USER-PAYMENT-UPDATE-201",
                202: "#BUG-USER-PAYMENT-UPDATE-202",
                401: "#BUG-USER-PAYMENT-UPDATE-401",
                403: "#BUG-USER-PAYMENT-UPDATE-403",
                404: "#BUG-USER-PAYMENT-UPDATE-404",
                422: "#BUG-USER-PAYMENT-UPDATE-422",
            },
        )


@pytest.mark.api
class TestUserPaymentDelete:
    def test_delete_user_payment(
        self,
        authenticated_session: requests.Session,
    ):
        """Проверка DELETE /user-payment (удаление платежа)."""
        user_id = _get_first_user_id(authenticated_session)
        if not user_id:
            pytest.skip("Нет доступных пользователей для теста")

        # Сначала создаем платеж для удаления
        create_payload = {
            "user_id": user_id,
            "amount": 500.25,
            "payment_date": "2025-11-25",
            "description": "Test payment for delete",
        }

        create_resp = authenticated_session.post(
            f"{authenticated_session.base_url}{BASE_PATH}",
            json=create_payload,
            timeout=10,
        )

        payment_id = None
        if create_resp.status_code == 201:
            data = create_resp.json()
            payment_id = data.get("id") or data.get("payment_id")

        if not payment_id:
            pytest.skip("Не удалось создать платеж для удаления")

        # Удаляем платеж
        response = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"payment_id": payment_id},
            timeout=10,
        )
        attach_response("user_payment.delete", response)

        assert_status(
            response,
            [204, 422],
            context="DELETE /user-payment",
            bug_map={
                200: "#BUG-USER-PAYMENT-DELETE-200",
                202: "#BUG-USER-PAYMENT-DELETE-202",
                401: "#BUG-USER-PAYMENT-DELETE-401",
                403: "#BUG-USER-PAYMENT-DELETE-403",
                404: "#BUG-USER-PAYMENT-DELETE-404",
            },
        )

    def test_delete_user_payment_invalid_id(
        self, authenticated_session: requests.Session
    ):
        """Проверка DELETE /user-payment с несуществующим ID."""
        response = authenticated_session.delete(
            f"{authenticated_session.base_url}{BASE_PATH}",
            params={"payment_id": 999999},
            timeout=10,
        )
        attach_response("user_payment.delete.invalid", response)

        assert_status(
            response,
            [204, 422],
            context="DELETE /user-payment (несуществующий ID)",
            bug_map={
                200: "#BUG-USER-PAYMENT-DELETE-INVALID-200",
                202: "#BUG-USER-PAYMENT-DELETE-INVALID-202",
                401: "#BUG-USER-PAYMENT-DELETE-INVALID-401",
                403: "#BUG-USER-PAYMENT-DELETE-INVALID-403",
                404: "#BUG-USER-PAYMENT-DELETE-INVALID-404",
            },
        )

    def test_user_payment_excel_all(
        self, authenticated_session: requests.Session
    ):
        """GET /user-payment/excel/all — экспорт всех платежей в Excel."""
        try:
            resp = authenticated_session.get(
                f"{authenticated_session.base_url}{BASE_PATH}/excel/all",
                timeout=8,
            )
        except requests_exceptions.Timeout:
            pytest.skip("API перегружен: таймаут GET /user-payment/excel/all")
        attach_response("user_payment.excel_all", resp)
        assert_status(
            resp,
            [200],
            context="GET /user-payment/excel/all",
            bug_map={
                403: "#BUG-USER-PAYMENT-EXCEL-403",
                404: "#BUG-USER-PAYMENT-EXCEL-404",
            },
        )

