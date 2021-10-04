import autogate
import sympy

def test1():

    t = sympy.Symbol('t', real=True)
    
    circuit = autogate.Circuit()
    # circuit.add_gate(gate=autogate.GateLibrary.H(), time=0, qubits=(0,))
    # circuit.add_gate(gate=autogate.GateLibrary.H(), time=0, qubits=(1,))
    # circuit.add_gate(gate=autogate.GateLibrary.cZ(), time=1, qubits=(0, 1))
    circuit.add_gate(gate=autogate.GateLibrary.Ry(t=+t/2), time=2, qubits=(0,))
    circuit.add_gate(gate=autogate.GateLibrary.Ry(t=-t/2), time=2, qubits=(1,))
    # circuit.add_gate(gate=autogate.GateLibrary.cZ(), time=3, qubits=(0, 1))
    # circuit.add_gate(gate=autogate.GateLibrary.H(), time=4, qubits=(0,))
    # circuit.add_gate(gate=autogate.GateLibrary.H(), time=4, qubits=(1,))
    print(circuit)
    
    print(sympy.pretty(circuit.operator))

test1()
