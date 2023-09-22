#include <twio/core/Reader.h>

TWIO_BEGIN


Reader::Reader(IInputStreamPtr stream) : _stream(stream)
{
    TWIO_ASSERT(stream != nullptr);
}

size_t Reader::Read(char* buffer, size_t size)
{
    TWIO_ASSERT(buffer != nullptr);

    size_t bufferRead = 0;
    while (bufferRead < size && _HasNext())
    {
        *(buffer++) = _Get();
    }

    // All from remain.
    if (bufferRead == size)
    {
        *buffer = '\0';
        return size;
    }

    // Buffer is empty now.
    size_t remain = size - bufferRead;
    size_t streamRead = _stream->Read(buffer, remain);
    _Push(buffer, streamRead);

    return bufferRead + streamRead;
}

int Reader::Read()
{
    if (_HasNext())
    {
        return _Get();
    }

    int ch = _stream->Read();
    if (ch != EOF)
    {
        _Push(ch);
    }

    return ch;
}

int Reader::Rewind()
{
    int ch = _Pop();

    TWIO_ASSERT(ch >= 0);

    return ch;
}

TWIO_END
