// Copyright (c) Steinwurf ApS 2017.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bnb/validator.hpp>
#include <gtest/gtest.h>

TEST(test_validator, api)
{
    std::error_code error;
    auto validator = bnb::validator<uint32_t>(42, error);
    validator.expect_eq(42);
    ASSERT_TRUE(!error);
    validator.expect_ne(43);
    ASSERT_TRUE(!error);
    validator.expect_lt(44);
    ASSERT_TRUE(!error);
    validator.expect_le(42);
    validator.expect_le(43);
    ASSERT_TRUE(!error);
    validator.expect_gt(41);
    ASSERT_TRUE(!error);
    validator.expect_ge(42);
    validator.expect_ge(41);
    ASSERT_TRUE(!error);
    validator.expect([](uint32_t value) { return value == 42; });
    ASSERT_TRUE(!error);
}

TEST(test_validator, expect_eq)
{
    std::error_code error;
    auto validator = bnb::validator<uint32_t>(42, error);
    validator.expect_eq(42);
    ASSERT_TRUE(!error);

    validator.expect_eq(43);
    ASSERT_FALSE(!error);
}

TEST(test_validator, expect_ne)
{
    std::error_code error;
    auto validator = bnb::validator<uint32_t>(42, error);
    validator.expect_ne(43);
    ASSERT_TRUE(!error);

    validator.expect_ne(42);
    ASSERT_FALSE(!error);
}

TEST(test_validator, expect_lt)
{
    std::error_code error;
    auto validator = bnb::validator<uint32_t>(42, error);
    validator.expect_lt(43);
    ASSERT_TRUE(!error);

    validator.expect_lt(42);
    ASSERT_FALSE(!error);
}

TEST(test_validator, expect_le)
{
    std::error_code error;
    auto validator = bnb::validator<uint32_t>(42, error);
    validator.expect_le(43);
    ASSERT_TRUE(!error);
    validator.expect_le(42);
    ASSERT_TRUE(!error);

    validator.expect_le(41);
    ASSERT_FALSE(!error);
}

TEST(test_validator, expect_gt)
{
    std::error_code error;
    auto validator = bnb::validator<uint32_t>(42, error);
    validator.expect_gt(41);
    ASSERT_TRUE(!error);

    validator.expect_gt(42);
    ASSERT_FALSE(!error);
}

TEST(test_validator, expect_ge)
{
    std::error_code error;
    auto validator = bnb::validator<uint32_t>(42, error);
    validator.expect_ge(41);
    ASSERT_TRUE(!error);
    validator.expect_ge(42);
    ASSERT_TRUE(!error);

    validator.expect_ge(43);
    ASSERT_FALSE(!error);
}

TEST(test_validator, expect)
{
    std::error_code error;
    auto validator = bnb::validator<uint32_t>(42, error);
    validator.expect([](uint32_t value) {
        EXPECT_EQ(42U, value);
        return true;
    });
    ASSERT_TRUE(!error);

    validator.expect([](uint32_t value) {
        EXPECT_EQ(42U, value);
        return false;
    });
    ASSERT_FALSE(!error);
}