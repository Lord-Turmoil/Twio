#include <twio/stream/BufferInputStream.h>
#include <cstring>
#include <utility>

TWIO_BEGIN


BufferInputStream::BufferInputStream(const char* buffer, size_t size)
{
    TWIO_ASSERT(buffer);

    _size = TWIO_ALIGN(size);
    _buffer = std::make_unique<char[]>(_size);

    memcpy(_buffer.get(), buffer, size);

    _next = 0;
}

BufferInputStream::BufferInputStream(const char* buffer)
{
    size_t size = strlen(buffer);
    _size = _Align8(size);
    _buffer = std::make_unique<char[]>(_size);

    memcpy(_buffer.get(), buffer, size);
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
    return _fp != nullptr;
}

size_t BufferInputStream::Read(char* buffer, size_t size)
{
    TWIO_ASSERT(!IsReady());

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
    TWIO_ASSERT(!IsReady());

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
    TWIO_ASSERT(!IsReady());

    // buffer empty
    if (_next >= _size)
    {
        return EOF;
    }

    return _buffer[_next++];
}

TWIO_END
