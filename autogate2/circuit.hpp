#pragma once

#include "gate.hpp"
#include <set>

namespace autogate { 

typedef std::pair<size_t, std::vector<size_t>> circuit_key_t;

class Circuit {

public:

Circuit() {}

const std::map<circuit_key_t, Gate>& gates() const { return gates_; }
const std::set<size_t> qubits() const { return qubits_; }
const std::set<size_t> times() const { return times_; }
const std::set<std::pair<size_t, size_t>> times_and_qubits() const { return times_and_qubits_; }
ssize_t max_qubit() const { return qubits_.size() ? *qubits_.rbegin() : -1; }
ssize_t max_time() const { return times_.size() ? *times_.rbegin() : -1; }
size_t nqubit() const { return max_qubit() + 1; }
size_t ntime() const { return max_time() + 1; }

void add_gate(
    size_t time,
    const std::vector<size_t>& qubits,
    const Gate& gate)
{
    if (qubits.size() != gate.nqubit()) throw std::runtime_error("Number of specified qubits != number of gate qubits");
    std::set<size_t> qubitset(qubits.begin(), qubits.end());
    if (qubitset.size() != qubits.size()) throw std::runtime_error("Repeated qubit indices");

    for (auto qubit : qubits) {
        std::pair<size_t, size_t> qindex(time, qubit);
        if (times_and_qubits_.find(qindex) != times_and_qubits_.end()) {
            throw std::runtime_error("Time and qubit indices already occupied");
        }
    }

    times_.insert(time);
    for (auto qubit : qubits) {
        qubits_.insert(qubit);
        times_and_qubits_.insert(std::pair<size_t, size_t>(time, qubit));
    }

    gates_[std::pair<size_t, std::vector<size_t>>(time, qubits)] = gate; 
}

TrigTensor matrix() const
{
    uint32_t nqubit = qubits_.size();
    size_t dim = 1ULL<<nqubit;
    std::vector<size_t> shape = {dim, dim};
    TrigTensor mat(shape);
    for (size_t index = 0; index < dim; index++) {
        mat.data()[index*dim + index] = TrigPolynomial::one();
    }

    for (auto const& gate : gates_) {
        uint32_t qubits = gate.first.second.size();
        TrigTensor mat2(shape);

        TrigTensor gate_op = gate.second.matrix();
        size_t gate_dim = gate_op.shape()[0];

        std::vector<size_t> oubits;
        for (size_t index = 0; index < nqubit; index++) {
            if (std::find(gate.first.second.begin(), gate.first.second.end(), index) == gate.first.second.end()) {
                oubits.push_back(index);
            }
        }

        for (size_t k1 = 0; k1 < 1ULL<<oubits.size(); k1++ ) {
            size_t k2 = 0;
            for (size_t o1 = 0; o1 < oubits.size(); o1++) {
                size_t o2 = oubits[o1];
                k2 += ((k1 & (1 << o1)) >> o1) << o2;
            }
            for (size_t l1 = 0; l1 < 1ULL<<qubits; l1++) {
                size_t l2 = 0;
                for (size_t q1 = 0; q1 < qubits; q1++) {
                    size_t q2 = gate.first.second[q1];
                    l2 += ((l1 & (1 << q1)) >> q1) << q2;
                }
                for (size_t m1 = 0; m1 < 1ULL<<qubits; m1++) {
                    size_t m2 = 0;
                    for (size_t r1 = 0; r1 < qubits; r1++) {
                        size_t r2 = gate.first.second[r1];
                        m2 += ((m1 & (1 << r1)) >> r1) << r2;
                    }
                    mat2.data()[(k2 + l2) * dim + (k2 + m2)] = gate_op.data()[l1 * gate_dim + m1];
                }
            }
        }
        mat = TrigTensor::gemm(mat2,mat);
    }
    return mat;
}

private:

std::map<circuit_key_t, Gate> gates_;
std::set<size_t> qubits_;
std::set<size_t> times_;
std::set<std::pair<size_t, size_t>> times_and_qubits_;

};

} // namespace autogate
