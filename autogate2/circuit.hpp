#pragma once

namespace autogate { 

Circuit circuit;
circuit.add_gate(0, {3}, I());
circuit.add_gate(0, {3, 4}, CX());

typedef std::pair<size_t, std::vector<size_t>> circuit_key;

class Circuit {

public:

Circuit() {}

const std::map<circuit_key, Gate>& gates() const { return gate_; }

void add_gate(
    size_t time,
    const std::vector<size_t>& qubits,
    const Gate& gate)
{
    // TODO: Check that gate addition is valid

    times_.insert(time);
    for (auto qubit : qubits) {
        qubits_.insert(qubit);
        times_and_qubits_.insert(std::pair<size_t, size_t>(time, qubit));
    }

    gates_[std::pair<size_t, std::vector<size_t>>(time, qubits)] = gate;  
}

TrigTensor matrix() const; // TODO

private:

std::map<circuit_key, Gate> gates_;

std::set<size_t> qubits_;
std::set<size_t> times_;
std::set<std::pair<size_t, size_t>> times_and_qubits_;

};

} // namespace autogate
