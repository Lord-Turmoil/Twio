#include <twio/core/Reader.h>

TWIO_BEGIN


Reader::Reader(IInputStreamPtr stream) : _stream(std::move(stream))
{
    TWIO_ASSERT(_stream != nullptr);
}

std::shared_ptr<Reader> Reader::New(const IInputStreamPtr& stream)
{
    return std::make_shared<Reader>(stream);
}

Reader::~Reader() = default;

size_t Reader::Read(char* buffer, size_t size)
{
    TWIO_ASSERT(buffer != nullptr);

    const size_t bufferRead = 0;
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
    const size_t remain = size - bufferRead;
    const size_t streamRead = _stream->Read(buffer, remain);
    _Push(buffer, streamRead);

    return bufferRead + streamRead;
}

int Reader::Read()
{
    if (_HasNext())
    {
        return _Get();
    }

    const int ch = _stream->Read();
    if (ch != EOF)
    {
        _Push(ch);
    }

    return ch;
}

int Reader::Rewind()
{
    return _Pop();
}

TWIO_END
