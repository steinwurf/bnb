// Copyright (c) Steinwurf ApS 2017.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bnb/bit_reader.hpp>

#include <endian/types.hpp>

#include <gtest/gtest.h>

TEST(test_bit_reader, init)
{
    std::error_code error;
    endian::u24::type v = 0x00FFFFFF;
    bnb::bit_reader<endian::u24, bitter::msb0, 1, 2, 3, 9, 9> reader(v, error);

    bool field0 = false;
    uint8_t field1 = 0;
    uint8_t field2 = 0;
    uint16_t field3 = 0;
    uint16_t field4 = 0;

    reader.read<0>(field0);
    ASSERT_FALSE((bool)error);
    reader.read<1>(field1);
    ASSERT_FALSE((bool)error);
    reader.read<2>(field2);
    ASSERT_FALSE((bool)error);
    reader.read<3>(field3);
    ASSERT_FALSE((bool)error);
    reader.read<4>(field4);
    ASSERT_FALSE((bool)error);

    EXPECT_EQ(true, field0);
    EXPECT_EQ(3U, field1);
    EXPECT_EQ(7U, field2);
    EXPECT_EQ(511U, field3);
    EXPECT_EQ(511U, field4);
}
