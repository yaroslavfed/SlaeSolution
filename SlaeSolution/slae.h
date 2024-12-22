#pragma once

#include <vector>

using matrix = std::vector<std::vector<int>>;
using vector = std::vector<int>;

class slae
{
    // Количество уравнений
    int num_equations;

    // Количество переменных
    int num_variables;

    // Расширенная матрица
    matrix extended_matrix;

    // Оригинальная матрица для проверки решения
    matrix original_matrix;

    // Общий размер расширенной матрицы
    int matrix_size;

    // Количество свободных переменных
    int num_free_variables;

public:
    // Конструктор по умолчанию
    slae();

    // Загрузка системы линейных уравнений из файла
    void input();

    // Решение системы линейных уравнений
    bool solve();

    // Проверка решения
    bool validate_solution();

    // Вывод результата в файл
    void output(bool has_solution) const;
};
