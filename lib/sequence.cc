#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "Polynomial.hh"

/******************************************************************************
 * Main function.
 *****************************************************************************/
int main(int const argc, char const* argv[])
{
    if(argc < 2)
    {
        std::cerr << "Usage:\n";
        std::cerr << "  " << argv[0] << " <input file>\n";
        return EXIT_FAILURE;
    }
    std::ifstream input(argv[1]);
    if(!input.good())
    {
        std::cerr << "File " << argv[1] << " could not be read.\n";
        return EXIT_FAILURE;
    }
    bool rational = (argc >= 3);

    std::vector<double> xcoords, ycoords;
    double xcoord, ycoord;
    while((input >> xcoord) && (input >> ycoord))
    {
        xcoords.push_back(xcoord);
        ycoords.push_back(ycoord);
    }
    auto begin = std::chrono::steady_clock::now();
    Polynomial p(xcoords, ycoords);
    auto end = std::chrono::steady_clock::now();
    auto delay = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    // To display more digits after the decimal point.
    std::cout.precision(12);

    p.rational = rational;
    std::cout << "[3mp[0m ≡ " << p << "\n";
    std::cout << "[3mp[0m(" << xcoord << ") = " << p(xcoord) << "\n";
    std::cout << "Done in " << delay << " µs.\n";
    return EXIT_SUCCESS;
}
