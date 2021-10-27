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
        std::pair<size_t, size_t> qindex(qubit,time);
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

TrigTensor matrix() const// ;  TODO
{
    const int nqubit = qubits_.size();
    const std::vector<size_t> dim = {1ULL<<nqubit, 1ULL<<nqubit};
    TrigTensor mat(dim);

    return mat;
}

private:

std::map<circuit_key_t, Gate> gates_;

std::set<size_t> qubits_;
std::set<size_t> times_;
std::set<std::pair<size_t, size_t>> times_and_qubits_;

};

} // namespace autogate
