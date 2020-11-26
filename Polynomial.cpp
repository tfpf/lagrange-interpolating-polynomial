#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

/*-----------------------------------------------------------------------------
Class declaration.
-----------------------------------------------------------------------------*/
class Polynomial;

/*-----------------------------------------------------------------------------
Function prototypes.
-----------------------------------------------------------------------------*/
std::string rationalise(double real, int long max_denominator = 1000000);
Polynomial interpolate(std::vector<double> const& x, std::vector<double> const& y);

/*-----------------------------------------------------------------------------
A class to store a polynomial. The polynomial is stored in terms of its
coefficients. For instance, if the polynomial is
    12.8x^5 - 1.62x^2 + 33x - 7.31
then it will be stored as the vector
    [-7.31, 33, -1.62, 0, 0, 12.8]
so that the coefficients are arranged in increasing order of the power of the
variable they are associated with. Which makes things simple, because the power
of the variable will be simply the index of the coefficient in the vector.

Members:
    name: string (symbol or group of symbols used to represent the polynomial)
    coeffs: vector (coefficients of the polynomial, ordered as described above)

Methods:
    Polynomial: constructor
    sanitise: remove trailing zeros from the coefficient vector
    print: display the coefficients of the polynomial
    set_name: set the polynomial name to the user-provided string
    get_name: get the polynomial name
    set_coeffs: set the coefficient vector to the user-provided vector
    get_coeffs: get the coefficient vector
    evaluate: evaluate the polynomial at a certain point
    degree: get the degree of the polynomial
-----------------------------------------------------------------------------*/
class Polynomial
{
    // members
    private: std::vector<double> coeffs;
    private: std::string name;

    // methods
    public: Polynomial(void);
    public: Polynomial(std::vector<double> const&);
    public: Polynomial(std::vector<double> const&, std::string const&);
    public: void sanitise(void);
    public: void print(void) const;
    public: void set_name(std::string const&);
    public: std::string get_name(void) const;
    public: void set_coeffs(std::vector<double> const&);
    public: std::vector<double> get_coeffs(void) const;
    public: double evaluate(double) const;
    public: int degree(void) const;
};

/*-----------------------------------------------------------------------------
Constructor. Sets the name of the polynomial to a generic polynomial-looking
name and initialises the coefficient vector to an empty vector.
-----------------------------------------------------------------------------*/
Polynomial::Polynomial(void)
    : coeffs({}), name("p")
{
}

/*-----------------------------------------------------------------------------
Constructor. Sets the name of the polynomial to a generic polynomial-looking
name and initialises the coefficient vector to the user-provided vector.

Args:
    coeffs: vector (coefficients of the polynomial)
-----------------------------------------------------------------------------*/
Polynomial::Polynomial(std::vector<double> const& coeffs)
    : coeffs(coeffs), name("p")
{
    sanitise();
}

/*-----------------------------------------------------------------------------
Constructor. Sets the name and coefficients of the polynomial to what the user
has provided.

Args:
    name: string (symbol or group of symbols used to represent the polynomial)
    coeffs: vector (coefficients of the polynomial)
-----------------------------------------------------------------------------*/
Polynomial::Polynomial(std::vector<double> const& coeffs, std::string const& name)
    : coeffs(coeffs), name(name)
{
    sanitise();
}

/*-----------------------------------------------------------------------------
Delete any trailing zeros in the vector of the coefficients. At any point,
possibly after an operation, if the vector contains zeros at the end, they must
be removed. It does not make sense to represent non-existent coefficients after
the highest power.

Another thing this method will do is remove extremely small coefficients which
are likely the result of floating-point precision problems (extremely small
values should probably have been zeros anyway).

For instance, if, for some reason, the coefficient vector becomes
    [3.3, 1.97, 8, 0, 4.2, 0, 1e-17, 0]
it has to be sanitised: the last three elements in the vector have to be
removed.
-----------------------------------------------------------------------------*/
void Polynomial::sanitise(void)
{
    // remove small numbers
    for(double& d: coeffs)
    {
        if(std::abs(d) <= 1e-10)
        {
            d = 0;
        }
    }

    // delete trailing zeros
    while(!coeffs.empty() && coeffs.back() == 0)
    {
        coeffs.pop_back();
    }
}

/*-----------------------------------------------------------------------------
Display the polynomial as if it were a vector of its coefficients. Use square
brackets around them to enforce the notion that the order of these numbers is
significant. (Since a vector in C++ is not the same as a set in maths.)
-----------------------------------------------------------------------------*/
void Polynomial::print(void) const
{
    std::cout << name << " = [";
    for(auto i = coeffs.begin(); i != coeffs.end(); ++i)
    {
        std::cout << rationalise(*i) << ", ";
    }
    std::cout << "]\n";
}

/*-----------------------------------------------------------------------------
Set the name of the polynomial. This could be used after the name of a
polynomial has been set automatically after a mathematical operation. There is
no way for the program to know the name of the variable used for the polynomial
object, so the user must supply the name. (Otherwise, the automatically set
name can be left unchanged.)

Note to self
There is no need to deallocate the previous name, because `std::string' handles
memory allocation and deallocation for the programmer.

Args:
    name: string (to set the polynomial name)
-----------------------------------------------------------------------------*/
void Polynomial::set_name(std::string const& name)
{
    this->name = name;
}

/*-----------------------------------------------------------------------------
Obtain the name of the polynomial. This will be used to set the name of a new
polynomial which has been created as a result of some supported mathematical
operation.

Returns:
    name of the polynomial
-----------------------------------------------------------------------------*/
std::string Polynomial::get_name(void) const
{
    return name;
}

/*-----------------------------------------------------------------------------
Set the vector of the coefficients of the polynomial. Before doing this, the
existing vector is swapped with an unallocated vector, effectively deallocating
the previously used memory.

Note to self
There is no need to deallocate the previous vector, because `std::vector'
handles memory allocation and deallocation for the programmer.

Args:
    coeffs: vector (coefficients to be set)
-----------------------------------------------------------------------------*/
void Polynomial::set_coeffs(std::vector<double> const& coeffs)
{
    this->coeffs = coeffs;
    this->sanitise();
}

/*-----------------------------------------------------------------------------
Obtain the vector of the coefficients of the polynomial.

Returns:
    vector of coefficients
-----------------------------------------------------------------------------*/
std::vector<double> Polynomial::get_coeffs(void) const
{
    return coeffs;
}

/*-----------------------------------------------------------------------------
Evaluate the polynomial for some value of the variable. Use Homer's method to
calculate the result. This reduces the number of computations required. For
instance, if the polynomial is
    12.8x^5 - 1.62x^2 + 33x - 7.31
then it will be evaluated as if it were written like this.
    ((((12.8x + 0)x + 0)x - 1.62)x + 33)x - 7.31

Args:
    coord: double (the coordinate at which the polynomial is to be evaluated)

Returns:
    value of the polynomial when its variable takes the value of `coord'
-----------------------------------------------------------------------------*/
double Polynomial::evaluate(double coord) const
{
    double result = 0;
    for(auto i = coeffs.rbegin(); i != coeffs.rend(); ++i)
    {
        result = result * coord + *i;
    }

    return result;
}

/*-----------------------------------------------------------------------------
Obtain the degree of the polynomial. This is, quite simply, one less than the
length of the coefficient vector. I could have chosen to work with the length
of the vector, but the degree of the polynomial is a more concrete property of
the polynomial.

Returns:
    degree of the polynomial
-----------------------------------------------------------------------------*/
int Polynomial::degree(void) const
{
    return coeffs.size() - 1;
}

/*-----------------------------------------------------------------------------
Define polynomial addition. The user may want to add two polynomials, or a
polynomial and a mathematical constant.

If a polynomial and a mathematical constant are added, the first coefficient of
the polynomial (if it exists) will get incremented by the provided constant.
(If it doesn't exist, it has to be created and set to the provided constant.)

If two polynomials are added, their corresponding coefficients are added. If
their degrees are not the same, the result has to be a polynomial with degree
equal to the greater of the two degrees; some coefficients in the polynomial
with greater degree remain untouched because they have no corresponding
coefficients in the other polynomial.

The name of the resulting polynomial will be a combination of the entities
being added. Brackets will enclose this combination, because, after repeated
mathematical operations, the name may not match the value because of operator
precedence.
-----------------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------
Define polynomial subtraction. Same rules as addition apply.
-----------------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------
Define polynomial multiplication.

When a polynomial and a mathematical constant are multiplied, the coefficients
of the resulting polynomial are obtained by multiplying the coefficients of the
multiplicand polynomial by the multiplying factor.

The coefficients of the product of two polynomials is obtained by performing
linear convolution on the coefficients of the two polynomials.
-----------------------------------------------------------------------------*/
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
            // included redundancy in this check to make bound check obvious
            if(k < 0 || k > p.degree() || n - k < 0 || n - k > q.degree())
            {
                continue;
            }

            coeffs[n] += coeffs_p[k] * coeffs_q[n - k];
        }
    }

    return Polynomial(coeffs, name);
}

/*-----------------------------------------------------------------------------
Define the division of a polynomial by a mathematical constant. Division of a
constant by a polynomial and division of a polynomial by a polynomial are not
implemented. (They are not required. For now.)
-----------------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------
Approximate a real number as a rational number with a small denominator. Much
of this code is copied from that of the `limit_denominator' method of the
Python class `fractions.Fraction'.

Note to self
If an `std::string' is not explicitly initialised, its default constructor is
called; it is automatically initialised as an empty string.

Args:
    real: double (the number to approximate)
    max_denominator: int long (maximum denominator the approximation may have)

Returns:
    rational approximation to the given number written as a string
-----------------------------------------------------------------------------*/
std::string rationalise(double real, int long max_denominator)
{
    // if the number is an integer (as best as can be told), return the integer
    int real_to_int = static_cast<int>(real);
    if(real_to_int == real)
    {
        return std::to_string(real_to_int);
    }

    // separate the sign of `real' from its magnitude
    // this allows the algorithm to focus on positive values
    std::string sign;
    if(real < 0)
    {
        sign = "-";
    }
    real = std::abs(real);

    // obtain a rational approximation with a large denominator
    // if, after removing the GCD, the denominator is small, that's the answer
    // use `long int' (otherwise, `real * d' may cause integer overflow)
    int long d = 1e15;
    int long n = real * d;
    int long g = std::gcd(n, d);
    n /= g;
    d /= g;
    if(d <= max_denominator)
    {
        return sign + std::to_string(n) + "/" + std::to_string(d);
    }

    // converge to an approximation with a smaller denominator
    int long p0 = 0, q0 = 1, p1 = 1, q1 = 0;
    while(1)
    {
        int long a = n / d;
        int long q2 = q0 + a * q1;
        if(q2 > max_denominator)
        {
            break;
        }

        int long p1_old = p1, q1_old = q1;
        p1 = p0 + a * p1;
        q1 = q2;
        p0 = p1_old;
        q0 = q1_old;

        int long d_old = d;
        d = n - a * d;
        n = d_old;
    }

    int long k = (max_denominator - q0) / q1;
    double bound1 = (double)(p0 + k * p1) / (q0 + k * q1);
    double bound2 = (double)p1 / q1;

    std::string rational;
    if(std::abs(bound2 - real) <= std::abs(bound1 - real))
    {
        rational = sign + std::to_string(p1) + "/" + std::to_string(q1);
    }
    else
    {
        rational = sign + std::to_string(p0 + k * p1) + "/" + std::to_string(q0 + k * q1);
    }

    return rational;
}

/*-----------------------------------------------------------------------------
Given two vectors, generate the interpolating polynomial on which all the
points represented by those vectors lie. A Lagrange Interpolating Polynomial
is built using the given points. Although it may increase the processing
required, it is easier to understand and debug the code, because the code
expressions resemble the mathematical expressions.

Note to self
The assignments used in the nested `for' loop in this function will not cause a
memory leak because the constructors and destructors added automatically will
take care of memory alocation and deallocation. You can confirm this using
Valgrind; you'll see that no heap memory is in use at the end of the program.

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
    result.set_name("ip");

    return result;
}

