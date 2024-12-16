#include "SLAE.h"
#include <iostream>

int main()
{
    try
    {
        slae slae;
        slae.input("input.txt");

        bool has_solution = slae.solve();

        if (has_solution)
            has_solution = slae.validate_solution();

        slae.output("output.txt", has_solution);
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << '\n';
        return 1;
    }

    return 0;
}
