#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "Polynomial.cpp"

/*-----------------------------------------------------------------------------
The main function.
-----------------------------------------------------------------------------*/
int main(int const argc, char const **argv)
{
    if(argc < 2)
    {
        std::cout << "usage:\n";
        std::cout << "\t" << argv[0] << " <input file>\n";
        return 1;
    }

    bool show_rational = 0;
    if(argc >= 3 && argv[2][0] == '1')
    {
        show_rational = 1;
    }

    // Note to self
    // there is no need to close an `std::ifstream' file
    std::ifstream infile(argv[1]);
    if(!infile)
    {
        std::cout << "File \'" << argv[1] << "\' could not be read.\n";
        return 2;
    }

    double xcoord, ycoord;
    std::vector<double> xcoords, ycoords;
    while((infile >> xcoord) && (infile >> ycoord))
    {
        xcoords.push_back(xcoord);
        ycoords.push_back(ycoord);
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Polynomial poly = interpolate(xcoords, ycoords);
    poly.print(show_rational);
    std::cout << poly.get_name() << "(" << xcoord << ") = " << poly.evaluate(xcoord) << "\n";
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "actual time taken: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " µs" << "\n";

    return 0;
}

