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

    ASSERT_FALSE((bool)reader.error());
    EXPECT_EQ(buffer.data(), reader.data());
    EXPECT_EQ(buffer.size(), reader.size());

    EXPECT_EQ(buffer.data(), reader.remaining_data());
    EXPECT_EQ(buffer.size(), reader.remaining_size());

    EXPECT_EQ(0U, reader.position());
}

TEST(test_stream_reader, read_bytes)
{
    std::vector<uint8_t> buffer {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
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

    EXPECT_EQ(0U, byte0);
    EXPECT_EQ(1U, byte1);
    EXPECT_EQ(2U, byte2);
    EXPECT_EQ(3U, byte3);
    EXPECT_EQ(4U, byte4);
    EXPECT_EQ(5U, byte5);
    EXPECT_EQ(6U, byte6);
    EXPECT_EQ(7U, byte7);
    EXPECT_EQ(8U, byte8);
    EXPECT_EQ(9U, byte9);

    // read out of bounds
    uint8_t initial_value = 88;
    uint8_t byte10 = initial_value;
    uint8_t byte11 = initial_value;
    uint8_t byte12 = initial_value;
    uint8_t byte13 = initial_value;
    reader.read(byte10);
    EXPECT_TRUE(bool(reader.error()));
    reader.read(byte11);
    EXPECT_TRUE(bool(reader.error()));
    reader.read(byte12);
    EXPECT_TRUE(bool(reader.error()));
    reader.read(byte13);
    EXPECT_TRUE(bool(reader.error()));

    EXPECT_EQ(initial_value, byte10);
    EXPECT_EQ(initial_value, byte11);
    EXPECT_EQ(initial_value, byte12);
    EXPECT_EQ(initial_value, byte13);
}

TEST(test_stream_reader, skip)
{
    std::vector<uint8_t> buffer = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
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

    auto skipped = reader.skip(5);
    skipped.read(byte0);
    skipped.read(byte1);
    skipped.read(byte2);
    skipped.read(byte3);
    skipped.read(byte4);
    reader.read(byte5);
    reader.read(byte6);
    reader.read(byte7);
    reader.read(byte8);
    reader.read(byte9);

    EXPECT_EQ(0U, byte0);
    EXPECT_EQ(1U, byte1);
    EXPECT_EQ(2U, byte2);
    EXPECT_EQ(3U, byte3);
    EXPECT_EQ(4U, byte4);
    EXPECT_EQ(5U, byte5);
    EXPECT_EQ(6U, byte6);
    EXPECT_EQ(7U, byte7);
    EXPECT_EQ(8U, byte8);
    EXPECT_EQ(9U, byte9);

    auto reader_with_error = reader.skip(1);

    EXPECT_TRUE((bool)reader.error());
    EXPECT_TRUE((bool)reader_with_error.error());
}

TEST(test_stream_reader, seek)
{
    std::vector<uint8_t> buffer = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    bnb::stream_reader<endian::big_endian> reader(buffer.data(), buffer.size());

    uint8_t byte5 = 0;
    uint8_t byte6 = 0;
    uint8_t byte7 = 0;
    uint8_t byte8 = 0;
    uint8_t byte9 = 0;

    reader.seek(10);
    reader.seek(5);
    reader.read(byte5);
    reader.read(byte6);
    reader.read(byte7);
    reader.read(byte8);
    reader.read(byte9);

    EXPECT_EQ(5U, byte5);
    EXPECT_EQ(6U, byte6);
    EXPECT_EQ(7U, byte7);
    EXPECT_EQ(8U, byte8);
    EXPECT_EQ(9U, byte9);

    reader.seek(11);

    EXPECT_TRUE((bool)reader.error());
}

TEST(test_stream_reader, read_bits)
{
    std::vector<uint8_t> buffer = { 0b10000100 };
    bnb::stream_reader<endian::big_endian> reader(buffer.data(), buffer.size());

    bool first_field = false;
    uint8_t second_field = 0;
    bool third_field = true;

    reader.read_bits<uint8_t, bitter::msb0, 1, 6, 1>()
        .read<0>(first_field)
        .read<1>(second_field).expect_eq(2)
        .read<2>(third_field);

    EXPECT_TRUE(!reader.error());
    EXPECT_EQ(true, first_field);
    EXPECT_EQ(2U, second_field);
    EXPECT_EQ(false, third_field);
}
