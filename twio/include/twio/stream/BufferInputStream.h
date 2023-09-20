#pragma once

#ifndef _TWIO_BUFFER_INPUT_STREAM_H_
#define _TWIO_BUFFER_INPUT_STREAM_H_

#include <twio/Macros.h>
#include <memory>

TWIO_BEGIN


class BufferInputStream final : public IInputStream
{
public:
    // Initialize via a given buffer with a fixed size.
    // To ensure the buffer is valid and immutable, a copy of the original
    // buffer is made.
    explicit BufferInputStream(const char* buffer, size_t size);

    // Initialize via a given buffer, which must be a null-terminated string.
    explicit BufferInputStream(const char* buffer);

    // Copy is prohibited.
    BufferInputStream& operator=(const BufferInputStream&) = delete;
    BufferInputStream& operator=(BufferInputStream&& obj) = delete;
    BufferInputStream(const BufferInputStream&) = delete;
    BufferInputStream(BufferInputStream&& obj) = delete;

    ~BufferInputStream() override;

public:
    // Close the buffer input means free the memory.
    void Close() override;

    // Check if the buffer is ready for reading.
    bool IsReady() const override;

    size_t Read(char* buffer, size_t size) override;
    size_t Read(char* buffer) override;
    int Read() override;

private:
    // Make size to be a multiple of 8.
    size_t _Align8(size_t size);

private:
    std::unique_ptr<char[]> _buffer;
    size_t _size;
    size_t _next;
};


TWIO_END

#endif // _TWIO_BUFFER_INPUT_STREAM_H_
