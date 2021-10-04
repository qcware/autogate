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

private:

std::vector<size_t> shape_;

size_t size_;

std::vector<TrigPolynomial> data_;

};

} // namespace autogate
