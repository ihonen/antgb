#pragma once

#include "bit.hh"
#include "emulator/interfaces/iregister.hh"
#include "types.hh"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

template<typename T, T read_mask_ = std::numeric_limits<T>::max(), T write_mask_ = std::numeric_limits<T>::max()>
class RegisterBase : public iRegister<T>
{
public:

    using value_t = T;
    using mask_t = T;
    using width_t = std::size_t;

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

    virtual Bit<T>& operator [](std::size_t pos) override
    {

    }

    virtual value_t& get() override
    {
        return bits_;
    }

    virtual value_t read() override
    {
        return bits_ & read_mask();
    }

    void write(value_t value)
    {
        bits_ = value & write_mask();
    }

    /*
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
    */

protected:
    T default_buffer_;
    T* external_buffer_;

    T& bits_;

    std::array<Bit<T>, width()> bit_objects_;
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
