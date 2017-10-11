// Copyright (c) Steinwurf ApS 2017.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <system_error>
#include <cassert>
#include <endian/stream_reader.hpp>
#include <endian/big_endian.hpp>
#include <endian/little_endian.hpp>

#include "bit_reader.hpp"
#include "validator.hpp"

namespace bnb
{
template<class Endianness>
class stream_reader
{
public:

    stream_reader(const uint8_t* data, uint64_t size) :
        m_stream(data, size)
    { }

    /// Reads from the stream and moves the read position.
    ///
    /// @param value reference to the value to be read
    template<class ValueType>
    validator<ValueType> read(ValueType& value)
    {
        if (m_error)
            return { m_error, value };

        if (sizeof(ValueType) > m_stream.remaining_size())
        {
            m_error = std::make_error_code(std::errc::result_out_of_range);
            return { m_error, value };
        }
        m_stream.read(value);
        return { m_error, value };
    }

    /// Reads raw bytes from the stream to fill a buffer represented by
    /// a mutable storage object.
    ///
    /// Note, that this function is provided only for convenience and
    /// it does not perform any endian conversions.
    ///
    /// @param data The data pointer to fill into
    /// @param size The number of bytes to fill.
    void read(uint8_t* data, uint64_t size)
    {
        if (m_error)
            return;

        if (size > m_stream.remaining_size())
        {
            m_error = std::make_error_code(std::errc::result_out_of_range);
            return;
        }

        m_stream.read(data, size);
        return;
    }

    template<class ValueType, class BitNumbering, uint32_t... Sizes>
    bit_reader<ValueType, BitNumbering, Sizes...> read_bits()
    {
        ValueType value = 0;

        if (m_error)
        {
            return bit_reader<ValueType, BitNumbering, Sizes...>(
                value, m_error);
        }

        if (sizeof(ValueType) > m_stream.remaining_size())
        {
            m_error = std::make_error_code(std::errc::result_out_of_range);
            return bit_reader<ValueType, BitNumbering, Sizes...>(
                value, m_error);
        }

        m_stream.read(value);
        return bit_reader<ValueType, BitNumbering, Sizes...>(value, m_error);
    }

    /// Changes the current read/write position in the stream. The
    /// position is absolute i.e. it is always relative to the
    /// beginning of the buffer which is position 0.
    ///
    /// @param new_position the new position
    void seek(uint64_t new_position)
    {
        if (m_error)
            return;

        if (new_position > m_stream.size())
        {
            m_error = std::make_error_code(std::errc::invalid_seek);
            return;
        }

        m_stream.seek(new_position);
    }

    /// Skips over a given number of bytes in the stream
    ///
    /// @param bytes_to_skip the bytes to skip
    stream_reader<Endianness> skip(uint64_t bytes_to_skip)
    {
        if (m_error)
            return *this;

        if (bytes_to_skip > m_stream.remaining_size())
        {
            m_error = std::make_error_code(std::errc::invalid_seek);
            return *this;
        }

        auto remaining_data = m_stream.remaining_data();
        m_stream.skip(bytes_to_skip);
        return stream_reader<Endianness>(remaining_data, bytes_to_skip);
    }

    /// A pointer to the stream's data at the current position.
    ///
    /// @return pointer to the stream's data at the current position.
    const uint8_t* remaining_data() const
    {
        assert(!m_error);
        return m_stream.remaining_data();
    }


    /// Gets the current read/write position in the stream
    ///
    /// @return the current position.
    uint64_t position() const
    {
        assert(!m_error);
        return m_stream.position();
    }

    /// The remaining number of bytes in the stream
    ///
    /// @return the remaining number of bytes.
    uint64_t remaining_size() const
    {
        assert(!m_error);
        return m_stream.remaining_size();
    }

    /// A pointer to the stream's data.
    ///
    /// @return pointer to the stream's data.
    const uint8_t* data() const
    {
        assert(!m_error);
        return m_stream.data();
    }

    /// Gets the size of the underlying buffer in bytes.
    ///
    /// @return the size of the buffer
    uint64_t size() const
    {
        assert(!m_error);
        return m_stream.size();
    }

    std::error_code error() const
    {
        return m_error;
    }

private:

    endian::stream_reader<Endianness> m_stream;
    std::error_code m_error;
};
}
