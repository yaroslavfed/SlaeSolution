#include "SLAE.h"
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <cstdlib>
#include <iostream>

slae::slae() : num_equations(0), num_variables(0), matrix_size(0), num_free_variables(0)
{
}

void slae::input(const std::string& file_name)
{
    // std::ifstream input_file(file_name);
    // check_file_stream(input_file, file_name);
    //
    // input_file >> num_equations >> num_variables;

    std::cin >> num_equations >> num_variables;
    
    matrix_size = num_equations + num_variables;

    extended_matrix.resize(matrix_size, vector(num_variables + 1, 0));
    original_matrix.resize(num_equations, vector(num_variables + 1, 0));

    for (size_t i = 0; i < num_equations; ++i)
    {
        for (size_t j = 0; j <= num_variables; ++j)
        {
            // input_file >> extended_matrix[i][j];
            std::cin >> extended_matrix[i][j];
            original_matrix[i][j] = extended_matrix[i][j];
        }
        // Инвертируем правую часть
        extended_matrix[i][num_variables] *= -1;
    }

    // Добавляем единичные строки для свободных переменных
    for (size_t i = num_equations, j = 0; i < matrix_size; ++i, ++j)
        extended_matrix[i][j] = 1;

    // input_file.close();
}

bool slae::solve()
{
    for (size_t i = 0; i < num_equations; ++i)
    {
        const size_t pivot_column = find_pivot_column(i);

        if (pivot_column == static_cast<size_t>(-1))
        {
            if (extended_matrix[i][num_variables] != 0)
                return false;
            continue;
        }

        eliminate_column(i, pivot_column);
        normalize_row(i, pivot_column);
    }
    return true;
}

bool slae::validate_solution()
{
    num_free_variables = num_variables - num_equations;

    const vector free_variables = generate_free_variables();
    const vector solution = compute_solution(free_variables);

    return check_solution(solution);
}

void slae::output(const std::string& file_name, const bool has_solution) const
{
    // std::ofstream output_file(file_name);
    // check_file_stream(output_file, file_name);

    if (!has_solution)
        // output_file << "NO SOLUTIONS";
        std::cout << "NO SOLUTIONS";
    else
    {
        // output_file << num_free_variables << '\n';
        std::cout << num_free_variables << '\n';
        for (size_t i = num_equations; i < matrix_size; ++i)
        {
            for (size_t j = num_variables - num_free_variables; j <= num_variables; ++j)
                // output_file << extended_matrix[i][j] << " ";
                std::cout << extended_matrix[i][j] << " ";
            // output_file << '\n';
            std::cout << '\n';
        }
    }
    // output_file.close();
}

size_t slae::find_pivot_column(const size_t row) const
{
    size_t pivot_column = static_cast<size_t>(-1);
    for (size_t j = row; j < num_variables; ++j)
        if (extended_matrix[row][j] != 0)
            if (pivot_column == static_cast<size_t>(-1)
                || std::abs(extended_matrix[row][j]) < std::abs(extended_matrix[row][pivot_column])
            )
                pivot_column = j;
    return pivot_column;
}

void slae::eliminate_column(const size_t row, const size_t pivot_column)
{
    for (size_t i = row + 1; i < matrix_size; ++i)
        if (extended_matrix[i][pivot_column] != 0)
        {
            const int factor = extended_matrix[i][pivot_column] / extended_matrix[row][pivot_column];
            for (size_t j = pivot_column; j <= num_variables; ++j)
                extended_matrix[i][j] -= factor * extended_matrix[row][j];
        }
}

void slae::normalize_row(const size_t row, const size_t pivot_column)
{
    const int pivot_value = extended_matrix[row][pivot_column];
    if (pivot_value != 0)
        for (size_t j = pivot_column; j <= num_variables; ++j)
            extended_matrix[row][j] /= pivot_value;
}

void slae::check_file_stream(const std::ifstream& stream, const std::string& file_name)
{
    if (!stream)
        throw std::runtime_error("Не удалось открыть файл: " + file_name);
}

void slae::check_file_stream(const std::ofstream& stream, const std::string& file_name)
{
    if (!stream)
        throw std::runtime_error("Не удалось открыть файл: " + file_name);
}

vector slae::generate_free_variables() const
{
    vector free_variables(num_free_variables);
    for (size_t i = 0; i < num_free_variables; ++i)
        // NOLINT(concurrency-mt-unsafe)
        free_variables[i] = std::rand() % 10;
    return free_variables;
}

vector slae::compute_solution(const vector& free_variables) const
{
    vector solution(num_variables, 0);
    for (size_t i = num_equations; i < matrix_size; ++i)
    {
        solution[i - num_equations] = extended_matrix[i][num_variables];
        for (size_t j = 0; j < num_free_variables; ++j)
            solution[i - num_equations]
                += extended_matrix[i][num_variables - num_free_variables + j] * free_variables[j];
    }
    return solution;
}

bool slae::check_solution(const vector& solution) const
{
    for (size_t i = 0; i < num_equations; ++i)
    {
        int sum = 0;
        for (size_t j = 0; j < num_variables; ++j)
            sum += original_matrix[i][j] * solution[j];

        if (sum != original_matrix[i][num_variables])
            return false;
    }
    return true;
}
