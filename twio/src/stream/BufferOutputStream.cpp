#include <twio/stream/BufferOutputStream.h>

TWIO_BEGIN

const size_t BufferOutputStream::DEFAULT_BUFFER_SIZE = 1024;

BufferOutputStream::BufferOutputStream(size_t size)
{
    _size = TWIO_ALIGN(size, DEFAULT_BUFFER_SIZE);
    _buffer = std::make_unique<char[]>(_size);
    _next = 0;
}

BufferOutputStream::~BufferOutputStream()
{
    Close();
}

void BufferOutputStream::Close()
{
    if (_buffer)
    {
        _buffer.reset();
        _size = 0;
        _next = 0;
    }
}

bool BufferOutputStream::IsReady() const
{
    return _buffer != nullptr;
}

size_t BufferOutputStream::Write(const char* buffer, size_t size)
{
    TWIO_ASSERT(!IsReady());

    const size_t newSize = _next + size;
    _EnsureBufferSize(newSize);

    memcpy(_buffer.get() + _next, buffer, size);
    _next += size;

    return size;
}

size_t BufferOutputStream::Write(char ch)
{
    TWIO_ASSERT(!IsReady());

    _EnsureBufferSize(_next + 1);
    _buffer[_next++] = ch;

    return 1;
}

void BufferOutputStream::_EnsureBufferSize(size_t size)
{
    if (size > _size)
    {
        const size_t newSize = TWIO_ALIGN(size, DEFAULT_BUFFER_SIZE);
        auto newBuffer = std::make_unique<char[]>(newSize);

        memcpy(newBuffer.get(), _buffer.get(), _size);

        _buffer = std::move(newBuffer);
        _size = newSize;
    }
}


TWIO_END
