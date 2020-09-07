#include <algorithm>
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
    set_name: set the polynomial name
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
Constructor. Does default initialisation.
-----------------------------------------------------------------------------*/
Polynomial::Polynomial()
{
    name = "p(x)";
    coeffs = {};
}

/*-----------------------------------------------------------------------------
Constructor. Initialises the polynomial coefficients using the vector provided
and assign a default name.
-----------------------------------------------------------------------------*/
Polynomial::Polynomial(std::vector<double> coeffs)
{
    this->name = "p(x)";
    this->coeffs = coeffs;
    this->sanitise();
}

/*-----------------------------------------------------------------------------
Constructor. Initialises the name and the polynomial coefficients using the
variables provided.

Args:
    name: string (symbol or group of symbols used to represent the polynomial)
    coeffs: vector (coefficients of the polynomial, ordered as described above)
-----------------------------------------------------------------------------*/
Polynomial::Polynomial(std::vector<double> coeffs, std::string name)
{
    this->name = name;
    this->coeffs = coeffs;
    this->sanitise();
}

/*-----------------------------------------------------------------------------
Delete any trailing zeros in the vector of the coefficients. At any point,
possibly after an operation, if the vector contains zeros at the end, they must
be removed. It does not make sense to represent non-existent coefficients after
the highest power.

For instance, if, for some reason, the coefficient vector becomes
    [3.3, 1.97, 8, 0, 4.2, 0, 0]
it has to be sanitised: the last two elements in the vector have to be removed.
-----------------------------------------------------------------------------*/
void Polynomial::sanitise(void)
{
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
        std::cout << *i << ", ";
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
Define polynomial addition. When two polynomials of different degrees are
added, the result has to be a polynomial with degree equal to the greater of
the two degrees.

The name of the resulting polynomial will be a combination of the names of the
polynomials being added. Brackets will enclose this combination, because, after
repeated mathematical operations, the name may not match the value because of
operator precedence.
-----------------------------------------------------------------------------*/
Polynomial operator+(Polynomial const& p, Polynomial const& q)
{
    // set up the name of the resulting polynomial
    std::string name = "(" + p.get_name() + " + " + q.get_name() + ")";

    // ensure that both vectors are of equal size
    std::vector<double> coeffs1 = p.get();
    std::vector<double> coeffs2 = q.get();
    int degree = std::max(p.degree(), q.degree());
    coeffs1.resize(degree + 1, 0);
    coeffs2.resize(degree + 1, 0);

    // set up the vector for the coefficients of the result
    std::vector<double> coeffs(degree + 1);
    for(int i = 0; i <= degree; ++i)
    {
        coeffs[i] = coeffs1[i] + coeffs2[i];
    }
    Polynomial result(coeffs, name);

    return result;
}

/*-----------------------------------------------------------------------------
Define polynomial subtraction. Same rules as addition apply: the different
degrees case has to be taken care of.
-----------------------------------------------------------------------------*/
Polynomial operator-(Polynomial const& p, Polynomial const& q)
{
    // set up the name of the resulting polynomial
    std::string name = "(" + p.get_name() + " - " + q.get_name() + ")";

    // ensure that both vectors are of equal size
    std::vector<double> coeffs1 = p.get();
    std::vector<double> coeffs2 = q.get();
    int degree = std::max(p.degree(), q.degree());
    coeffs1.resize(degree + 1);
    coeffs2.resize(degree + 1);

    // set up the vector for the coefficients of the result
    std::vector<double> coeffs(degree + 1);
    for(int i = 0; i <= degree; ++i)
    {
        coeffs[i] = coeffs1[i] - coeffs2[i];
    }
    Polynomial result(coeffs, name);

    return result;
}

/*-----------------------------------------------------------------------------
Define polynomial multiplication. The coefficient vector of the product of two
polynomials is the convolution of their coefficient vectors.
-----------------------------------------------------------------------------*/
Polynomial operator*(Polynomial const& p, Polynomial const& q)
{
    // set up the name of the resulting polynomial
    std::string name = "(" + p.get_name() + " * " + q.get_name() + ")";

    std::vector<double> coeffs1 = p.get();
    std::vector<double> coeffs2 = q.get();
    int psize = coeffs1.size();
    int qsize = coeffs2.size();

    // set up the vector for the coefficients of the result
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

            coeffs[n] += coeffs1[k] * coeffs2[n - k];
        }
    }
    Polynomial result(coeffs, name);

    return result;
}

/*-----------------------------------------------------------------------------
Define the division of a polynomial by a mathematical constant. This isn't
polynomial division proper.
-----------------------------------------------------------------------------*/
Polynomial operator/(Polynomial const& p, double f)
{
    // set up the name of the resulting polynomial
    std::string name = "(" + p.get_name() + " / " + std::to_string(f) + ")";

    // set up the vector for the coefficients of the result
    std::vector<double> coeffs = p.get();
    for(int i = 0; i <= p.degree(); ++i)
    {
        coeffs[i] /= f;
    }
    Polynomial result(coeffs, name);

    return result;
}

