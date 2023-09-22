#include <twio/stream/BufferInputStream.h>
#include <cstring>
#include <utility>
#include <cstdio>   // EOF

TWIO_BEGIN


BufferInputStream::BufferInputStream(const char* buffer, size_t size)
    : _size(size), _next(0)
{
    TWIO_ASSERT(buffer);

    _buffer = std::make_unique<char[]>(TWIO_ALIGN_8(_size));

    memcpy(_buffer.get(), buffer, size);

}

BufferInputStream::BufferInputStream(const char* buffer)
    : _next(0)
{
    _size = strlen(buffer);
    _buffer = std::make_unique<char[]>(TWIO_ALIGN_8(_size));

    memcpy(_buffer.get(), buffer, _size);
}

std::shared_ptr<BufferInputStream> BufferInputStream::New(const char* buffer, size_t size)
{
    return std::make_shared<BufferInputStream>(buffer, size);
}

std::shared_ptr<BufferInputStream> BufferInputStream::New(const char* buffer)
{
    return std::make_shared<BufferInputStream>(buffer);
}

BufferInputStream::~BufferInputStream()
{
    Close();
}

void BufferInputStream::Close()
{
    // Set buffer to nullptr.
    _buffer.reset();
}

bool BufferInputStream::IsReady() const
{
    return _buffer != nullptr;
}

size_t BufferInputStream::Read(char* buffer, size_t size)
{
    TWIO_ASSERT(IsReady());

    // adjust size
    size = std::min(size, _size - _next);
    if (size == 0)
    {
        return 0;
    }

    memcpy(buffer, _buffer.get() + _next, size);
    buffer[size] = '\0';

    return size;
}

size_t BufferInputStream::Read(char* buffer)
{
    TWIO_ASSERT(IsReady());

    // buffer empty
    if (_next >= _size)
    {
        *buffer = '\0';
        return 0;
    }

    *buffer = _buffer[_next++];

    return 1;
}

int BufferInputStream::Read()
{
    TWIO_ASSERT(IsReady());

    // buffer empty
    if (_next >= _size)
    {
        return EOF;
    }

    return _buffer[_next++];
}

TWIO_END
