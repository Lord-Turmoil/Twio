#include <twio/stream/BufferInputStream.h>
#include <twio/core/Reader.h>

#include <cstdio>

const char BUFFER[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main()
{
    auto stream = twio::BufferInputStream::New(BUFFER);
    auto reader = twio::Reader(stream);

    int ch;
    while ((ch = reader.Read()) != EOF)
    {
        putchar(ch);
    }
    putchar('\n');
    while ((ch = reader.Rewind()) > 0)
    {
        putchar(ch);
    }

    return 0;
}
