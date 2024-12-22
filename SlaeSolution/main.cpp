#include "SLAE.h"
#include <iostream>

int main()
{
    try
    {
        slae slae;

        slae.input();
        const bool solvable = slae.solve();

        if (!solvable)
        {
            slae.output(solvable);
            return 0;
        }

        if (!slae.validate_solution())
        {
            slae.output(solvable);
            return 0;
        }

        slae.output(solvable);
        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
