#pragma once

#include "ibit.hh"

#include <cstddef>

template<typename T>
class iRegister
{
public:
    virtual ~iRegister() = 0;

    virtual iBit& operator[](std::size_t n) = 0;

    virtual T& get() = 0;
    virtual T read() = 0;
    virtual void write(T value) = 0;
};
