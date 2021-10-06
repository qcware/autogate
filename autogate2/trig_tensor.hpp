#pragma once

namespace autogate {

class TrigTensor {

public:

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

std::vector<TrigPolynomial>& data() { return data_; }
// TODO expose size to user
// TODO vector of strides
    
// => Math Functions <= //
    
TrigTensor operator+() const;
TrigTensor operator-() const;
    
TrigTensor& operator+=(const TrigTensor& other);
TrigTensor& operator-=(const TrigTensor& other);
friend TrigTensor operator+(const TrigTensor& a, const TrigTensor& b);
friend TrigTensor operator-(const TrigTensor& a, const TrigTensor& b);

TrigTensor& operator+=(const std::complex<double>& scal);
TrigTensor& operator-=(const std::complex<double>& scal);
TrigTensor& operator*=(const std::complex<double>& scal);
TrigTensor& operator/=(const std::complex<double>& scal);
friend TrigTensor operator+(const std::complex<double>& scalar, const TrigTensor& tensor);
friend TrigTensor operator+(const TrigTensor& tensor, const std::complex<double>& scalar);
friend TrigTensor operator-(const std::complex<double>& scalar, const TrigTensor& tensor);
friend TrigTensor operator-(const TrigTensor& tensor, const std::complex<double>& scalar);
friend TrigTensor operator*(const std::complex<double>& scalar, const TrigTensor& tensor);
friend TrigTensor operator*(const TrigTensor& tensor, const std::complex<double>& scalar);
friend TrigTensor operator/(const TrigTensor& tensor, const std::complex<double>& scalar);
    
TrigTensor conj() const;
TrigTensor T() const;
TrigTensor dagger() const;
    
private:

std::vector<size_t> shape_;

size_t size_;

std::vector<TrigPolynomial> data_;

};

} // namespace autogate
