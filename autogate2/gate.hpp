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

};

} // namespace autogate
