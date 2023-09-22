#include <twio/core/AdvancedReader.h>

TWIO_BEGIN

AdvancedReader::AdvancedReader(IInputStreamPtr stream)
    : _stream(std::move(stream)), _lineNo(1), _charNo(0)
{
}

AdvancedReader::~AdvancedReader() = default;

std::shared_ptr<AdvancedReader> AdvancedReader::New(const IInputStreamPtr& stream)
{
    return std::make_shared<AdvancedReader>(stream);
}

size_t AdvancedReader::Read(char* buffer, size_t size)
{
    TWIO_ASSERT(buffer != nullptr);

    const size_t bufferRead = 0;
    char ch;
    while (bufferRead < size && _HasNext())
    {
        ch = _Get();
        *(buffer++) = ch;
        _MoveForward(ch);
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
    for (size_t i = 0; i < streamRead; i++)
    {
        ch = buffer[i];
        _Push(ch);
        _MoveForward(ch);
    }

    return bufferRead + streamRead;
}

int AdvancedReader::Read()
{
    if (_HasNext())
    {
        return _Get();
    }

    const int ch = _stream->Read();
    if (ch != EOF)
    {
        _Push(ch);
        _MoveForward(ch);
    }

    return ch;
}

int AdvancedReader::Rewind()
{
    const int ch = _Pop();
    _MoveBackward(ch);
    return ch;
}

int AdvancedReader::Line() const
{
    return _lineNo;
}

int AdvancedReader::Char() const
{
    return _charNo;
}

IInputStreamPtr AdvancedReader::Stream() const
{
    return _stream;
}

// Currently will ignore '\r'.
void AdvancedReader::_MoveForward(char ch)
{
    if (ch == '\n')
    {
        _lineNo++;
        _lastChar.push(_charNo);
        _charNo = 0;
    }
    else if (ch != '\r')
    {
        _charNo++;
    }
}

void AdvancedReader::_MoveBackward(char ch)
{
    if (ch == '\n')
    {
        _lineNo--;
        _charNo = _lastChar.top() + 1;
        _lastChar.pop();
    }
    else if (ch != '\r')
    {
        _charNo--;
    }
}

TWIO_END
