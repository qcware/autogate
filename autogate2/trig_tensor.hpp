#pragma once

namespace autogate {

class TrigTensor {

public:

TrigTensor() {}

TrigTensor(const std::vector<size_t>& shape) :
    shape_(shape)
{
    size_ = 1;
    for (auto shape2: shape_) {
        size_ *= shape2;
    }
    data_.resize(size_);
}

const std::vector<size_t>& shape() const { return shape_; }
const size_t size() const { return size_; }

std::vector<TrigPolynomial>& data() { return data_; }
const std::vector<TrigPolynomial>& data() const { return data_; }
// TODO vector of strides
    
TrigTensor operator+() const
{
    TrigTensor tensor(shape_);
    std::vector<TrigPolynomial>& data = tensor.data();
    for (size_t index = 0; index < data.size(); index++) {
        data[index] = +data_[index];
    }
    return tensor;
}

TrigTensor operator-() const
{
    TrigTensor tensor(shape_);
    std::vector<TrigPolynomial>& data = tensor.data();
    for (size_t index = 0; index < data.size(); index++) {
        data[index] = -data_[index];
    }
    return tensor;
}

TrigTensor& operator+=(const TrigTensor& other)
{
    if (shape_ != other.shape()) throw std::runtime_error("Tensors are not the same shape");

    const std::vector<TrigPolynomial>& data = other.data();
    for (size_t index = 0; index < data.size(); index++) {
        data_[index] += data[index];
    }
    return *this;
}

TrigTensor& operator-=(const TrigTensor& other)
{
    if (shape_ != other.shape()) throw std::runtime_error("Tensors are not the same shape");

    const std::vector<TrigPolynomial>& data = other.data();
    for (size_t index = 0; index < data.size(); index++) {
        data_[index] -= data[index];
    }
    return *this;
}

friend TrigTensor operator+(const TrigTensor& a, const TrigTensor& b)
{
    TrigTensor tensor = a;
    tensor += b;
    return tensor;
}

friend TrigTensor operator-(const TrigTensor& a, const TrigTensor& b)
{
    TrigTensor tensor = a;
    tensor -= b;
    return tensor;
}

friend TrigTensor operator*(const TrigTensor& a, const TrigTensor& b)
{
    if (a.shape() != b.shape()) throw std::runtime_error("Tensors are not the same shape");

    TrigTensor tensor(a.shape());
    std::vector<TrigPolynomial>& data = tensor.data();
    const std::vector<TrigPolynomial>& adata = a.data();
    const std::vector<TrigPolynomial>& bdata = b.data();
    for (size_t index = 0; index < adata.size(); index++) {
       data[index] = adata[index] * bdata[index]; 
    }
    return tensor;
}

TrigTensor operator*(const std::complex<double>& scalar) const
{
    TrigTensor tensor(shape_);
    std::vector<TrigPolynomial>& data = tensor.data();
    for (size_t index = 0; index < data.size(); index++) {
        data[index] = data_[index] * scalar;
    }
    return tensor;
}

friend TrigTensor operator*(const std::complex<double>& scalar, const TrigTensor& tensor)
{
    return tensor * scalar;
}

TrigTensor operator/(const std::complex<double>& scalar) const
{
    TrigTensor tensor(shape_);
    std::vector<TrigPolynomial>& data = tensor.data();
    for (size_t index = 0; index < data.size(); index++) {
        data[index] = data_[index] / scalar;
    }
    return tensor;
}

TrigTensor operator+=(const std::complex<double>& scalar)
{
    TrigTensor tensor(shape_);
    std::vector<TrigPolynomial>& data = tensor.data();
    for (size_t index = 0; index < data.size(); index++) {
        data[index] += data_[index];
    }
    return tensor;
}

friend TrigTensor operator+(const TrigTensor& a, const std::complex<double>& scalar)
{
    TrigTensor tensor = a;
    tensor += scalar;
    return tensor;
}

friend TrigTensor operator+(const std::complex<double>& scalar, const TrigTensor& a)
{
    TrigTensor tensor = a;
    tensor += scalar;
    return tensor;
}

TrigTensor operator-=(const std::complex<double>& scalar)
{
    TrigTensor tensor(shape_);
    std::vector<TrigPolynomial>& data = tensor.data();
    for (size_t index = 0; index < data.size(); index++) {
        data[index] -= data_[index];
    }
    return tensor;
}

friend TrigTensor operator-(const std::complex<double>& scalar, const TrigTensor& tensor)
{
    return -tensor + scalar;
}

friend TrigTensor operator-(const TrigTensor& tensor, const std::complex<double>& scalar)
{
    return tensor + (-scalar);
}

static TrigTensor gemm(const TrigTensor& a, const TrigTensor& b)
{
    if (a.shape() != b.shape()) throw std::runtime_error("Tensors are not the same shape");

    TrigTensor tensor(a.shape());
    std::vector<TrigPolynomial>& data = tensor.data();
    const std::vector<TrigPolynomial>& adata = a.data();
    const std::vector<TrigPolynomial>& bdata = b.data();
    const size_t dim = tensor.shape()[0];
    for (size_t i = 0; i < dim; i++) {
        for (size_t j = 0; j < dim; j++) {
            for (size_t k = 0; k < dim; k++) {
                data[(i*dim) + j] += adata[(i*dim) + k] * bdata[j + (k*dim)]; 
            }
        }
    }
    return tensor;
}
 
TrigTensor& operator*=(const std::complex<double>& scalar);
TrigTensor& operator/=(const std::complex<double>& scalar);
    
TrigTensor conj() const;
TrigTensor T() const;
TrigTensor dagger() const;
    
private:

std::vector<size_t> shape_;

size_t size_;

std::vector<TrigPolynomial> data_;

};

} // namespace autogate
