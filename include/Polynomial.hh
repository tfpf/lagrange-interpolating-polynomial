#ifndef LAGRANGE_INTERPOLATING_POLYNOMIAL_INCLUDE_POLYNOMIAL_HH_
#define LAGRANGE_INTERPOLATING_POLYNOMIAL_INCLUDE_POLYNOMIAL_HH_

#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

class Polynomial: public std::vector<double>
{
    public:
    bool rational;

    public:
    Polynomial();
    Polynomial(std::initializer_list<double> const& list);
    Polynomial(std::vector<double> const& vector);
    Polynomial(std::vector<double> const& xcoords, std::vector<double> const& ycoords);
    void sanitise(void);
    double operator()(double x);
};

std::ostream& operator<<(std::ostream& ostream, Polynomial const& p);
void operator+=(Polynomial& p, Polynomial const& q);
Polynomial operator+(Polynomial const& p, Polynomial const& q);
void operator-=(Polynomial& p, Polynomial const& q);
Polynomial operator-(Polynomial const& p, Polynomial const& q);
void operator*=(Polynomial& p, Polynomial const& q);
Polynomial operator*(Polynomial const& p, Polynomial const& q);
void operator/=(Polynomial& p, double d);
Polynomial operator/(Polynomial const& p, double d);

std::string rationalise(double number, int long long max_denominator=1000000);

#endif  // LAGRANGE_INTERPOLATING_POLYNOMIAL_INCLUDE_POLYNOMIAL_HH_
