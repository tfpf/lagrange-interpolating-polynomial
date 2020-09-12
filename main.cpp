#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
#include <vector>

#include "pstl.cpp"

/*-----------------------------------------------------------------------------
Given two vectors, generate the interpolating polynomial on which all the
points represented by those vectors lie. A Lagrange Interpolating Polynomial
is built using the given points. Although it may increase the processing
required, it is easier to understand and debug the code, because the code
expressions resemble the mathematical expressions.

Args:
    x: vector (x-coordinates of the points to be interpolated between)
    y: vector (y-coordinates of the points to be interpolated between)

Returns:
    Polynomial object which describes the interpolating polynomial
-----------------------------------------------------------------------------*/
Polynomial interpolate(std::vector<double> const& x, std::vector<double> const& y)
{
    // sanity 1
    if(x.size() <= 1 || y.size() <= 1)
    {
        throw std::invalid_argument("At least two points are required for interpolation.");
    }

    // sanity 2
    std::set<double> s(x.begin(), x.end());
    if(s.size() != x.size())
    {
        throw std::invalid_argument("Interpolating points must have unique x-coordinates.");
    }

    // if the vectors are of different lengths, extra coordinates are ignored
    int num_points = std::min(x.size(), y.size());

    Polynomial result;
    for(int j = 0; j < num_points; ++j)
    {
        Polynomial local({y[j]});
        for(int k = 0; k < num_points; ++k)
        {
            if(k == j)
            {
                continue;
            }

            local = local * Polynomial({-x[k], 1}) / (-x[k] + x[j]);
        }
        result = result + local;
    }
    result.set_name("ip(x)");

    return result;
}

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
    std::cout << c.get_name() << " = " << c.evaluate(coord) << " when x = " << coord << "\n";

    return 0;
}

