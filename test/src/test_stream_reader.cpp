// Copyright (c) Steinwurf ApS 2017.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bnb/stream_reader.hpp>
#include <endian/big_endian.hpp>
#include <gtest/gtest.h>

TEST(test_stream_reader, init)
{
    std::vector<uint8_t> buffer(10);
    bnb::stream_reader<endian::big_endian> reader(buffer.data(), buffer.size());

    EXPECT_EQ(buffer.data(), reader.data());
    EXPECT_EQ(buffer.size(), reader.size());
}

TEST(test_stream_reader, read_bytes)
{
    uint8_t expected_value = 3;
    std::vector<uint8_t> buffer(10, expected_value);
    bnb::stream_reader<endian::big_endian> reader(buffer.data(), buffer.size());

    uint8_t byte0 = 0;
    uint8_t byte1 = 0;
    uint8_t byte2 = 0;
    uint8_t byte3 = 0;
    uint8_t byte4 = 0;
    uint8_t byte5 = 0;
    uint8_t byte6 = 0;
    uint8_t byte7 = 0;
    uint8_t byte8 = 0;
    uint8_t byte9 = 0;

    reader.read(byte0);
    reader.read(byte1);
    reader.read(byte2);
    reader.read(byte3);
    reader.read(byte4);
    reader.read(byte5);
    reader.read(byte6);
    reader.read(byte7);
    reader.read(byte8);
    reader.read(byte9);

    EXPECT_EQ(expected_value, byte0);
    EXPECT_EQ(expected_value, byte1);
    EXPECT_EQ(expected_value, byte2);
    EXPECT_EQ(expected_value, byte3);
    EXPECT_EQ(expected_value, byte4);
    EXPECT_EQ(expected_value, byte5);
    EXPECT_EQ(expected_value, byte6);
    EXPECT_EQ(expected_value, byte7);
    EXPECT_EQ(expected_value, byte8);
    EXPECT_EQ(expected_value, byte9);

    // read out of bounds
    uint8_t initial_value = 88;
    uint8_t byte10 = initial_value;
    reader.read(byte10);

    ASSERT_TRUE(bool(reader.error()));
    EXPECT_EQ(initial_value, byte10);
}

TEST(test_stream_reader, read_bits)
{
    std::vector<uint8_t> buffer = { 0x41 };
    bnb::stream_reader<endian::big_endian> reader(buffer.data(), buffer.size());

    bool first_field = false;
    uint8_t second_field = false;
    reader.read_bits<uint8_t, 1, 7>()
        .read<0>(first_field)
        .read<1>(second_field);

    EXPECT_EQ(true, first_field);
    EXPECT_EQ(32U, second_field);
}