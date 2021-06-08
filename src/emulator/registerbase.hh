#pragma once

#include "types.hh"

#include <cassert>
#include <cstddef>
#include <cstdint>

template<typename T, T read_mask_, T write_mask_>
class RegisterBase
{
protected:
    T default_buffer_;
    T* external_buffer_;

    T& bits_;

public:

    using value_t = T;
    using mask_t = T;
    using width_t = std::size_t;

    RegisterBase()
        : external_buffer_(nullptr)
        , bits_(default_buffer_)
    {
    }

    RegisterBase(T& external_buffer)
        : external_buffer_(&external_buffer)
        , bits_(*external_buffer_)
    {
    }

    static constexpr width_t width()
    {
        return sizeof(T) * 8;
    }

    static constexpr mask_t read_mask()
    {
        return read_mask_;
    }

    static constexpr mask_t write_mask()
    {
        return write_mask_;
    }

    value_t& get()
    {
        return bits_;
    }

    value_t read()
    {
        return bits_ & read_mask();
    }

    void write(value_t value)
    {
        bits_ = value & write_mask();
    }

    template<width_t n>
    void set()
    {
        assert(n < width());
        bits_ |= write_mask() & (1 << n);
    }

    template<width_t n>
    void clear()
    {
        assert(n < width());
        bits_ &= ~write_mask() | ~(1 << n);
    }

    template<width_t n>
    void toggle()
    {
        assert(n < width());
        bits_ ^= write_mask() & (1 << n);
    }

    template<width_t n>
    bool test()
    {
        assert(n < width());
        return bool((bits_ & read_mask() & (1 << n)) >> n);
    }
};

template<typename T, addr_t address_, T read_mask_, T write_mask_>
class IoRegisterBase : public RegisterBase<T, read_mask_, write_mask_>
{
public:
    static constexpr addr_t address()
    {
        return address_;
    }
};
