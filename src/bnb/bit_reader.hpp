// Copyright (c) Steinwurf ApS 2017.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <system_error>
#include <bitter/reader.hpp>
#include <bitter/msb0.hpp>
#include <bitter/lsb0.hpp>

#include "validator_wrapper.hpp"

namespace bnb
{
template<class Type, class BitNumbering, uint32_t... Sizes>
class bit_reader
{
public:

    using value_type = typename Type::type;
    using reader_type = bitter::reader<Type, BitNumbering, Sizes...>;

public:

    bit_reader(value_type value, std::error_code& error) :
        m_reader(value),
        m_error(error)
    { }

    template<uint32_t Index, class ValueType>
    validator_wrapper<bit_reader, ValueType> read()
    {
        ValueType value = 0;
        if (m_error)
            return { *this, m_error, value };
        value = m_reader.template field<Index>().template read_as<ValueType>();
        return { *this, m_error, value };
    }

    template<uint32_t Index, class ValueType>
    validator_wrapper<bit_reader, ValueType> read(ValueType& value)
    {
        if (m_error)
            return { *this, m_error, value };
        value = m_reader.template field<Index>().template read_as<ValueType>();
        return { *this, m_error, value };
    }

private:

    reader_type m_reader;
    std::error_code& m_error;
};
}
