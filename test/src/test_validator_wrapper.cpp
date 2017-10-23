// Copyright (c) Steinwurf ApS 2017.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bnb/validator_wrapper.hpp>
#include <gtest/gtest.h>
namespace
{
struct dummy
{
    uint32_t test(uint32_t some_value)
    {
        return some_value;
    }
};
}
TEST(test_validator_wrapper, init)
{
    std::error_code error;
    uint32_t value = 42;
    dummy my_dummy;
    bnb::validator_wrapper<dummy, uint32_t> validator(my_dummy, value, error);

    EXPECT_EQ(5U, validator.test(5));
    validator.expect_eq(42);
    EXPECT_TRUE(!error);
}
