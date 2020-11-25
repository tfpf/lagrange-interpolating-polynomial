#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
#include <vector>

#include "Polynomial.cpp"

/*-----------------------------------------------------------------------------
The main function.
-----------------------------------------------------------------------------*/
int main(int const argc, char const **argv)
{
    // check arguments
    if(argc < 2)
    {
        std::cout << "usage:\n";
        std::cout << "\t" << argv[0] << " <input file>\n";
        return 1;
    }

    // read input
    std::ifstream infile(argv[1]);
    if(!infile)
    {
        std::cout << "File " << argv[1] << " could not be read.\n";
        return 2;
    }

    // read the number of points in the file
    int num_points;
    infile >> num_points;
    std::vector<double> x(num_points), y(num_points);

    // read the coordinates of the points into vectors
    for(int i = 0; i < num_points; ++i)
    {
        infile >> x[i];
        infile >> y[i];
    }

    // read final coordinate at which the polynomial has to be evaluated
    double coord;
    infile >> coord;

    // magic
    Polynomial c = interpolate(x, y);
    c.print();
    std::cout << c.get_name() << "(" << coord << ") = " << c.evaluate(coord) << "\n";

    return 0;
}

