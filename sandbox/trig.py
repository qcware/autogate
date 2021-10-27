import autogate2 as autogate

gate = autogate.GateLibrary.I()
for item in gate.matrix.data:
    print(item)

circuit = autogate.Circuit()
circuit.add_gate(time=0,qubits=(0,),gate=autogate.GateLibrary.I())
circuit.add_gate(time=1,qubits=(1,),gate=autogate.GateLibrary.I())
print(circuit)

