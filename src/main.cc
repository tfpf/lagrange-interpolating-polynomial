#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include "Polynomial.hh"

/******************************************************************************
 * Main function.
 *****************************************************************************/
int main(int const argc, char const **argv)
{
    if(argc < 2)
    {
        std::cout << "usage:\n";
        std::cout << "\t" << argv[0] << " <input file>\n";
        return 1;
    }

    bool show_rational = (argc >= 3 && argv[2][0] == '1') ? true : false;

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

    auto begin = std::chrono::steady_clock::now();
    Polynomial poly = interpolate(xcoords, ycoords);
    double val = poly.evaluate(xcoord);
    auto end = std::chrono::steady_clock::now();
    poly.print(show_rational);
    std::cout << poly.get_name() << "(" << xcoord << ") = " << val << "\n";
    std::cout << "actual time taken: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " µs" << "\n";

    return 0;
}
