import autogate2 as autogate

circuit = autogate.Circuit()
circuit.add_gate(time=0,qubits=(2,),gate=autogate.GateLibrary.I())
circuit.add_gate(time=2,qubits=(1,),gate=autogate.GateLibrary.I())
circuit.add_gate(time=3,qubits=(0,1),gate=autogate.GateLibrary.G('a'))
print(circuit)

op = circuit.matrix()
for item in op.data:
    print(item)

