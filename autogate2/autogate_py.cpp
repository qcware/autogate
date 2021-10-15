#include "trig.hpp"
#include "trig_tensor.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
using namespace py::literals;

namespace autogate {

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

}

} // namespace autogate
