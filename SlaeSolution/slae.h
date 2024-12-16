#pragma once

#include <vector>
#include <string>

using matrix = std::vector<std::vector<int>>;
using vector = std::vector<int>;

class slae
{
    size_t num_equations; // Количество уравнений
    size_t num_variables; // Количество переменных
    matrix extended_matrix; // Расширенная матрица
    matrix original_matrix; // Оригинальная матрица для проверки решения
    size_t matrix_size; // Общий размер расширенной матрицы
    size_t num_free_variables; // Количество свободных переменных

public:
    slae(); // Конструктор по умолчанию

    // Загрузка системы линейных уравнений из файла
    void input(const std::string& file_name);

    // Решение системы линейных уравнений
    bool solve();

    // Проверка решения
    bool validate_solution();

    // Вывод результата в файл
    void output(const std::string& file_name, bool has_solution) const;

private:
    // Вспомогательные функции
    size_t find_pivot_column(size_t row) const; // Поиск ведущего столбца
    void eliminate_column(size_t row, size_t pivot_column); // Устранение элементов в столбце
    void normalize_row(size_t row, size_t pivot_column); // Нормализация строки

    // Утилиты для проверки работы с файлами
    static void check_file_stream(const std::ifstream& stream, const std::string& file_name); // Проверка входного файла
    static void check_file_stream(const std::ofstream& stream, const std::string& file_name); // Проверка выходного файла

    // Генерация случайных свободных переменных
    vector generate_free_variables() const;

    // Вычисление вектора решений
    vector compute_solution(const vector& free_variables) const;

    // Проверка решения относительно оригинальной матрицы
    bool check_solution(const vector& solution) const;
};
