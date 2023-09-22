#include <twio/stream/BufferInputStream.h>
#include <twio/core/Reader.h>

#include <cstdio>

#include "twio/core/Writer.h"
#include "twio/stream/FileOutputStream.h"
#include "twio/utils/Printer.h"

const char BUFFER[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main()
{
    const auto inputStream = twio::BufferInputStream::New(BUFFER);
    const auto reader = twio::Reader::New(inputStream);

    int ch;
    while ((ch = reader->Read()) != EOF)
    {
        putchar(ch);
    }
    putchar('\n');
    while ((ch = reader->Rewind()) > 0)
    {
        putchar(ch);
    }
    putchar('\n');

    const auto outputStream = twio::FileOutputStream::New("test.txt");
    const auto writer = twio::Writer::New(outputStream);

    const auto printer = twio::Printer::New(reader, writer);

    printer->Print();

    return 0;
}
