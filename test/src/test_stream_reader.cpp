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
    std::error_code error;
    bnb::stream_reader<endian::big_endian> reader(
        buffer.data(), buffer.size(), error);

    ASSERT_FALSE((bool)error);
    EXPECT_EQ(buffer.data(), reader.data());
    EXPECT_EQ(buffer.size(), reader.size());

    EXPECT_EQ(buffer.data(), reader.remaining_data());
    EXPECT_EQ(buffer.size(), reader.remaining_size());

    EXPECT_EQ(0U, reader.position());
}

TEST(test_stream_reader, read_bytes)
{
    std::vector<uint8_t> buffer {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::error_code error;
    bnb::stream_reader<endian::big_endian> reader(
        buffer.data(), buffer.size(), error);

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

    reader.read<1>(byte0);
    reader.read<1>(byte1);
    reader.read<1>(byte2);
    reader.read<1>(byte3);
    reader.read<1>(byte4);
    reader.read<1>(byte5);
    reader.read<1>(byte6);
    reader.read<1>(byte7);
    reader.read<1>(byte8);
    reader.read<1>(byte9);

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
    reader.read<1>(byte10);
    EXPECT_TRUE(bool(error));
    reader.read<1>(byte11);
    EXPECT_TRUE(bool(error));
    reader.read<1>(byte12);
    EXPECT_TRUE(bool(error));
    reader.read<1>(byte13);
    EXPECT_TRUE(bool(error));

    EXPECT_EQ(initial_value, byte10);
    EXPECT_EQ(initial_value, byte11);
    EXPECT_EQ(initial_value, byte12);
    EXPECT_EQ(initial_value, byte13);
}

TEST(test_stream_reader, skip)
{
    std::vector<uint8_t> buffer = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::error_code error;
    bnb::stream_reader<endian::big_endian> reader(
        buffer.data(), buffer.size(), error);

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
    skipped.read<1>(byte0);
    skipped.read<1>(byte1);
    skipped.read<1>(byte2);
    skipped.read<1>(byte3);
    skipped.read<1>(byte4);
    reader.read<1>(byte5);
    reader.read<1>(byte6);
    reader.read<1>(byte7);
    reader.read<1>(byte8);
    reader.read<1>(byte9);

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

    EXPECT_TRUE((bool)error);
    EXPECT_TRUE((bool)reader.error());
    EXPECT_TRUE((bool)reader_with_error.error());

    // make sure we can still skip without crashing
    auto another_reader_with_error = reader_with_error.skip(1);
    EXPECT_TRUE((bool)error);
    EXPECT_TRUE((bool)another_reader_with_error.error());
}

TEST(test_stream_reader, seek)
{
    std::vector<uint8_t> buffer =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 255, 0, 0, 0, 0, 0, 0, 2
    };

    std::error_code error;
    bnb::stream_reader<endian::big_endian> reader(
        buffer.data(), buffer.size(), error);

    uint8_t byte5 = 0;
    uint8_t byte6 = 0;
    uint8_t byte7 = 0;
    uint8_t byte8 = 0;
    uint8_t byte9 = 0;
    int64_t byte10 = 0;

    reader.seek(10);
    reader.seek(5);
    reader.read<1>(byte5);
    reader.read<1>(byte6);
    reader.read<1>(byte7);
    reader.read<1>(byte8);
    reader.read<1>(byte9);
    reader.peek<8>().expect_eq(-72057594037927934);
    reader.peek<8>().expect_eq(18374686479671623682U);
    reader.read<8>(byte10).expect_eq(-72057594037927934);

    EXPECT_EQ(5U, byte5);
    EXPECT_EQ(6U, byte6);
    EXPECT_EQ(7U, byte7);
    EXPECT_EQ(8U, byte8);
    EXPECT_EQ(9U, byte9);
    EXPECT_EQ(-72057594037927934, byte10);

    EXPECT_TRUE(!error);

    reader.seek(reader.size() + 1);

    EXPECT_FALSE(!error);

    // Make sure we stay in error state even though we read back to a
    // valid point
    reader.seek(1);
    EXPECT_FALSE(!error);
}

TEST(test_stream_reader, read_bits)
{
    std::vector<uint8_t> buffer = { 0b10010010 };
    std::error_code error;
    bnb::stream_reader<endian::big_endian> reader(
        buffer.data(), buffer.size(), error);

    bool first_field = false;
    uint8_t third_field = 0;

    reader.read_bits<bitter::u8, bitter::msb0, 1, 5, 2>()
    .get<0>(first_field)
    .get<1>().expect_eq(4)
    .get<2>(third_field);

    EXPECT_TRUE(!error);
    EXPECT_TRUE(!reader.error());
    EXPECT_EQ(true, first_field);
    EXPECT_EQ(2U, third_field);

    uint8_t some_field = 42;

    // check that if we read too much data we will get an error
    reader.read_bits<bitter::u8, bitter::msb0, 8>() // force error
    .get<0>(some_field);
    EXPECT_TRUE((bool) error);
    EXPECT_EQ(42U, some_field);

    // Check that we can still read bits without crashing
    reader.read_bits<bitter::u8, bitter::msb0, 8>()
    .get<0>(some_field);

    EXPECT_TRUE((bool) error);
    EXPECT_EQ(42U, some_field);
}
