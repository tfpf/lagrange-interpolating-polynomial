#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

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
    print: display the polynomial
    set_name: set the polynomial name to the user-provided string
    get_name: get the polynomial name
    set: set the coefficient vector to the user-provided vector
    get: get the coefficient vector
    evaluate: evaluate the polynomial at a certain point
    degree: get the degree of the polynomial
-----------------------------------------------------------------------------*/
class Polynomial
{
    private: std::string name;
    private: std::vector<double> coeffs;

    public: Polynomial();
    public: Polynomial(std::vector<double>);
    public: Polynomial(std::vector<double>, std::string);
    public: void sanitise(void);
    public: void print(void) const;
    public: void set_name(std::string);
    public: std::string get_name(void) const;
    public: void set(std::vector<double>);
    public: std::vector<double> get(void) const;
    public: double evaluate(double) const;
    public: int degree(void) const;
};

/*-----------------------------------------------------------------------------
Constructor. Sets the name of the polynomial to a generic polynomial-looking
name and initialises the coefficient vector to an empty vector.
-----------------------------------------------------------------------------*/
Polynomial::Polynomial()
    : name("p(x)"), coeffs({})
{
}

/*-----------------------------------------------------------------------------
Constructor. Sets the name of the polynomial to a generic polynomial-looking
name and initialises the coefficient vector to the user-provided vector.

Args:
    coeffs: vector (coefficients of the polynomial)
-----------------------------------------------------------------------------*/
Polynomial::Polynomial(std::vector<double> coeffs)
    : name("p(x)"), coeffs(coeffs)
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
Polynomial::Polynomial(std::vector<double> coeffs, std::string name)
    : name(name), coeffs(coeffs)
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
        std::cout << std::setprecision(12) << *i << ", ";
    }
    std::cout << "]\n";
}

/*-----------------------------------------------------------------------------
Set the name of the polynomial. This could be used after the name of a
polynomial has been set automatically after a mathematical operation. There is
no way for the program to know the name of the variable used for the polynomial
object, so the user must supply the name. (Otherwise, the automatically set
name can be left unchanged.)

Args:
    name: string (to set the polynomial name)
-----------------------------------------------------------------------------*/
void Polynomial::set_name(std::string name)
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

Args:
    coeffs: vector (coefficients to be set)
-----------------------------------------------------------------------------*/
void Polynomial::set(std::vector<double> coeffs)
{
    std::vector<double>().swap(this->coeffs);
    this->coeffs = coeffs;
    this->sanitise();
}

/*-----------------------------------------------------------------------------
Obtain the vector of the coefficients of the polynomial. I don't expect to use
this function much. It is written primarily for completeness (there is already
a `set' method) and for debugging.

Returns:
    vector of coefficients
-----------------------------------------------------------------------------*/
std::vector<double> Polynomial::get(void) const
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

TODO At the moment, the addition of two polynomials is implemented in a very
clumsy manner, though it is the best I could come up with right now. Improve
that logic later.
-----------------------------------------------------------------------------*/
Polynomial operator+(Polynomial const& p, double f)
{
    std::string name = "(" + p.get_name() + " + " + std::to_string(f) + ")";
    std::vector<double> coeffs = p.get();
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
    std::vector<double> coeffs = p.get();
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

    std::vector<double> const& coeffs_p = p.get();
    std::vector<double> const& coeffs_q = q.get();
    int min_degree = std::min(p.degree(), q.degree());
    int max_degree = std::max(p.degree(), q.degree());
    std::vector<double> coeffs(max_degree + 1);

    int i = 0;
    for(; i <= min_degree; ++i)
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
    std::vector<double> coeffs = p.get();
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
    std::vector<double> coeffs = p.get();
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

    std::vector<double> const& coeffs_p = p.get();
    std::vector<double> const& coeffs_q = q.get();
    int min_degree = std::min(p.degree(), q.degree());
    int max_degree = std::max(p.degree(), q.degree());
    std::vector<double> coeffs(max_degree + 1);

    int i = 0;
    for(; i <= min_degree; ++i)
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
    std::vector<double> coeffs = p.get();
    for(double& d: coeffs)
    {
        d *= f;
    }

    return Polynomial(coeffs, name);
}

Polynomial operator*(double f, Polynomial const& p)
{
    std::string name = "(" + std::to_string(f) + " * " + p.get_name() + ")";
    std::vector<double> coeffs = p.get();
    for(double& d: coeffs)
    {
        d *= f;
    }

    return Polynomial(coeffs, name);
}

Polynomial operator*(Polynomial const& p, Polynomial const& q)
{
    std::string name = "(" + p.get_name() + " * " + q.get_name() + ")";

    std::vector<double> const& coeffs_p = p.get();
    std::vector<double> const& coeffs_q = q.get();
    int psize = coeffs_p.size();
    int qsize = coeffs_q.size();

    std::vector<double> coeffs(psize + qsize - 1, 0);
    for(int n = 0; n < psize + qsize - 1; ++n)
    {
        for(int k = 0; k <= n; ++k)
        {
            // included redundancy in this check to make bound check obvious
            if(k < 0 || k >= psize || n - k < 0 || n - k >= qsize)
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
    std::vector<double> coeffs = p.get();
    for(double& d: coeffs)
    {
        d /= f;
    }

    return Polynomial(coeffs, name);
}

