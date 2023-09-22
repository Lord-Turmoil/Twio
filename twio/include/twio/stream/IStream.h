#pragma once

#ifndef _TWIO_STREAM_H_
#define _TWIO_STREAM_H_

#include <twio/Macros.h>

TWIO_BEGIN


class IStream
{
public:
    virtual ~IStream() = default;

    virtual void Close() = 0;
    virtual bool IsReady() const = 0;
};


class IInputStream : public IStream
{
public:
    ~IInputStream() override = default;

    // Read up to 'size' characters from the stream.
    virtual size_t Read(char* buffer, size_t size) = 0;

    // Read one character from the stream.
    virtual size_t Read(char* buffer) = 0;

    // Read one character from the stream.
    virtual int Read() = 0;
};


class IOutputStream : public IStream
{
public:
    ~IOutputStream() override {}

    // Write up to 'size' characters from the buffer to the stream.
    virtual size_t Write(const char* buffer, size_t size) = 0;

    // Write one character to the stream.
    virtual size_t Write(char ch) = 0;
};


TWIO_END

#endif // _TWIO_STREAM_H_
