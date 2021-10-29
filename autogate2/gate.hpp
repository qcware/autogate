#pragma once

#include <vector>
#include <cstddef>
#include "trig_tensor.hpp"

namespace autogate { 

class Gate {

public:

Gate() {}

Gate(
    uint32_t nqubit,
    const TrigTensor& matrix,
    const std::vector<std::string>& ascii_symbols) :
    nqubit_(nqubit),
    matrix_(matrix),
    ascii_symbols_(ascii_symbols)
{
    if (matrix_.shape() != std::vector<size_t> { (1ULL<<nqubit_), (1ULL<<nqubit_) }) throw std::runtime_error("matrix is not shape (2**nqubit,)*2");
    if (ascii_symbols_.size() != nqubit_) throw std::runtime_error("ascii_symbols.size() != nqubit");
}

uint32_t nqubit() const { return nqubit_; }
const TrigTensor& matrix() const { return matrix_; }
const std::vector<std::string>& ascii_symbols() const { return ascii_symbols_; }

private: 

uint32_t nqubit_;
TrigTensor matrix_;
std::vector<std::string> ascii_symbols_;

};

class GateLibrary {

public:

static
Gate I()
{
    uint32_t nqubit = 1;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    matrix.data()[0*2+0] = TrigPolynomial::one();
    matrix.data()[1*2+1] = TrigPolynomial::one();
    std::vector<std::string> ascii_symbols = {"I"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate X()
{
    uint32_t nqubit = 1;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    matrix.data()[0*2+1] = TrigPolynomial::one();
    matrix.data()[1*2+0] = TrigPolynomial::one();
    std::vector<std::string> ascii_symbols = {"X"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate Y()
{
    uint32_t nqubit = 1;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    std::complex<double> I (0.0, 1.0); 
    matrix.data()[0*2+1] = -I * TrigPolynomial::one();
    matrix.data()[1*2+0] = +I * TrigPolynomial::one();
    std::vector<std::string> ascii_symbols = {"Y"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate Z()
{
    uint32_t nqubit = 1;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    matrix.data()[0*2+0] = +TrigPolynomial::one();
    matrix.data()[1*2+1] = -TrigPolynomial::one();
    std::vector<std::string> ascii_symbols = {"Z"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate H()
{
    uint32_t nqubit = 1;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    matrix.data()[0*2+0] = +TrigPolynomial::one() / sqrt(2.0);
    matrix.data()[0*2+1] = +TrigPolynomial::one() / sqrt(2.0);
    matrix.data()[1*2+0] = +TrigPolynomial::one() / sqrt(2.0);
    matrix.data()[1*2+1] = -TrigPolynomial::one() / sqrt(2.0);
    std::vector<std::string> ascii_symbols = {"H"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate Ry(char symbol)
{
    uint32_t nqubit = 1;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    matrix.data()[0*2+0] = +TrigPolynomial::cos(symbol);
    matrix.data()[0*2+1] = -TrigPolynomial::sin(symbol);
    matrix.data()[1*2+0] = +TrigPolynomial::sin(symbol);
    matrix.data()[1*2+1] = +TrigPolynomial::cos(symbol);
    std::vector<std::string> ascii_symbols = {"Ry"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate oX()
{
    uint32_t nqubit = 2;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    matrix.data()[0*4+2] =  TrigPolynomial::one();
    matrix.data()[1*4+1] =  TrigPolynomial::one();
    matrix.data()[2*4+0] =  TrigPolynomial::one();
    matrix.data()[3*4+3] =  TrigPolynomial::one();
    std::vector<std::string> ascii_symbols = {"O","X"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate oY()
{
    uint32_t nqubit = 2;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    std::complex<double> I (0.0, 1.0); 
    matrix.data()[0*4+2] = -I * TrigPolynomial::one();
    matrix.data()[1*4+1] =      TrigPolynomial::one();
    matrix.data()[2*4+0] = +I * TrigPolynomial::one();
    matrix.data()[3*4+3] =      TrigPolynomial::one();
    std::vector<std::string> ascii_symbols = {"O","Y"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate oZ()
{
    uint32_t nqubit = 2;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    matrix.data()[0*4+0] = +TrigPolynomial::one();
    matrix.data()[1*4+1] = +TrigPolynomial::one();
    matrix.data()[2*4+2] = -TrigPolynomial::one();
    matrix.data()[3*4+3] = +TrigPolynomial::one();
    std::vector<std::string> ascii_symbols = {"O","Z"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate oH()
{
    uint32_t nqubit = 2;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    matrix.data()[0*4+0] = +TrigPolynomial::one() / sqrt(2.0);
    matrix.data()[0*4+2] = +TrigPolynomial::one() / sqrt(2.0);
    matrix.data()[1*4+1] = +TrigPolynomial::one();
    matrix.data()[2*4+0] = +TrigPolynomial::one() / sqrt(2.0);
    matrix.data()[2*4+2] = -TrigPolynomial::one() / sqrt(2.0);
    matrix.data()[3*4+3] =  TrigPolynomial::one();
    std::vector<std::string> ascii_symbols = {"O","H"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate cX()
{
    uint32_t nqubit = 2;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    matrix.data()[0*4+0] = TrigPolynomial::one();
    matrix.data()[1*4+3] = TrigPolynomial::one();
    matrix.data()[2*4+2] = TrigPolynomial::one();
    matrix.data()[3*4+1] = TrigPolynomial::one();
    std::vector<std::string> ascii_symbols = {"@","X"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate cY()
{
    uint32_t nqubit = 2;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    std::complex<double> I (0.0, 1.0); 
    matrix.data()[0*4+0] =      TrigPolynomial::one();
    matrix.data()[1*4+3] = -I * TrigPolynomial::one();
    matrix.data()[2*4+2] =      TrigPolynomial::one();
    matrix.data()[3*4+1] = +I * TrigPolynomial::one();
    std::vector<std::string> ascii_symbols = {"@","Y"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate cZ()
{
    uint32_t nqubit = 2;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    matrix.data()[0*4+0] = +TrigPolynomial::one();
    matrix.data()[1*4+1] = +TrigPolynomial::one();
    matrix.data()[2*4+2] = +TrigPolynomial::one();
    matrix.data()[3*4+3] = -TrigPolynomial::one();
    std::vector<std::string> ascii_symbols = {"@","Z"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate cH()
{
    uint32_t nqubit = 2;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    matrix.data()[0*4+0] = +TrigPolynomial::one();
    matrix.data()[1*4+1] = +TrigPolynomial::one() / sqrt(2.0);
    matrix.data()[1*4+3] = +TrigPolynomial::one() / sqrt(2.0);
    matrix.data()[2*4+2] = +TrigPolynomial::one();
    matrix.data()[3*4+1] = +TrigPolynomial::one() / sqrt(2.0);
    matrix.data()[3*4+3] = -TrigPolynomial::one() / sqrt(2.0);
    std::vector<std::string> ascii_symbols = {"@","H"};

    return Gate(nqubit, matrix, ascii_symbols);
}

static
Gate G(char symbol)
{
    uint32_t nqubit = 2;
    std::vector<size_t> dim = {(1ULL<<nqubit), (1ULL<<nqubit)};
    TrigTensor matrix(dim);
    matrix.data()[0*4+0] = +TrigPolynomial::one();
    matrix.data()[1*4+1] = +TrigPolynomial::cos(symbol);
    matrix.data()[1*4+2] = -TrigPolynomial::sin(symbol);
    matrix.data()[2*4+1] = +TrigPolynomial::sin(symbol);
    matrix.data()[2*4+2] = +TrigPolynomial::cos(symbol);
    matrix.data()[3*4+3] = +TrigPolynomial::one();
    std::vector<std::string> ascii_symbols = {"G0","G1"};

    return Gate(nqubit, matrix, ascii_symbols);
}

};

} // namespace autogate
