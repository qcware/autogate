#pragma once

#include <stdexcept>
#include <utility>
#include <vector>
#include <map>
#include <complex>
#include <algorithm>

namespace autogate {

class TrigMonomial {

public:

TrigMonomial(
    const std::vector<std::pair<char, int>>& variables) :
    variables_(variables)
{
    if (!std::is_sorted(variables_.begin(), variables_.end())) throw std::runtime_error("variables must be sorted");
    for (auto variable : variables) {
        if (std::get<1>(variable) == 0) throw std::runtime_error("zero order not permitted");
    }
    for (ssize_t index = 0; index < ((ssize_t) variables_.size()) - 1; index++) {
        if (std::get<0>(variables_[index]) == std::get<0>(variables_[index+1])) throw std::runtime_error("duplicate symbols not permitted");
    }
}

const std::vector<std::pair<char, int>>& variables() const { return variables_; }

static
TrigMonomial one() { return TrigMonomial({}); }

friend bool operator<(const TrigMonomial& a, const TrigMonomial& b) { return a.variables() < b.variables(); }
friend bool operator>(const TrigMonomial& a, const TrigMonomial& b) { return a.variables() > b.variables(); }
friend bool operator<=(const TrigMonomial& a, const TrigMonomial& b) { return a.variables() <= b.variables(); }
friend bool operator>=(const TrigMonomial& a, const TrigMonomial& b) { return a.variables() >= b.variables(); }
friend bool operator==(const TrigMonomial& a, const TrigMonomial& b) { return a.variables() == b.variables(); }
friend bool operator!=(const TrigMonomial& a, const TrigMonomial& b) { return a.variables() != b.variables(); }

TrigMonomial conj() const 
{ 
    std::vector<std::pair<char, int>> variables(variables_.size()); 
    for (size_t index = 0; index < variables_.size(); index++) {
        variables[index] = std::pair<char, int>(std::get<0>(variables_[index]), -std::get<1>(variables_[index]));
    }
    return TrigMonomial(variables);
}

friend TrigMonomial operator*(const TrigMonomial& a, const TrigMonomial& b) 
{
    std::vector<std::pair<char, int>> variables; 
    size_t indexa = 0;
    size_t indexb = 0;
    for (; indexa < a.variables().size() && indexb < b.variables().size();) {
        char symbola = std::get<0>(a.variables()[indexa]);
        char symbolb = std::get<0>(b.variables()[indexb]);
        int ordera = std::get<1>(a.variables()[indexa]);
        int orderb = std::get<1>(b.variables()[indexb]);
        if (symbola < symbolb) {
            variables.push_back(std::pair<char, int>(symbola, ordera));
            indexa++;
        } else if (symbolb > symbola) {
            variables.push_back(std::pair<char, int>(symbolb, orderb));
            indexb++;
        } else {
            if (ordera + orderb != 0) {
                variables.push_back(std::pair<char, int>(symbola, ordera + orderb));
            }
            indexa++;
            indexb++;
        }
    }
    for (; indexa < a.variables().size(); indexa++) {
        variables.push_back(a.variables()[indexa]);
    }
    for (; indexb < b.variables().size(); indexb++) {
        variables.push_back(b.variables()[indexb]);
    }

    return TrigMonomial(variables);
}

private:

std::vector<std::pair<char, int>> variables_;

};

class TrigPolynomial {

public:

TrigPolynomial(
    const std::map<TrigMonomial, std::complex<double>>& polynomial) :
    polynomial_(polynomial)
    {}

TrigPolynomial(){}

const std::map<TrigMonomial, std::complex<double>>& polynomial() const { return polynomial_; }

static 
TrigPolynomial zero() { return TrigPolynomial(); }

static 
TrigPolynomial one() { return TrigPolynomial({{ TrigMonomial::one(), 1.0}}); }

TrigPolynomial operator+() const 
{ 
    std::map<TrigMonomial, std::complex<double>> polynomial;
    for (auto const& it : polynomial_) {
        const TrigMonomial& monomial = it.first;
        const std::complex<double>& coefficient = it.second;
        polynomial[monomial] = +coefficient;
    }
    return TrigPolynomial(polynomial);
}
    
TrigPolynomial operator-() const 
{ 
    std::map<TrigMonomial, std::complex<double>> polynomial;
    for (auto const& it : polynomial_) {
        const TrigMonomial& monomial = it.first;
        const std::complex<double>& coefficient = it.second;
        polynomial[monomial] = -coefficient;
    }
    return TrigPolynomial(polynomial);
}

TrigPolynomial& operator+=(const TrigPolynomial& other)
{
    for (auto const& it : other.polynomial()) {
        polynomial_[it.first] += it.second;
    } 
    return *this;
}
    
TrigPolynomial& operator-=(const TrigPolynomial& other)
{
    for (auto const& it : other.polynomial()) {
        polynomial_[it.first] -= it.second;
    } 
    return *this;
}

TrigPolynomial operator+(const TrigPolynomial& other) const
{
    TrigPolynomial t = *this;
    t += other;
    return t;
}
    
TrigPolynomial operator-(const TrigPolynomial& other) const
{
    TrigPolynomial t = *this;
    t -= other;
    return t;
}
    
friend TrigPolynomial operator*(const TrigPolynomial& a, const TrigPolynomial& b)
{
    std::map<TrigMonomial, std::complex<double>> polynomial;
    for (auto const& ita : a.polynomial()) {
        for (auto const& itb : b.polynomial()) {
            polynomial[ita.first * itb.first] += ita.second * itb.second;
        }
    }
    return TrigPolynomial(polynomial);
}

TrigPolynomial operator*(const std::complex<double>& scalar) const 
{ 
    std::map<TrigMonomial, std::complex<double>> polynomial;
    for (auto const& it : polynomial_) {
        const TrigMonomial& monomial = it.first;
        const std::complex<double>& coefficient = it.second;
        polynomial[monomial] = coefficient * scalar;
    }
    return TrigPolynomial(polynomial);
}

friend TrigPolynomial operator*(const std::complex<double>& scalar, const TrigPolynomial& poly)
{
    return poly * scalar;
}

TrigPolynomial operator/(const std::complex<double>& scalar) const 
{ 
    std::map<TrigMonomial, std::complex<double>> polynomial;
    for (auto const& it : polynomial_) {
        const TrigMonomial& monomial = it.first;
        const std::complex<double>& coefficient = it.second;
        polynomial[monomial] = coefficient / scalar;
    }
    return TrigPolynomial(polynomial);
}

TrigPolynomial operator+=(const std::complex<double>& scalar)
{
    std::map<TrigMonomial, std::complex<double>> polynomial = polynomial_;
    for (auto const& it : polynomial_) {
        polynomial[it.first] = it.second + scalar;
    }
    return TrigPolynomial(polynomial);
}

TrigPolynomial operator+(const std::complex<double>& scalar) const
{
    std::map<TrigMonomial, std::complex<double>> polynomial = polynomial_;
    polynomial[TrigMonomial::one()] += scalar;
    return TrigPolynomial(polynomial);
}

friend TrigPolynomial operator+(const std::complex<double>& scalar, const TrigPolynomial& poly) 
{
    std::map<TrigMonomial, std::complex<double>> polynomial = poly.polynomial();
    polynomial[TrigMonomial::one()] += scalar;
    return TrigPolynomial(polynomial);
}

TrigPolynomial operator-=(const std::complex<double>& scalar)
{
    std::map<TrigMonomial, std::complex<double>> polynomial = polynomial_;
    for (auto const& it : polynomial_) {
        polynomial[it.first] = it.second - scalar;
    }
    return TrigPolynomial(polynomial);
}

friend TrigPolynomial operator-(const TrigPolynomial& poly, const std::complex<double>& scalar) 
{
    return poly + (-scalar);
}
    
friend TrigPolynomial operator-(const std::complex<double>& scalar, const TrigPolynomial& poly) 
{
    return -poly + scalar;
}

TrigPolynomial conj() const
{
    std::map<TrigMonomial, std::complex<double>> polynomial;
    for (auto it : polynomial_) {
        polynomial[it.first] = std::conj(it.second);
    }
    return TrigPolynomial(polynomial);
}
    
TrigPolynomial sieved(double cutoff=1.0E-12) const
{
    std::map<TrigMonomial, std::complex<double>> polynomial;
    for (auto it : polynomial_) {
        if (std::abs(it.second) > cutoff) {
            polynomial[it.first] = it.second;
        }
    }
    return TrigPolynomial(polynomial);
}
    
static
bool equivalent_keys(const TrigPolynomial& a, const TrigPolynomial& b)
{
    if (a.polynomial().size() != b.polynomial().size()) return false; 

    std::map<TrigMonomial, std::complex<double>> triga = a.polynomial();
    std::map<TrigMonomial, std::complex<double>> trigb = b.polynomial();
    for (auto itera = triga.begin(), iterb = trigb.begin(); itera != triga.end(); ++itera, ++iterb) {
        if (itera->first != iterb->first) return false;
    }
    return true;
}
    
static
bool equivalent_values(const TrigPolynomial& a, const TrigPolynomial& b, double cutoff=1.0E-12)
{
    if (!equivalent_keys(a, b)) throw std::runtime_error("Keys must be equivalent");

    std::map<TrigMonomial, std::complex<double>> triga = a.polynomial();
    std::map<TrigMonomial, std::complex<double>> trigb = b.polynomial();
    for (auto itera = triga.begin(), iterb = trigb.begin(); itera != triga.end(); ++itera, ++iterb) {
        if (std::abs(itera->second - iterb->second) > cutoff) return false;
    }
    return true;
}

static
bool equivalent(const TrigPolynomial& a, const TrigPolynomial& b, double cutoff=1.0E-12)
{
    if (!equivalent_keys(a, b)) return false;
    return equivalent_values(a, b);
}

static
TrigPolynomial cos(char symbol, int order=1)
{
    return TrigPolynomial({
        {TrigMonomial({{symbol, +order}}), +0.5},
        {TrigMonomial({{symbol, -order}}), +0.5}
        });
}

static
TrigPolynomial sin(char symbol, int order=1)
{
    std::complex<double> I(0.0, 1.0);
    return TrigPolynomial({
        {TrigMonomial({{symbol, +order}}), +0.5 * I},
        {TrigMonomial({{symbol, -order}}), -0.5 * I}
        });
}

private:

std::map<TrigMonomial, std::complex<double>> polynomial_;

};

} // namespace autogate
