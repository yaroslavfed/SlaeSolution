#include "SLAE.h"
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <iostream>

slae::slae() : num_equations(0), num_variables(0), matrix_size(0), num_free_variables(0)
{
}

void slae::input()
{
    std::cin >> num_equations >> num_variables;
    matrix_size = num_equations + num_variables;

    extended_matrix.resize(matrix_size, std::vector<int>(num_variables + 1));
    original_matrix.resize(matrix_size, std::vector<int>(num_variables + 1));

    for (int i = 0; i < num_equations; i++)
    {
        for (int j = 0; j <= num_variables; j++)
        {
            std::cin >> extended_matrix[i][j];
            original_matrix[i][j] = extended_matrix[i][j];
        }
        // Инвертируем правую часть
        extended_matrix[i][num_variables] *= -1;
    }

    // Добавляем единичные строки для свободных переменных
    for (int i = num_equations, j = 0; i < matrix_size; i++, j++)
        extended_matrix[i][j] = 1;
}

bool slae::solve()
{
    for (int i = 0; i < num_equations; i++)
        while (true)
        {
            int min_index = -1;
            for (int j = i; j < num_equations; j++)
                if (extended_matrix[i][j] != 0 && (min_index == -1 || abs(extended_matrix[i][j]) < abs(
                    extended_matrix[i][min_index])))
                    min_index = j;

            if (min_index == -1)
            {
                if (extended_matrix[i][num_equations] != 0)
                    return false;

                extended_matrix.erase(extended_matrix.begin() + i);
                --i;
                --num_equations;
                --matrix_size;
                break;
            }

            for (int j = i; j < num_equations; j++)
                if (j != min_index && extended_matrix[i][j] != 0)
                {
                    int q = extended_matrix[i][j] / extended_matrix[i][min_index];
                    for (int k = i; k < matrix_size; k++)
                        extended_matrix[k][j] -= extended_matrix[k][min_index] * q;
                }

            if (extended_matrix[i][i] == 0)
                for (int k = i; k < matrix_size; k++)
                    std::swap(extended_matrix[k][i], extended_matrix[k][min_index]);

            bool non_diagonal_exists = false;
            for (int j = i + 1; j < num_equations; j++)
                if (extended_matrix[i][j] != 0)
                {
                    non_diagonal_exists = true;
                    break;
                }

            if (!non_diagonal_exists)
            {
                if (extended_matrix[i][i] == 0 || extended_matrix[i][num_equations] % extended_matrix[i][i] != 0)
                    return false;

                const int q = extended_matrix[i][num_equations] / extended_matrix[i][i];
                for (int k = i; k < matrix_size; k++)
                    extended_matrix[k][num_equations] -= extended_matrix[k][i] * q;

                if (extended_matrix[i][num_equations] == 0)
                    break;
            }
        }
    return true;
}

bool slae::validate_solution()
{
    num_free_variables = num_variables - num_equations;

    std::vector<int> free_variables(num_free_variables);
    for (int i = 0; i < num_free_variables; i++)
        free_variables[i] = rand(); // NOLINT(concurrency-mt-unsafe)

    std::vector<int> x(num_variables);
    for (int i = 0; i < num_variables; i++)
        x[i] = i >= num_equations
                   ? extended_matrix[i][num_variables]
                   : 0;

    for (int s = num_equations, i = 0; s < matrix_size; s++, i++)
        for (int j = 0; j < num_free_variables; j++)
            x[s - num_equations] += extended_matrix[s][num_variables - (j + 1)] * free_variables[j];

    std::vector<int> test(num_equations);
    for (int i = 0; i < num_equations; i++)
        for (int j = 0; j < num_variables; j++)
            test[i] += original_matrix[i][j] * x[j];

    int result = 0;
    for (int i = 0; i < num_equations; i++)
        result += abs(test[i] - original_matrix[i][num_variables]);

    return result == 0;
}

void slae::output(const bool has_solution) const
{
    if (!has_solution)
        std::cout << "NO SOLUTIONS" << '\n';
    else
    {
        std::cout << num_free_variables << '\n';
        for (int i = num_equations; i < matrix_size; i++)
        {
            for (int j = num_variables - num_free_variables; j <= num_variables; j++)
                std::cout << extended_matrix[i][j] << " ";
            std::cout << '\n';
        }
    }
}
