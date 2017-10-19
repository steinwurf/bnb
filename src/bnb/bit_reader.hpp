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
private:

    /// The internal bitter reader type
    using reader_type = bitter::reader<Type, BitNumbering, Sizes...>;

public:

    /// The data type used as storage for this bit reader.
    using value_type = typename Type::type;

public:

    /// Constructs a bit reader
    /// @param value The value to serve as the data for this bit reader
    /// @param error The error code to set upon error
    bit_reader(value_type value, std::error_code& error) :
        m_reader(value),
        m_error(error)
    { }

    /// Reads a given value at a given index
    /// @param value The destination for the read value. Nothing will be read
    ///              if the error code has been set.
    /// @return A reference to this object wrapped in a validator object.
    ///         This allows for the next values to be read and for the current
    ///         value to be validated if needed.
    template<uint32_t Index, class ValueType>
    validator_wrapper<bit_reader, ValueType> read(ValueType& value)
    {
        if (m_error)
            return { *this, value, m_error };
        value = m_reader.template field<Index>().template read_as<ValueType>();
        return { *this, value, m_error };
    }

    /// Reads value at a given index without storage the value
    /// @return A reference to this object wrapped in a validator object.
    ///         This allows for the next values to be read and for the current
    ///         value to be validated if needed.
    template<uint32_t Index>
    auto read()
    {
        value_type value = 0;
        return read<Index, value_type>(value);
    }

private:

    reader_type m_reader;
    std::error_code& m_error;
};
}
