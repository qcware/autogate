import autogate
import sympy

def test1():

    t = sympy.Symbol('t', real=True)
    
    circuit = autogate.Circuit()
    circuit.add_gate(gate=autogate.GateLibrary.H(), time=0, qubits=(0,))
    circuit.add_gate(gate=autogate.GateLibrary.H(), time=0, qubits=(1,))
    circuit.add_gate(gate=autogate.GateLibrary.cZ(), time=1, qubits=(0, 1))
    circuit.add_gate(gate=autogate.GateLibrary.Ry(t=+t/2), time=2, qubits=(0,))
    circuit.add_gate(gate=autogate.GateLibrary.Ry(t=-t/2), time=2, qubits=(1,))
    circuit.add_gate(gate=autogate.GateLibrary.cZ(), time=3, qubits=(0, 1))
    circuit.add_gate(gate=autogate.GateLibrary.H(), time=4, qubits=(0,))
    circuit.add_gate(gate=autogate.GateLibrary.H(), time=4, qubits=(1,))
    print(circuit)
    
    print(sympy.pretty(circuit.operator))

def test2():

    t = sympy.Symbol('t', real=True)

    circuit = autogate.Circuit()
    circuit.add_gate(gate=autogate.GateLibrary.oX(), time=0, qubits=(1, 0))
    circuit.add_gate(gate=autogate.GateLibrary.oX(), time=0, qubits=(2, 3))
    circuit.add_gate(gate=autogate.ControlledGate(controls=[True]*2, gate=autogate.GateLibrary.G(t=t)), time=1, qubits=(0, 3, 1, 2))
    circuit.add_gate(gate=autogate.GateLibrary.oX(), time=2, qubits=(1, 0))
    circuit.add_gate(gate=autogate.GateLibrary.oX(), time=2, qubits=(2, 3))
    print(circuit)
    
    print(sympy.pretty(circuit.operator))

test1()
test2()
