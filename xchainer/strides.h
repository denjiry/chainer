#pragma once

#include <array>
#include <cstdint>
#include <initializer_list>
#include <sstream>

#include <gsl/gsl>

#include "xchainer/constant.h"
#include "xchainer/dtype.h"
#include "xchainer/error.h"
#include "xchainer/shape.h"

namespace xchainer {

class Strides {
    using DimsType = std::array<int64_t, kMaxNdim>;

public:
    using const_iterator = DimsType::const_iterator;
    using const_reverse_iterator = DimsType::const_reverse_iterator;

    // Creates strides for contiguous array.
    Strides(const Shape& shape, Dtype dtype) : Strides{shape, GetElementSize(dtype)} {}
    Strides(const Shape& shape, int64_t element_size);

    // by gsl:span
    Strides(gsl::span<const int64_t> dims) : dims_(), ndim_(gsl::narrow_cast<int8_t>(dims.size())) {
        CheckNdim();
        std::copy(dims.begin(), dims.end(), const_cast<DimsType&>(dims_).begin());
    }

    // by initializer list
    Strides(std::initializer_list<int64_t> dims) : Strides(gsl::make_span(dims.begin(), dims.end())) {}

    // copy
    Strides(const Strides&) = default;
    Strides& operator=(const Strides&) = delete;

    // move
    Strides(Strides&&) = default;
    Strides& operator=(Strides&&) = delete;

    std::string ToString() const;

    size_t size() const noexcept { return static_cast<size_t>(ndim_); }

    int8_t ndim() const noexcept { return ndim_; }

    int64_t operator[](int8_t index) const {
        if (!(0 <= index && index < ndim_)) {
            throw DimensionError("index out of bounds");
        }
        return dims_[index];
    }

    // iterators
    const_iterator cbegin() const noexcept { return dims_.cbegin(); }
    const_iterator cend() const noexcept { return dims_.cbegin() + ndim_; }
    const_iterator begin() const noexcept { return cbegin(); }
    const_iterator end() const noexcept { return cend(); }

    // reverse iterators
    const_reverse_iterator crbegin() const noexcept { return dims_.crend() - ndim_; }
    const_reverse_iterator crend() const noexcept { return dims_.crend(); }
    const_reverse_iterator rbegin() const noexcept { return crbegin(); }
    const_reverse_iterator rend() const noexcept { return crend(); }

    // span
    gsl::span<const int64_t> span() const { return {&dims_[0], static_cast<size_t>(ndim_)}; }

private:
    void CheckNdim() const {
        if (ndim_ > kMaxNdim) {
            throw DimensionError("too many dimensions");
        }
    }

    DimsType dims_;
    int8_t ndim_;
};

inline bool operator==(const Strides& lhs, const Strides& rhs) { return lhs.span() == rhs.span(); }

inline bool operator!=(const Strides& lhs, const Strides& rhs) { return lhs.span() != rhs.span(); }

std::ostream& operator<<(std::ostream&, const Strides&);

void CheckEqual(const Strides& lhs, const Strides& rhs);

}  // namespace xchainer
