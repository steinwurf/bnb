// Copyright (c) Steinwurf ApS 2017.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bnb/bit_reader.hpp>

#include <endian/types.hpp>

#include <gtest/gtest.h>

TEST(test_bit_reader, api)
{
    std::error_code error;
    endian::u24::type v = 0x00FFFFFF;
    bnb::bit_reader<endian::u24, bitter::msb0, 1, 2, 3, 9, 9> reader(v, error);

    uint8_t field1 = 0;
    uint8_t field2 = 0;
    uint16_t field3 = 0;
    uint16_t field4 = 0;

    reader.get<0>().expect_eq(true);
    ASSERT_FALSE((bool)error);
    reader.get<1>(field1).expect_eq(3U);
    ASSERT_FALSE((bool)error);
    reader.get<2>(field2);
    ASSERT_FALSE((bool)error);
    reader.get<3>(field3);
    ASSERT_FALSE((bool)error);
    reader.get<4>(field4);
    ASSERT_FALSE((bool)error);

    EXPECT_EQ(3U, field1);
    EXPECT_EQ(7U, field2);
    EXPECT_EQ(511U, field3);
    EXPECT_EQ(511U, field4);
}

TEST(test_bit_reader, force_error)
{
    std::error_code error;
    endian::u24::type v = 0x00FFFFFF;
    bnb::bit_reader<endian::u24, bitter::msb0, 1, 2, 3, 9, 9> reader(v, error);

    bool field0 = false;
    uint8_t field1 = 0;
    uint8_t field2 = 0;
    uint16_t field3 = 0;
    uint16_t field4 = 0;

    reader.get<0>(field0);
    ASSERT_FALSE((bool)error);
    reader.get<1>(field1).expect_ne(3U); // force error
    ASSERT_TRUE((bool)error);
    reader.get<2>(field2);
    ASSERT_TRUE((bool)error);
    reader.get<3>(field3);
    ASSERT_TRUE((bool)error);
    reader.get<4>(field4);
    ASSERT_TRUE((bool)error);

    // At this point everything went smooth
    EXPECT_EQ(true, field0);

    // Here we failed as field1 was indeed 3.
    EXPECT_EQ(3U, field1);
    EXPECT_EQ(0U, field2);
    EXPECT_EQ(0U, field3);
    EXPECT_EQ(0U, field4);
}
