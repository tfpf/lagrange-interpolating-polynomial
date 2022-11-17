#include <algorithm>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Polynomial.hh"

#define THROW(exception, message)  \
{  \
    throw exception(std::string(__FILE__) + ':' + std::to_string(__LINE__)  \
                    + ", in function " + __func__ + ". " + message);  \
}

/******************************************************************************
 * Constructor. Create an empty vector, which shall be equivalent to a
 * single-element vector containing zero. Both shall represent the zero
 * polynomial.
 *
 * @return The zero polynomial.
 *****************************************************************************/
Polynomial::Polynomial()
{
}

/******************************************************************************
 * Constructor. Implicit conversion of a brace-enclosed initialiser list to a
 *     polynomial.
 *
 * @param list Coefficients to initialise this polynomial with. The order of
 *     the coefficients shall be in increasing order of the exponent of the
 *     variable (i.e. the constant term must come first).
 *
 * @return A polynomial with the given coefficients.
 *****************************************************************************/
Polynomial::Polynomial(std::initializer_list<double> const& list)
: std::vector<double>(list)
{
    this->sanitise();
}

/******************************************************************************
 * Constructor. Implicit conversion of a vector to a polynomial.
 *
 * @param vector Coefficients to initialise this polynomial with. The order of
 *     the coefficients shall be in increasing order of the exponent of the
 *     variable (i.e. the constant term must come first).
 *
 * @return A polynomial with the given coefficients.
 *****************************************************************************/
Polynomial::Polynomial(std::vector<double> const& vector)
: std::vector<double>(vector)
{
    this->sanitise();
}

/******************************************************************************
 * Constructor. Given the x- and y-coordinates of a set of points, find the
 * interpolating polynomial which passes through all of them. If the two
 * arguments are of different sizes, the extra coordinates present at the end
 * of the larger argument are ignored.
 *
 * @param xcoords
 * @param ycoords
 *
 * @return A polynomial passing through the given points.
 *****************************************************************************/
Polynomial::Polynomial(std::vector<double> const& xcoords, std::vector<double> const& ycoords)
{
    std::size_t num_of_points = std::min(xcoords.size(), ycoords.size());
    if(num_of_points <= 1)
    {
        THROW(std::invalid_argument, "At least two points are required for interpolation.")
    }
    std::unordered_map<double, std::size_t> unique_xcoords;
    for(auto const& xcoord: xcoords)
    {
        if(unique_xcoords[xcoord] > 0)
        {
            auto str_xcoord = std::to_string(xcoord);
            std::string message = "Expected distinct x-coordinates, but " + str_xcoord + " occurs multiple times.";
            THROW(std::invalid_argument, message)
        }
        ++unique_xcoords[xcoord];
    }

    // Calculate using the formula for the Lagrange interpolating polynomial.
    for(std::size_t i = 0; i < num_of_points; ++i)
    {
        Polynomial local = {ycoords[i]};
        for(std::size_t j = 0; j < num_of_points; ++j)
        {
            if(i != j)
            {
                local *= Polynomial({-xcoords[j], 1}) / (-xcoords[j] + xcoords[i]);
            }
        }
        *this += local;
    }
    this->sanitise();
}

/******************************************************************************
 * Replace extremely small coefficients with zeros. Remove trailing zero
 * coefficients.
 *****************************************************************************/
void Polynomial::sanitise(void)
{
    for(auto& coefficient: *this)
    {
        if(std::abs(coefficient) <= 1e-10)
        {
            coefficient = 0;
        }
    }
    while(!this->empty() && this->back() == 0)
    {
        this->pop_back();
    }
}

/******************************************************************************
 * Print a polynomial.
 *
 * @param ostream Output stream.
 * @param p Polynomial.
 *
 * @return The output stream.
 *****************************************************************************/
std::ostream& operator<<(std::ostream& ostream, Polynomial const& p)
{
    char const* delimiter = "";
    char const* actual_delimiter = ", ";
    ostream << "[";
    for(auto const& coefficient: p)
    {
        std::cout << delimiter;
        if(p.rational)
        {
            std::cout << rationalise(coefficient);
        }
        else
        {
            std::cout << coefficient;
        }
        delimiter = actual_delimiter;
    }
    std::cout << "]";
    return ostream;
}

/******************************************************************************
 * Add two polynomials in-place.
 *
 * @param p
 * @param q
 *****************************************************************************/
void operator+=(Polynomial& p, Polynomial const& q)
{
    std::size_t p_size = p.size();
    std::size_t q_size = q.size();
    std::size_t i = 0;
    while(i < p_size && i < q_size)
    {
        p[i] += q[i];
        ++i;
    }
    while(i < q_size)
    {
        p.push_back(q[i++]);
    }
    p.sanitise();
}

/******************************************************************************
 * Add two polynomials.
 *
 * @param p
 * @param q
 *
 * @return Sum of the arguments.
 *****************************************************************************/
Polynomial operator+(Polynomial const& p, Polynomial const& q)
{
    Polynomial result = p;
    result += q;
    return result;
}

/******************************************************************************
 * Subtract two polynomials in-place.
 *
 * @param p
 * @param q
 *****************************************************************************/
void operator-=(Polynomial& p, Polynomial const& q)
{
    std::size_t p_size = p.size();
    std::size_t q_size = q.size();
    std::size_t i = 0;
    while(i < p_size && i < q_size)
    {
        p[i] -= q[i];
        ++i;
    }
    while(i < q_size)
    {
        p.push_back(-q[i++]);
    }
    p.sanitise();
}

/******************************************************************************
 * Subtract two polynomials.
 *
 * @param p
 * @param q
 *
 * @return Difference of the arguments.
 *****************************************************************************/
Polynomial operator-(Polynomial const& p, Polynomial const& q)
{
    Polynomial result = p;
    result -= q;
    return result;
}

/******************************************************************************
 * Multiply two polynomials in-place.
 *
 * @param p
 * @param q
 *****************************************************************************/
void operator*=(Polynomial& p, Polynomial const& q)
{
    p = p * q;
}

/******************************************************************************
 * Multiply two polynomials.
 *
 * @param p
 * @param q
 *
 * @return Product of the arguments.
 *****************************************************************************/
Polynomial operator*(Polynomial const& p, Polynomial const& q)
{
    if(p.empty() || q.empty())
    {
        return {};
    }

    // Convolution of two sequences.
    Polynomial result;
    std::size_t p_size = p.size();
    std::size_t q_size = q.size();
    for(std::size_t n = 0; n < p_size + q_size - 1; ++n)
    {
        result.push_back(0);
        for(std::size_t k = 0; k <= n; ++k)
        {
            if(k < p_size && n - k < q_size)
            {
                result.back() += p[k] * q[n - k];
            }
        }
    }
    result.sanitise();
    return result;
}

/******************************************************************************
 * Divide a polynomial by a scalar in-place.
 *
 * @param p
 * @param d
 *****************************************************************************/
void operator/=(Polynomial& p, double d)
{
    for(auto& coefficient: p)
    {
        coefficient /= d;
    }
    p.sanitise();
}

/******************************************************************************
 * Divide a polynomial by a scalar.
 *
 * @param p
 * @param d
 *
 * @return Quotient of division.
 *****************************************************************************/
Polynomial operator/(Polynomial const& p, double d)
{
    Polynomial result = p;
    result /= d;
    return result;
}

/******************************************************************************
 * Evaluate the polynomial.
 *
 * @param x x-coordinate of the point to evaluate the polynomial at.
 *
 * @return y-coordinate of the polynomial at the given x-coordinate.
 *****************************************************************************/
double Polynomial::operator()(double x)
{
    double y = 0;
    for(auto it = this->crbegin(); it != this->crend(); it = std::next(it))
    {
        y = y * x + *it;
    }
    return y;
}

/******************************************************************************
 * Approximate a real number as a rational number with a small denominator.
 * Much of this code is copied from that of the `limit_denominator` method of
 * the Python class `fractions.Fraction`.
 *
 * @param number Number to approximate.
 * @param max_denominator Maximum denominator the approximation may have.
 *
 * @return Rational approximation of `number`.
 *****************************************************************************/
std::string rationalise(double number, int long long max_denominator)
{
    auto number_to_integer = static_cast<int long long>(number);
    if(number_to_integer == number)
    {
        return std::to_string(number_to_integer);
    }

    // Discard the sign so we can focus on positive values.
    std::string sign;
    if(number < 0)
    {
        sign.push_back('-');
    }
    number = std::abs(number);

    // Obtain an initial approximation with a large denominator.
    int long long d = 1e12;
    int long long n = number * d;
    int long long g = std::gcd(n, d);
    n /= g;
    d /= g;
    if(d <= max_denominator)
    {
        std::string result = sign + std::to_string(n);
        if(d != 1)
        {
            return result + '/' + std::to_string(d);
        }
        return result;
    }

    // Converge to a better approximation.
    int long long p0 = 0;
    int long long q0 = 1;
    int long long p1 = 1;
    int long long q1 = 0;
    while(true)
    {
        int long long a = n / d;
        int long long q2 = q0 + a * q1;
        if(q2 > max_denominator)
        {
            break;
        }
        int long long p1_ = p1;
        p1 = p0 + a * p1;
        q0 = q1;
        q1 = q2;
        p0 = p1_;
        int long long d_ = d;
        d = n - a * d;
        n = d_;
    }

    // Obtain two bounds. Choose the one with the smaller denominator if both
    // are equally accurate.
    int long long k = (max_denominator - q0) / q1;
    double bound1 = static_cast<double>(p0 + k * p1) / (q0 + k * q1);
    double bound2 = static_cast<double>(p1) / q1;
    if(std::abs(bound2 - number) <= std::abs(bound1 - number))
    {
        n = p1;
        d = q1;
    }
    else
    {
        n = p0 + k * p1;
        d = q0 + k * q1;
    }
    std::string result = sign + std::to_string(n);
    if(d != 1)
    {
        return result + '/' + std::to_string(d);
    }
    return result;
}
