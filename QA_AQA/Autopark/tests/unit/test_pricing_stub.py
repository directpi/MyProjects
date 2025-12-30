"""
@file: test_pricing_stub.py
@description: Заглушки unit-тестов расчётной логики (тарифы/стоимость)
@dependencies: [pytest]
@created: 2025-12-24
@author: Directpi
@test_type: unit
"""

import pytest

pytestmark = [
    pytest.mark.unit,
    pytest.mark.skip(
        reason="Исходный код расчётной логики недоступен; заглушка unit-теста"
    ),
]


@pytest.mark.parametrize(
    "input_, expected",
    [
        ({"distance": 0, "rate": 10}, 0),
        ({"distance": 10, "rate": 10}, 100),
    ],
)
def test_calc_price_basic(input_, expected):
    """Заглушка: базовый расчёт стоимости по расстоянию и тарифу."""
    pass


def test_calc_price_raises_on_negative_distance():
    """Заглушка: отрицательная дистанция должна приводить к ошибке."""
    pass


def test_calc_price_applies_min_fee():
    """Заглушка: проверка минимальной платы, если предусмотрена бизнес-логикой."""
    pass
