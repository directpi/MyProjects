[К оглавлению](../../README.md)

# matrix+

Реализация библиотеки matrix_oop.h


## Содержание

1. [Описание](#introduction)
2. [Информация](#information)
3. [Реализация](#реализация-функции-библиотеки-matrix_ooph)

## Описание

В данном учебном проекте реализована библиотека для работы с матрицами, с использованием ООП. Объектно-ориентированный подход позволяет реализовать библиотеку для работы с матрицами в виде отдельного класса, над объектами которого определены операции, представленные как методами, так и стандартными операторами +, -, * и т.д.

## Информация

### Операции над матрицами

Ниже представлено краткое описание операций над матрицами, которые реализованы в библиотеке. Некоторые операции имеют исключительные ситуации, требующие специальной обработки с использованием механизма исключений. 

| Операция    | Описание   | Исключительные ситуации |
| ----------- | ----------- | ----------- |
| `bool EqMatrix(const Matrix& other)` | Проверяет матрицы на равенство между собой. |  |
| `void SumMatrix(const Matrix& other)` | Прибавляет вторую матрицу к текущей | различная размерность матриц. |
| `void SubMatrix(const Matrix& other)` | Вычитает из текущей матрицы другую | различная размерность матриц. |
| `void MulNumber(const double num)` | Умножает текущую матрицу на число. |  |
| `void MulMatrix(const Matrix& other)` | Умножает текущую матрицу на вторую. | число столбцов первой матрицы не равно числу строк второй матрицы. |
| `Matrix Transpose()` | Создает новую транспонированную матрицу из текущей и возвращает ее. |  |
| `Matrix CalcComplements()` | Вычисляет матрицу алгебраических дополнений текущей матрицы и возвращает ее. | Матрица не является квадратной. |
| `double Determinant()` | Вычисляет и возвращает определитель текущей матрицы. | Матрица не является квадратной. |
| `Matrix InverseMatrix()` | Вычисляет и возвращает обратную матрицу. | Определитель матрицы равен 0. |

Помимо реализации данных операций, реализованы конструкторы и деструкторы:

| Метод    | Описание   |
| ----------- | ----------- |
| `Matrix()` | Базовый конструктор, инициализирующий матрицу некоторой заранее заданной размерностью. |  
| `Matrix(int rows, int cols)` | Параметризированный конструктор с количеством строк и столбцов. | 
| `Matrix(const Matrix& other)` | Конструктор копирования. |
| `Matrix(Matrix&& other)` | Конструктор переноса. |
| `~Matrix()` | Деструктор. |

А также перегружены следующие операторы, частично соответствующие операциям выше:

| Оператор    | Описание   | Исключительные ситуации |
| ----------- | ----------- | ----------- |
| `+`      | Сложение двух матриц.  | Различная размерность матриц. |
| `-`   | Вычитание одной матрицы из другой. | Различная размерность матриц. |
| `*`  | Умножение матриц и умножение матрицы на число. | Число столбцов первой матрицы не равно числу строк второй матрицы. |
| `==`  | Проверка на равенство матриц (`EqMatrix`). | |
| `=`  | Присвоение матрице значений другой матрицы. | |
| `+=`  | Присвоение сложения (`SumMatrix`).   | Различная размерность матриц. |
| `-=`  | Присвоение разности (`SubMatrix`). | Различная размерность матриц. |
| `*=`  | Присвоение умножения (`MulMatrix`/`MulNumber`). | Число столбцов первой матрицы не равно числу строк второй матрицы. |
| `(int i, int j)`  | Индексация по элементам матрицы (строка, колонка). | Индекс за пределами матрицы. |

## Реализация функции библиотеки matrix_oop.h

- Программа разработана на языке C++ стандарта C++17 с использованием компилятора gcc;
- Код программы находится в папке src;
- Стиль Google Style;
- Реализована матрица в виде класса `Matrix`;
- Хранятся только приватные поля `matrix_`, `rows_` и `cols_`;
- Реализован доступ к приватным полям `rows_` и `cols_` через accessor и mutator. При увеличении размера матрица дополняется нулевыми элементами, при уменьшении - лишнее просто отбрасывается;
- Решение оформлено как статическая библиотека (с заголовочным файлом matrix_oop.h);
- Реализованы операции, описанные [выше](#операции-над-матрицами)
- Перегружены операторы в соответствии с таблицой в разделе [выше](#операции-над-матрицами).
- Подготовлено полное покрытие unit-тестами функций библиотеки c помощью библиотеки GTest;
- Предусмотрен Makefile для сборки библиотеки и тестов (с целями all, clean, test, matrix_oop.a).