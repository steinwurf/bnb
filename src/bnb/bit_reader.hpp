// Copyright (c) Steinwurf ApS 2017.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <system_error>
#include <cassert>
#include <memory>
#include <bitter/reader.hpp>

namespace bnb
{

template<class DataType, uint32_t... Sizes>
class bit_reader
{
public:

    bit_reader(DataType value, std::error_code& error) :
        m_reader(std::make_unique<bitter::reader<DataType, Sizes...>>(value)),
        m_error(error)
    { }

    bit_reader(std::error_code& error) :
        m_reader(nullptr),
        m_error(error)
    { }

    template<uint32_t Index, class ValueType>
    bit_reader<DataType, Sizes...>& read(ValueType& value)
    {
        if (m_error)
            return *this;
        value = m_reader->template field<Index>().template read_as<ValueType>();
        return *this;
    }


private:

    std::unique_ptr<bitter::reader<DataType, Sizes...>> m_reader;
    std::error_code& m_error;
};
}
