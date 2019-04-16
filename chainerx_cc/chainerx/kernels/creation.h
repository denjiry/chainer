#pragma once

#include <cstdint>

#include "chainerx/array.h"
#include "chainerx/op.h"
#include "chainerx/scalar.h"

namespace chainerx {

class ArangeOp : public Op {
public:
    static const char* name() { return "Arange"; }

    virtual void Call(Scalar start, Scalar step, const Array& out) = 0;
};

class CopyOp : public Op {
public:
    static const char* name() { return "Copy"; }

    // Copies the elements from one array to the other.
    //
    // The arrays must match in shape and dtype and need to reside on this device.
    virtual void Call(const Array& a, const Array& out) = 0;
};

class IdentityOp : public Op {
public:
    static const char* name() { return "Identity"; }

    // Creates the identity array.
    // out must be a square 2-dim array.
    virtual void Call(const Array& out) = 0;
};

class EyeOp : public Op {
public:
    static const char* name() { return "Eye"; }

    // Creates a 2-dimensional array with ones along the k-th diagonal and zeros elsewhere.
    // out must be a square 2-dim array.
    virtual void Call(int64_t k, const Array& out) = 0;
};

class DiagflatOp : public Op {
public:
    static const char* name() { return "Diagflat"; }

    virtual void Call(const Array& v, int64_t k, const Array& out) = 0;
};

class LinspaceOp : public Op {
public:
    static const char* name() { return "Linspace"; }

    // Creates an evenly spaced 1-d array.
    // `out.ndim()` must be 1 with at least 1 elements.
    virtual void Call(double start, double stop, const Array& out) = 0;
};

}  // namespace chainerx
