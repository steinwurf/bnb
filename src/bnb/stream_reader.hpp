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

    /// Constructs a stream reader over a pre-allocated buffer.
    ///
    /// @param data The pointer to the data.
    /// @param size The size of the allocated data
    /// @param error A reference to the error code to set if an error happened
    stream_reader(const uint8_t* data, uint64_t size, std::error_code& error) :
        m_stream(data, size),
        m_error(error)
    { }

    /// Reads from the stream and moves the read position.
    ///
    /// @param value reference to the value to be read.
    template<uint8_t Bytes, class ValueType>
    validator<ValueType> read_bytes(ValueType& value)
    {
        if (m_error)
            return { value, m_error };

        if (Bytes > m_stream.remaining_size())
        {
            m_error = std::make_error_code(std::errc::result_out_of_range);
            return { value, m_error };
        }
        m_stream.template read_bytes<Bytes, ValueType>(value);
        return { value, m_error };
    }

    /// Reads from the stream and moves the read position.
    template<uint8_t Bytes>
    validator<uint64_t> read_bytes()
    {
        uint64_t value;
        return read_bytes<Bytes, uint64_t>(value);
    }

    /// Peeks in the stream without moving the read position.
    ///
    /// @param value reference to the value to be read.
    /// @param offset number of bytes to offset the peeking with
    template<uint8_t Bytes, class ValueType>
    validator<ValueType> peek_bytes(ValueType& value, uint64_t offset=0) const
    {
        if (m_error)
            return { value, m_error };

        if (Bytes > m_stream.remaining_size() - offset)
        {
            m_error = std::make_error_code(std::errc::result_out_of_range);
            return { value, m_error };
        }
        m_stream.template peek_bytes<Bytes, ValueType>(value, offset);
        return { value, m_error };
    }

    /// Peeks in the stream without moving the read position.
    /// @param offset number of bytes to offset the peeking with
    template<uint8_t Bytes>
    validator<uint64_t> peek_bytes(uint64_t offset=0) const
    {
        uint64_t value = 0;
        return peek_bytes<Bytes, uint64_t>(value, offset);
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

    /// Returns a Bit Reader covering a given number of bytes and
    /// moves the read position.
    /// @return A bit reader covering the number of bytes in the Type template.
    template<class Type, class BitNumbering, uint32_t... Sizes>
    bit_reader<Type, BitNumbering, Sizes...> read_bits()
    {
        using value_type = typename Type::type;
        value_type value = 0;

        if (m_error)
        {
            return bit_reader<Type, BitNumbering, Sizes...>(value, m_error);
        }

        if (Type::size > m_stream.remaining_size())
        {
            m_error = std::make_error_code(std::errc::result_out_of_range);
            return bit_reader<Type, BitNumbering, Sizes...>(value, m_error);
        }

        m_stream.template read_bytes<Type::size, value_type>(value);
        return bit_reader<Type, BitNumbering, Sizes...>(value, m_error);
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
        return stream_reader<Endianness>(
            remaining_data, bytes_to_skip, m_error);
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
        return m_stream.data();
    }

    /// Gets the size of the underlying buffer in bytes.
    ///
    /// @return the size of the buffer
    uint64_t size() const
    {
        return m_stream.size();
    }

    /// Returns the error code
    /// @return the error code
    std::error_code error() const
    {
        return m_error;
    }

private:

    endian::stream_reader<Endianness> m_stream;
    std::error_code& m_error;
};
}
