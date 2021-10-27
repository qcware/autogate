#include "trig.hpp"
#include "trig_tensor.hpp"
#include "gate.hpp"
#include "circuit.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
using namespace py::literals;

namespace autogate {

py::dict py_circuit_gates(
  const Circuit& circuit)
{
  py::dict d;
  for (auto it : circuit.gates()) {
    const circuit_key_t& key = it.first;
    const Gate& gate = it.second;
    d[py::make_tuple(std::get<0>(key), py::tuple(py::cast(std::get<1>(key))))] = gate;
  }  
  return d;
}

PYBIND11_MODULE(autogate_plugin, m) {

py::class_<TrigMonomial>(m, "TrigMonomial")
.def(py::init<const std::vector<std::pair<char, int>>&>(), "variables"_a)
.def_property("variables", &TrigMonomial::variables, nullptr)
.def_static("one", &TrigMonomial::one)
.def("conj", &TrigMonomial::conj)
.def(py::self * py::self)
;

py::class_<TrigPolynomial>(m, "TrigPolynomial")
.def(py::init<const std::map<TrigMonomial, std::complex<double>>&>(), "polynomial"_a)
.def_property("polynomial", &TrigPolynomial::polynomial, nullptr)
.def_static("zero", &TrigPolynomial::zero)
.def_static("one", &TrigPolynomial::one)
.def(+py::self)
.def(-py::self)
.def(py::self += py::self)
.def(py::self -= py::self)
.def(py::self + py::self)
.def(py::self - py::self)
.def(py::self * py::self)
.def(py::self * std::complex<double>())
.def(std::complex<double>() * py::self)
.def(py::self / std::complex<double>())
.def(py::self += std::complex<double>())
.def(py::self + std::complex<double>())
.def(std::complex<double>() + py::self)
.def(py::self -= std::complex<double>())
.def(py::self - std::complex<double>())
.def(std::complex<double>() - py::self)
.def("conj", &TrigPolynomial::conj)
.def("sieved", &TrigPolynomial::sieved, "cutoff"_a=1.0E-12)
.def_static("equivalent_keys", &TrigPolynomial::equivalent_keys, "a"_a, "b"_a)
.def_static("equivalent_values", &TrigPolynomial::equivalent_values, "a"_a, "b"_a, "cutoff"_a=1.0E-12)
.def_static("equivalent", &TrigPolynomial::equivalent, "a"_a, "b"_a, "cutoff"_a=1.0E-12)
.def_static("cos", &TrigPolynomial::cos, "symbol"_a, "order"_a=1)
.def_static("sin", &TrigPolynomial::sin, "symbol"_a, "order"_a=1)
;

py::class_<TrigTensor>(m, "TrigTensor")
.def(py::init<const std::vector<size_t>&>(), "shape"_a)
.def_property("shape", &TrigTensor::shape, nullptr)
.def_property("size", &TrigTensor::size, nullptr)
.def_property("data", [](TrigTensor& a){return a.data();}, nullptr)
.def(+py::self)
.def(-py::self)
.def(py::self += py::self)
.def(py::self -= py::self)
.def(py::self + py::self)
.def(py::self - py::self)
.def(py::self * py::self)
.def(py::self * std::complex<double>())
.def(std::complex<double>() * py::self)
.def(py::self / std::complex<double>())
.def(py::self += std::complex<double>())
.def(py::self + std::complex<double>())
.def(std::complex<double>() + py::self)
.def(py::self -= std::complex<double>())
.def(py::self - std::complex<double>())
.def(std::complex<double>() - py::self)
;

py::class_<Gate>(m, "Gate")
.def(py::init<uint32_t, const TrigTensor&, const std::vector<std::string>&>(), "nqubit"_a, "matrix"_a, "ascii_symbols"_a)
.def_property("nqubit", &Gate::nqubit, nullptr)
.def_property("matrix", &Gate::matrix, nullptr)
.def_property("ascii_symbols", &Gate::ascii_symbols, nullptr)
;

py::class_<GateLibrary>(m, "GateLibrary")
.def_static("I", &GateLibrary::I)
;

py::class_<Circuit>(m, "Circuit")
.def(py::init<>())
.def_property("gates", py_circuit_gates, nullptr)
.def_property("qubits", &Circuit::qubits, nullptr)
.def_property("times", &Circuit::times, nullptr)
.def_property("times_and_qubits", &Circuit::times_and_qubits, nullptr)
.def_property("max_qubit", &Circuit::max_qubit, nullptr)
.def_property("max_time", &Circuit::max_time, nullptr)
.def_property("nqubit", &Circuit::nqubit, nullptr)
.def_property("ntime", &Circuit::ntime, nullptr)
.def("add_gate", &Circuit::add_gate, "time"_a, "qubits"_a, "gate"_a)
;

}

} // namespace autogate

