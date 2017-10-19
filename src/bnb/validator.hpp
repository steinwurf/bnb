// Copyright (c) Steinwurf ApS 2017.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <system_error>
#include <cassert>

#include "validator_wrapper.hpp"

namespace bnb
{
namespace detail
{
/// Empty dummy class for providing a "pure" validator which doesn't take the
/// Super template.
class empty
{ };
}

template<class ValueType>
class validator : public validator_wrapper<detail::empty, ValueType>
{
public:
    validator(ValueType value, std::error_code& error) :
        validator_wrapper<detail::empty, ValueType>(*this, value, error)
    { }
};
}
