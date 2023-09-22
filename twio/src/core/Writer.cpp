#include <twio/core/Writer.h>

TWIO_BEGIN

Writer::Writer(IOutputStreamPtr stream) : _stream(std::move(stream))
{
    TWIO_ASSERT(_stream != nullptr);
}

Writer::~Writer() = default;

std::shared_ptr<Writer> Writer::New(const IOutputStreamPtr& stream)
{
    return std::make_shared<Writer>(stream);
}

size_t Writer::Write(const char* buffer, size_t size)
{
    return _stream->Write(buffer, size);
}

size_t Writer::Write(const char* buffer)
{
    return _stream->Write(buffer);
}

size_t Writer::Write(char ch)
{
    return _stream->Write(ch);
}

IOutputStreamPtr Writer::Stream() const
{
    return _stream;
}

TWIO_END
