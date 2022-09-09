#ifndef SEQUENCE_INTERPOLATION_INCLUDE_POLYNOMIAL_HH_
#define SEQUENCE_INTERPOLATION_INCLUDE_POLYNOMIAL_HH_

#include <string>
#include <vector>

typedef int long long ll;

std::string rationalise(double real, ll max_denominator=1000000);

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

Polynomial interpolate(std::vector<double> const& x, std::vector<double> const& y);

#endif  // SEQUENCE_INTERPOLATION_INCLUDE_POLYNOMIAL_HH_
