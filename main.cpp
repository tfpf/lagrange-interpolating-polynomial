#include <algorithm>
#include <cmath>
#include <fstream>
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
    if(argc >= 3)
    {
        try
        {
            show_rational = std::stoi(argv[2]);
        }
        catch(std::invalid_argument const& e)
        {
            show_rational = 0;
        }
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
    for(int i = 0; (infile >> xcoord) && (infile >> ycoord); ++i)
    {
        xcoords.push_back(xcoord);
        ycoords.push_back(ycoord);
    }

    Polynomial poly = interpolate(xcoords, ycoords);
    poly.print(show_rational);
    std::cout << poly.get_name() << "(" << xcoord << ") = " << poly.evaluate(xcoord) << "\n";

    return 0;
}

