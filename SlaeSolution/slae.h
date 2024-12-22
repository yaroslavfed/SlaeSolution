#pragma once

#include <vector>

class slae
{
    // Количество уравнений
    int num_equations;

    // Количество переменных
    int num_variables;

    // Расширенная матрица
    std::vector<std::vector<int>> extended_matrix;

    // Оригинальная матрица для проверки решения
    std::vector<std::vector<int>> original_matrix;

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
