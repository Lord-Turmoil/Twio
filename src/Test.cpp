#include <twio/stream/BufferInputStream.h>
#include <twio/core/Reader.h>

#include <cstdio>

const char BUFFER[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main()
{
    const auto stream = twio::BufferInputStream::New(BUFFER);
    const auto reader = twio::Reader::New(stream);

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

    return 0;
}
