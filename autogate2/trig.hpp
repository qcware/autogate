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

TrigPolynomial operator*(const std::complex<double>& scal) const 
{ 
    std::map<TrigMonomial, std::complex<double>> polynomial;
    for (auto const& it : polynomial_) {
        const TrigMonomial& monomial = it.first;
        const std::complex<double>& coefficient = it.second;
        polynomial[monomial] = coefficient * scal;
    }
    return TrigPolynomial(polynomial);
}

friend TrigPolynomial operator*(const std::complex<double>& scal, const TrigPolynomial& poly)
{
    return poly * scal;
}

TrigPolynomial operator/(const std::complex<double>& scal) const 
{ 
    std::map<TrigMonomial, std::complex<double>> polynomial;
    for (auto const& it : polynomial_) {
        const TrigMonomial& monomial = it.first;
        const std::complex<double>& coefficient = it.second;
        polynomial[monomial] = coefficient / scal;
    }
    return TrigPolynomial(polynomial);
}

TrigPolynomial operator+(const std::complex<double>& scal) const
{
    std::map<TrigMonomial, std::complex<double>> polynomial = polynomial_;
    polynomial[TrigMonomial::one()] += scal;
    return TrigPolynomial(polynomial);
}

friend TrigPolynomial operator+(const std::complex<double>& scal, const TrigPolynomial& poly) 
{
    return poly + scal;
}

friend TrigPolynomial operator-(const std::complex<double>& scal, const TrigPolynomial& poly) 
{
    return -poly + scal;
}

friend TrigPolynomial operator-(const TrigPolynomial& poly, const std::complex<double>& scal) 
{
    return poly + (-scal);
}
    
// TODO: Make this math library look exactly like TrigTensor
    
TrigPolynomial conj() const;
    
static
bool equivalent_keys(const TrigPolynomial& a, const TrigPolynomial& b);
    
// Throw if not equivalent_keys
static
bool equivalent_values(const TrigPolynomial& a, const TrigPolynomial& b, double cutoff=1.0E-12);
    
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
