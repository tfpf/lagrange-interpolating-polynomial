#ifndef SEQUENCE_INTERPOLATION_POLYNOMIAL_CPP
#define SEQUENCE_INTERPOLATION_POLYNOMIAL_CPP

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

typedef int long ilong;

class Polynomial;

// Function prototypes.
template<typename Type> Type my_gcd(Type m, Type n);
std::string rationalise(double real, ilong max_denominator=1000000);
Polynomial interpolate(std::vector<double> const& x, std::vector<double> const& y);

/******************************************************************************
 * A class to represent a polynomial. For instance, the polynomial
 * \code{.unparsed}
 *     12.8x^5 - 1.62x^2 + 33x - 7.31
 * \endcode
 * will be stored as the vector
 * \code{.unparsed}
 *     [-7.31, 33, -1.62, 0, 0, 12.8]
 * \endcode
 * so that the coefficients are arranged in increasing order of the power of
 * the variable they are associated with. Which makes things simple, because
 * the power will be simply the index of the coefficient in the vector.
 *****************************************************************************/
class Polynomial
{
    private: std::vector<double> coeffs;
    private: std::string name;

    public: Polynomial(void);
    public: Polynomial(std::vector<double> const& coeffs);
    public: Polynomial(std::vector<double> const& coeffs, std::string const& name);
    public: void sanitise(void);
    public: void print(bool show_rational=false) const;
    public: void set_name(std::string const& name);
    public: std::string get_name(void) const;
    public: void set_coeffs(std::vector<double> const& coeffs);
    public: std::vector<double> get_coeffs(void) const;
    public: double evaluate(double coord) const;
    public: int degree(void) const;
};

/******************************************************************************
 * Constructor. Creates the zero polynomial and gives it a generic
 * polynomial-looking name.
 *****************************************************************************/
Polynomial::Polynomial(void)
    : coeffs({}), name("p")
{
}

/******************************************************************************
 * Constructor. Creates a polynomial with the given coefficients and gives it a
 * generic polynomial-looking name.
 *
 * @param coeffs Vector containing the desired coefficients of the polynomial.
 *****************************************************************************/
Polynomial::Polynomial(std::vector<double> const& coeffs)
    : coeffs(coeffs), name("p")
{
    sanitise();
}

/******************************************************************************
 * Constructor. Creates a polynomial with the given coefficients and name.
 *
 * @param name Desired name of the polynomial.
 * @param coeffs Vector containing the desired coefficients of the polynomial.
 *****************************************************************************/
Polynomial::Polynomial(std::vector<double> const& coeffs, std::string const& name)
    : coeffs(coeffs), name(name)
{
    sanitise();
}

/******************************************************************************
 * Replace coefficients of extremely small magnitude with zeros. Then delete
 * trailing zeros from the coefficient vector. It does not make sense to
 * represent non-existent coefficients after the highest power.
 *
 * For instance, if, for some reason, the coefficient vector becomes
 * \code{.unparsed}
 *     [3.3, 1.97, 8, 0, 4.2, 0, 1e-17, 0]
 * \endcode
 * this method will remove the last three coefficients.
 *****************************************************************************/
void Polynomial::sanitise(void)
{
    for(double& d: coeffs)
    {
        if(std::abs(d) <= 1e-10)
        {
            d = 0;
        }
    }
    while(!coeffs.empty() && coeffs.back() == 0)
    {
        coeffs.pop_back();
    }
}

/******************************************************************************
 * Display the vector of the coefficients of the polynomial.
 *
 * @param show_rational Whether the coefficients should be represented as
 *     rational numbers (i.e. ratios of integers) or not.
 *****************************************************************************/
void Polynomial::print(bool show_rational) const
{
    std::cout << name << " ≡ [";
    for(auto i = coeffs.begin(); i != coeffs.end(); ++i)
    {
        if(i != coeffs.begin())
        {
            std::cout << ", ";
        }
        if(show_rational)
        {
            std::cout << rationalise(*i);
        }
        else
        {
            std::cout << std::setprecision(12) << *i;
        }
    }
    std::cout << "]\n";
}

/******************************************************************************
 * Set the name of the polynomial. This could be used after the name of a
 * polynomial has been set automatically after a mathematical operation (if
 * desired). There is no way for the program to know the name of the variable
 * used for the polynomial object, so the user must supply the name.
 *
 * @param name Desired name of the polynomial.
 *****************************************************************************/
void Polynomial::set_name(std::string const& name)
{
    this->name = name;
}

/******************************************************************************
 * Obtain the name of the polynomial.
 *
 * @return Name of the polynomial.
 *****************************************************************************/
std::string Polynomial::get_name(void) const
{
    return name;
}

/******************************************************************************
 * Set the coefficients of the polynomial.
 *
 * @param coeffs Vector containing the desired coefficients of the polynomial.
 *****************************************************************************/
void Polynomial::set_coeffs(std::vector<double> const& coeffs)
{
    this->coeffs = coeffs;
    this->sanitise();
}

/******************************************************************************
 * Obtain the coefficients of the polynomial.
 *
 * @return Vector containing the desired coefficients of the polynomial.
 *****************************************************************************/
std::vector<double> Polynomial::get_coeffs(void) const
{
    return coeffs;
}

/******************************************************************************
 * Evaluate the polynomial for some value of the variable. Use Homer's method
 * to calculate the result. (This reduces the number of computations required.)
 * For instance, if the polynomial is
 * \code{.unparsed}
 *     12.8x^5 - 1.62x^2 + 33x - 7.31
 * \endcode
 * then it will be evaluated as if it were written like this.
 * \code{.unparsed}
 *     ((((12.8x + 0)x + 0)x - 1.62)x + 33)x - 7.31
 * \endcode
 *
 * @param coord The coordinate at which the polynomial is to be evaluated.
 *
 * @return Value of the polynomial when its variable has the value of `coord`.
 *****************************************************************************/
double Polynomial::evaluate(double coord) const
{
    double result = 0;
    for(auto i = coeffs.rbegin(); i != coeffs.rend(); ++i)
    {
        result = result * coord + *i;
    }
    return result;
}

/******************************************************************************
 * Obtain the degree of the polynomial. This is one less than the length of the
 * coefficient vector. (I could have chosen to work with the length of the
 * vector, but the degree of the polynomial is a more concrete property.)
 *
 * @return Degree of the polynomial.
 *****************************************************************************/
int Polynomial::degree(void) const
{
    return coeffs.size() - 1;
}

/******************************************************************************
 * Define the sum of a polynomial and a constant. Create a new polynomial whose
 * constant term is the sum of the provided constant and the constant term of
 * the original polynomial. The name of this new polynomial will be a
 * combination of the entities being added.
 *****************************************************************************/
Polynomial operator+(Polynomial const& p, double f)
{
    std::string name = "(" + p.get_name() + " + " + std::to_string(f) + ")";
    std::vector<double> coeffs = p.get_coeffs();
    if(p.degree() == -1)
    {
        coeffs.push_back(0);
    }
    coeffs[0] += f;
    return Polynomial(coeffs, name);
}

/******************************************************************************
 * Define the sum of a constant and a polynomial. Create a new polynomial whose
 * constant term is the sum of the provided constant and the constant term of
 * the original polynomial. The name of this new polynomial will be a
 * combination of the entities being added.
 *****************************************************************************/
Polynomial operator+(double f, Polynomial const& p)
{
    std::string name = "(" + std::to_string(f) + " + " + p.get_name() + ")";
    std::vector<double> coeffs = p.get_coeffs();
    if(p.degree() == -1)
    {
        coeffs.push_back(0);
    }
    coeffs[0] += f;
    return Polynomial(coeffs, name);
}

/******************************************************************************
 * Define the sum of two polynomials. Create a new polynomial whose
 * coefficients are the sums of the corresponding coefficients of the two
 * polynomials. The name of this new polynomial will be a combination of the
 * entities being added. Its degree is equal to the greater of their degrees.
 *****************************************************************************/
Polynomial operator+(Polynomial const& p, Polynomial const& q)
{
    std::string name = "(" + p.get_name() + " + " + q.get_name() + ")";
    std::vector<double> const& coeffs_p = p.get_coeffs();
    std::vector<double> const& coeffs_q = q.get_coeffs();
    std::vector<double> coeffs(std::max(p.degree(), q.degree()) + 1);
    int i = 0;
    for(; i <= std::min(p.degree(), q.degree()); ++i)
    {
        coeffs[i] = coeffs_p[i] + coeffs_q[i];
    }
    for(; i <= p.degree(); ++i)
    {
        coeffs[i] = coeffs_p[i];
    }
    for(; i <= q.degree(); ++i)
    {
        coeffs[i] = coeffs_q[i];
    }
    return Polynomial(coeffs, name);
}

/******************************************************************************
 * Define polynomial subtraction. Same rules as addition apply.
 *****************************************************************************/
Polynomial operator-(Polynomial const& p, double f)
{
    std::string name = "(" + p.get_name() + " - " + std::to_string(f) + ")";
    std::vector<double> coeffs = p.get_coeffs();
    if(p.degree() == -1)
    {
        coeffs.push_back(0);
    }
    coeffs[0] -= f;
    return Polynomial(coeffs, name);
}

/******************************************************************************
 * Define polynomial subtraction. Same rules as addition apply.
 *****************************************************************************/
Polynomial operator-(double f, Polynomial const& p)
{
    std::string name = "(" + std::to_string(f) + " - " + p.get_name() + ")";
    std::vector<double> coeffs = p.get_coeffs();
    std::transform(coeffs.begin(), coeffs.end(), coeffs.begin(), std::negate<double>());
    if(coeffs.size() == 0)
    {
        coeffs.push_back(0);
    }
    coeffs[0] += f;
    return Polynomial(coeffs, name);
}

/******************************************************************************
 * Define polynomial subtraction. Same rules as addition apply.
 *****************************************************************************/
Polynomial operator-(Polynomial const& p, Polynomial const& q)
{
    std::string name = "(" + p.get_name() + " - " + q.get_name() + ")";
    std::vector<double> const& coeffs_p = p.get_coeffs();
    std::vector<double> const& coeffs_q = q.get_coeffs();
    std::vector<double> coeffs(std::max(p.degree(), q.degree()) + 1);
    int i = 0;
    for(; i <= std::min(p.degree(), q.degree()); ++i)
    {
        coeffs[i] = coeffs_p[i] - coeffs_q[i];
    }
    for(; i <= p.degree(); ++i)
    {
        coeffs[i] = coeffs_p[i];
    }
    for(; i <= q.degree(); ++i)
    {
        coeffs[i] = -coeffs_q[i];
    }
    return Polynomial(coeffs, name);
}

/******************************************************************************
 * Define the product of a polynomial and a constant. Create a new polynomial
 * whose coefficients are the product of the constant and the corresponding
 * coefficients of the original polynomial. The name of this new polynomial
 * will be a combination of the entities being multiplied.
 *****************************************************************************/
Polynomial operator*(Polynomial const& p, double f)
{
    std::string name = "(" + p.get_name() + " * " + std::to_string(f) + ")";
    std::vector<double> coeffs = p.get_coeffs();
    for(double& d: coeffs)
    {
        d *= f;
    }
    return Polynomial(coeffs, name);
}

/******************************************************************************
 * Define the product of a constant and a polynomial. Create a new polynomial
 * whose coefficients are the product of the constant and the corresponding
 * coefficients of the original polynomial. The name of this new polynomial
 * will be a combination of the entities being multiplied.
 *****************************************************************************/
Polynomial operator*(double f, Polynomial const& p)
{
    std::string name = "(" + std::to_string(f) + " * " + p.get_name() + ")";
    std::vector<double> coeffs = p.get_coeffs();
    for(double& d: coeffs)
    {
        d *= f;
    }
    return Polynomial(coeffs, name);
}

/******************************************************************************
 * Define the product of two polynomials. Create a new polynomial whose
 * coefficients are the linear convolution of those of the two polynomials. The
 * name of this new polynomial will be a combination of the entities being
 * multiplied. Its degree is equal to the sum of their degrees.
 *****************************************************************************/
Polynomial operator*(Polynomial const& p, Polynomial const& q)
{
    std::string name = "(" + p.get_name() + " * " + q.get_name() + ")";
    std::vector<double> const& coeffs_p = p.get_coeffs();
    std::vector<double> const& coeffs_q = q.get_coeffs();
    std::vector<double> coeffs(p.degree() + q.degree() + 1, 0);
    for(int n = 0; n <= p.degree() + q.degree(); ++n)
    {
        for(int k = 0; k <= n; ++k)
        {
            // The redundant check for a negative value makes the bound check
            // obvious.
            if(k < 0 || k > p.degree() || n - k < 0 || n - k > q.degree())
            {
                continue;
            }

            coeffs[n] += coeffs_p[k] * coeffs_q[n - k];
        }
    }
    return Polynomial(coeffs, name);
}

/******************************************************************************
 * Define the quotient of the division of a polynomial be a constant. Create a
 * new polynomial whose coefficients are the ratio of the corresponding
 * coefficients of the original polynomial and the constant. The name of this
 * new polynomial will be a combination of the entities involved.
 *****************************************************************************/
Polynomial operator/(Polynomial const& p, double f)
{
    std::string name = "(" + p.get_name() + " / " + std::to_string(f) + ")";
    std::vector<double> coeffs = p.get_coeffs();
    for(double& d: coeffs)
    {
        d /= f;
    }
    return Polynomial(coeffs, name);
}

/******************************************************************************
 * Calculate the greatest common divisor of two integers. This implementation
 * is provided because older versions of GCC do not provide an implementation
 * of `std::gcd`, even though they may support the C++17 standard.
 *
 * @param m Positive integer.
 * @param n Positive integer.
 *
 * @return GCD of `m` and `n`.
 *****************************************************************************/
template<typename Type>
Type my_gcd(Type m, Type n)
{
    while(n != 0)
    {
        Type t = m % n;
        m = n;
        n = t;
    }
    return m;
}

/******************************************************************************
 * Approximate a real number as a rational number with a small denominator.
 * Much of this code is copied from that of the `limit_denominator` method of
 * the Python class `fractions.Fraction`.
 *
 * @param real Number to approximate.
 * @param max_denominator Maximum denominator the approximation may have.
 *
 * @return Rational approximation of `real`.
 *****************************************************************************/
std::string rationalise(double real, ilong max_denominator)
{
    ilong real_to_int = static_cast<int>(real);
    if(real_to_int == real)
    {
        return std::to_string(real_to_int);
    }

    // Separate the sign of `real` from its magnitude. This allows the
    // algorithm to focus on positive values.
    std::string sign;
    if(real < 0)
    {
        sign = "-";
    }
    real = std::abs(real);

    // Obtain a rational approximation with a large denominator. If the
    // denominator is small after removing the GCD, that's the answer.
    // `ilong` is used because `int` may overflow.
    ilong d = 1e12;
    ilong n = real * d;
    ilong g = my_gcd(n, d);
    n /= g;
    d /= g;
    if(d <= max_denominator)
    {
        if(d == 1)
        {
            return sign + std::to_string(n);
        }

        return sign + std::to_string(n) + "/" + std::to_string(d);
    }

    // Converge to an approximation with a smaller denominator.
    ilong p0 = 0, q0 = 1, p1 = 1, q1 = 0;
    while(true)
    {
        ilong a = n / d;
        ilong q2 = q0 + a * q1;
        if(q2 > max_denominator)
        {
            break;
        }

        ilong p1_old = p1, q1_old = q1;
        p1 = p0 + a * p1;
        q1 = q2;
        p0 = p1_old;
        q0 = q1_old;

        ilong d_old = d;
        d = n - a * d;
        n = d_old;
    }

    // Obtain two bounds. Choose the one with the smaller denominator if both
    // are equally accurate.
    ilong k = (max_denominator - q0) / q1;
    double bound1 = static_cast<double>(p0 + k * p1) / (q0 + k * q1);
    double bound2 = static_cast<double>(p1) / q1;
    if(std::abs(bound2 - real) <= std::abs(bound1 - real))
    {
        n = p1;
        d = q1;
    }
    else
    {
        n = p0 + k * p1;
        d = q0 + k * q1;
    }

    if(d == 1)
    {
        return sign + std::to_string(n);
    }

    return sign + std::to_string(n) + "/" + std::to_string(d);
}

/******************************************************************************
 * Given two vectors, generate the interpolating polynomial on which all the
 * points represented by those vectors lie. A Lagrange interpolating polynomial
 * is built using the given points. Although it may increase the processing
 * required, it is easier to understand and debug the code, because it
 * resembles the mathematical formula.
 *
 * No memory leaks occur in this loop, as confirmed by Valgrind.
 *
 * @param x Vector of x-coordinates.
 * @param y Vector of y-coordinates.
 *
 * @return Interpolating polynomial which passes through all points represented
 *     by `x` and `y`.
 *****************************************************************************/
Polynomial interpolate(std::vector<double> const& x, std::vector<double> const& y)
{
    if(x.size() <= 1 || y.size() <= 1)
    {
        throw std::invalid_argument("At least two points are required for interpolation.");
    }

    std::set<double> s(x.begin(), x.end());
    if(s.size() != x.size())
    {
        throw std::invalid_argument("Interpolating points must have unique x-coordinates.");
    }

    // If the vectors are of different lengths, extra coordinates are ignored.
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
    result.set_name("ip");
    return result;
}

#endif // SEQUENCE_INTERPOLATION_POLYNOMIAL_CPP
