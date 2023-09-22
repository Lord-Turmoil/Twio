#include <twio/utils/Printer.h>

TWIO_BEGIN

Printer::Printer(IReaderPtr reader, IWriterPtr writer)
    : _reader(std::move(reader)), _writer(std::move(writer))
{
    TWIO_ASSERT(_reader != nullptr);
    TWIO_ASSERT(_writer != nullptr);
}

std::shared_ptr<Printer> Printer::New(IReaderPtr reader, IWriterPtr writer)
{
    return std::make_shared<Printer>(std::move(reader), std::move(writer));
}

void Printer::Print()
{
    int ch;
    while ((ch = _reader->Read()) != EOF)
    {
        _writer->Write(ch);
    }
}

TWIO_END
