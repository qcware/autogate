import autogate

circuit = autogate.Circuit()
circuit.add_gate(gate=autogate.GateLibrary.I(), time=0, qubits=(0,))
circuit.add_gate(gate=autogate.GateLibrary.I(), time=0, qubits=(1,))
circuit.add_gate(gate=autogate.GateLibrary.I(), time=1, qubits=(0,))
print(circuit)
