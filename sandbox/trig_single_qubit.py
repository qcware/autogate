import autogate2 as autogate

print('----')
print('I()')
gate = autogate.GateLibrary.I()
for item in gate.matrix.data:
    print(item)

print('----')
print('X()')
gate = autogate.GateLibrary.X()
for item in gate.matrix.data:
    print(item)

print('----')
print('Y()')
gate = autogate.GateLibrary.Y()
for item in gate.matrix.data:
    print(item)

print('----')
print('Z()')
gate = autogate.GateLibrary.Z()
for item in gate.matrix.data:
    print(item)

print('----')
print('H()')
gate = autogate.GateLibrary.H()
for item in gate.matrix.data:
    print(item)

print('----')
print('Ry(\'a\')')
gate = autogate.GateLibrary.Ry('a')
for item in gate.matrix.data:
    print(item)

circuit = autogate.Circuit()
circuit.add_gate(time=0,qubits=(0,),gate=autogate.GateLibrary.I())
circuit.add_gate(time=1,qubits=(1,),gate=autogate.GateLibrary.X())
circuit.add_gate(time=2,qubits=(2,),gate=autogate.GateLibrary.Y())
circuit.add_gate(time=3,qubits=(3,),gate=autogate.GateLibrary.Z())
circuit.add_gate(time=4,qubits=(4,),gate=autogate.GateLibrary.H())
circuit.add_gate(time=5,qubits=(5,),gate=autogate.GateLibrary.Ry('a'))
print(circuit)

