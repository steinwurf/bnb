// Copyright (c) Steinwurf ApS 2017.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <system_error>
#include <cassert>
#include <functional>

namespace bnb
{

template<class Super, class ValueType>
class validator_wrapper : public Super
{
public:

    validator_wrapper(Super& super, std::error_code& error, ValueType value) :
        Super(super),
        m_error(error),
        m_value(value)
    { }

    validator_wrapper& expect_eq(ValueType expected_value)
    {
        if (m_error)
            return *this;

        if (m_value != expected_value)
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

    validator_wrapper& expect_ne(ValueType expected_value)
    {
        if (m_error)
            return *this;

        if (m_value == expected_value)
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

    validator_wrapper& expect_lt(ValueType expected_value)
    {
        if (m_error)
            return *this;

        if (m_value >= expected_value)
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

    validator_wrapper& expect_le(ValueType expected_value)
    {
        if (m_error)
            return *this;

        if (m_value > expected_value)
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

    validator_wrapper& expect_gt(ValueType expected_value)
    {
        if (m_error)
            return *this;

        if (m_value <= expected_value)
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

    validator_wrapper& expect_ge(ValueType expected_value)
    {
        if (m_error)
            return *this;

        if (m_value < expected_value)
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

    validator_wrapper& expect(std::function<bool(ValueType)> expect_func)
    {
        if (m_error)
            return *this;

        if (!expect_func(m_value))
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

private:

    std::error_code& m_error;
    ValueType m_value;
};
}
