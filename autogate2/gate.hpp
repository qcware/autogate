#pragma once

#include <vector>
#include <cstddef>
#include "trig_tensor.hpp"

namespace autogate { 

class Gate {

public:

Gate(
    uint32_t nqubit,
    const TrigTensor& matrix,
    const std::vector<std::string>& ascii_symbols) :
    nqubit_(nqubit),
    matrix_(matrix),
    ascii_symbols_(ascii_symbols)
{
    if (matrix_.shape() != { (1ULL<<nqubit_), (1ULL<<nqubit_) }) throw std::runtime_error("matrix is not shape (2**nqubit,)*2");
}

uint32_t nqubit() const { return nqubit_; }
const TrigTensor& matrix() const { return matrix_; }
const std::vector<std::string>& ascii_symbols() const { return ascii_symbols_; }

private: 

uint32_t nqubit_;
TrigTensor matrix_;
std::vector<std::string> ascii_symbols_;

};

} // namespace autogate
