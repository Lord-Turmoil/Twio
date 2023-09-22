#include <twio/stream/BufferInputStream.h>
#include <twio/core/Reader.h>

#include <cstdio>

#include "twio/core/AdvancedReader.h"
#include "twio/core/Writer.h"
#include "twio/stream/BufferOutputStream.h"
#include "twio/stream/FileOutputStream.h"
#include "twio/utils/Printer.h"

const char BUFFER[] = "ABC\nDEFGHI\nJKLMNOPQR\nSTUVWXYZ\n";

int main()
{
    auto inputStream = twio::BufferInputStream::New(BUFFER);
    auto advancedReader = twio::AdvancedReader::New(inputStream);

    int ch;
    while ((ch = advancedReader->Read()) != EOF)
    {
        printf("%c - Ln: %d, Ch: %d\n", ch, advancedReader->Line(), advancedReader->Char());
    }
    putchar('\n');
    while ((ch = advancedReader->Rewind()) > 0)
    {
        printf("%c - Ln: %d, Ch: %d\n", ch, advancedReader->Line(), advancedReader->Char());
    }
    putchar('\n');

    const auto outputStream = twio::BufferOutputStream::New();
    const auto writer = twio::Writer::New(outputStream);
    const auto printer = twio::Printer::New(advancedReader, writer);

    printer->Print();

    printf("========================================\n");

    inputStream = twio::BufferInputStream::New(outputStream->Yield());
    advancedReader = twio::AdvancedReader::New(inputStream);
    while ((ch = advancedReader->Read()) != EOF)
    {
        printf("%c - Ln: %d, Ch: %d\n", ch, advancedReader->Line(), advancedReader->Char());
    }

    return 0;
}