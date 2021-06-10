#pragma once

#include "emulator/interfaces/ibit.hh"

#include <cstddef>

template<typename T>
class Bit : public iBit
{
protected:
    T& register_;
    std::size_t pos_;
    bool is_readable_;
    bool is_writable_;
public:
    inline Bit(T& register_, std::size_t pos, bool is_readable, bool is_writable)
        : register_(register_)
        , pos_(pos)
        , is_readable_(is_readable)
        , is_writable_(is_writable)
    {
    }

    virtual inline ~Bit() override = default;

    virtual inline bool test() override
    {
        return ((register_ & (1 << pos_)) >> pos_) & is_readable_;
    }

    virtual inline void set() override
    {
        register_ |= (1 & static_cast<T>(is_writable_)) << pos_;
    }

    virtual inline void clear() override
    {
        register_ &= ~(1 & static_cast<T>(is_writable_)) << pos_;
    }

    virtual inline void update(bool condition) override
    {
        // Set if condition is true, clear if false.
        register_ |= (1 & static_cast<T>(is_writable_) & static_cast<T>(condition)) << pos_;
        register_ &= ~(1 & static_cast<T>(is_writable_) & static_cast<T>(condition) << pos_);
    }
};
