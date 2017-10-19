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

    /// Constructs a validator wrapper.
    /// @param super A reference to the wrapped object.
    /// @param value The value to check
    /// @param error The error code to set if the validation failed
    validator_wrapper(Super& super, ValueType value, std::error_code& error) :
        Super(super),
        m_value(value),
        m_error(error)
    { }

    /// Checks if the given value is equal to the expected value.
    /// @param expected_value The expected value
    /// @returns A reference to this object, so that more expectations
    ///          or calls to the wrapped object can be made.
    validator_wrapper& expect_eq(ValueType expected_value)
    {
        if (m_error)
            return *this;

        if (m_value != expected_value)
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

    /// Checks if the given value is not equal to the expected value.
    /// @param expected_value The expected value
    /// @returns A reference to this object, so that more expectations
    ///          or calls to the wrapped object can be made.
    validator_wrapper& expect_ne(ValueType expected_value)
    {
        if (m_error)
            return *this;

        if (m_value == expected_value)
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

    /// Checks if the given value is less than to the expected value.
    /// @param expected_value The expected value
    /// @returns A reference to this object, so that more expectations
    ///          or calls to the wrapped object can be made.
    validator_wrapper& expect_lt(ValueType expected_value)
    {
        if (m_error)
            return *this;

        if (m_value >= expected_value)
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

    /// Checks if the given value is less than or equal to the expected value.
    /// @param expected_value The expected value
    /// @returns A reference to this object, so that more expectations
    ///          or calls to the wrapped object can be made.
    validator_wrapper& expect_le(ValueType expected_value)
    {
        if (m_error)
            return *this;

        if (m_value > expected_value)
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

    /// Checks if the given value is greater than to the expected value.
    /// @param expected_value The expected value
    /// @returns A reference to this object, so that more expectations
    ///          or calls to the wrapped object can be made.
    validator_wrapper& expect_gt(ValueType expected_value)
    {
        if (m_error)
            return *this;

        if (m_value <= expected_value)
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

    /// Checks if the given value is greater than or equal to the expected value.
    /// @param expected_value The expected value
    /// @returns A reference to this object, so that more expectations
    ///          or calls to the wrapped object can be made.
    validator_wrapper& expect_ge(ValueType expected_value)
    {
        if (m_error)
            return *this;

        if (m_value < expected_value)
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

    /// Checks if the given value can make the give expect function return true.
    /// @param expected_value The expected function
    /// @returns A reference to this object, so that more expectations
    ///          or calls to the wrapped object can be made.
    validator_wrapper& expect(std::function<bool(ValueType)> expect_func)
    {
        if (m_error)
            return *this;

        if (!expect_func(m_value))
            m_error = std::make_error_code(std::errc::result_out_of_range);

        return *this;
    }

private:

    ValueType m_value;
    std::error_code& m_error;
};
}
