// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#include <twio.h>

const char BUFFER[] = "ABC\nDEFGHI\nJKLMNOPQR\nSTUVWXYZ\n";
char finalBuffer[32];

void memory_test();
void console_test();


int main()
{
    memory_test();
    console_test();

    /*
     * Test basic buffer reader.
     */

    // Create an input stream from a null-terminated buffer.
    twio::IInputStreamPtr inputStream = twio::BufferInputStream::New(BUFFER);
    // Create an advanced reader from the input stream that record line and char info.
    twio::IAdvancedReaderPtr advancedReader = twio::AdvancedReader::New(inputStream);

    int ch;
    printf("Forward reading\n");
    while (advancedReader->HasNext())
    {
        ch = advancedReader->Read();
        if (ch == '\n') continue;
        printf("%c - Ln: %d, Ch: %d\n", ch, advancedReader->Line(), advancedReader->Char());
    }
    printf("Backward reading\n");
    advancedReader->Rewind();
    printf("%c - Ln: %d, Ch: %d\n", advancedReader->Read(), advancedReader->Line(), advancedReader->Char());

    while ((ch = advancedReader->Rewind()) > 0)
    {
        if (ch == '\n') continue;
        printf("%c - Ln: %d, Ch: %d\n", ch, advancedReader->Line(), advancedReader->Char());
    }
    putchar('\n');


    /*
     * Test basic buffer writer.
     */

    // Create a buffer writer.
    std::shared_ptr<twio::BufferOutputStream> outputStream = twio::BufferOutputStream::New();
    std::shared_ptr<twio::Writer> writer = twio::Writer::New(outputStream);

    // Since the advanced reader is rewind to the beginning, we can wire it
    // to the writer using a printer.
    std::shared_ptr<twio::Printer> printer = twio::Printer::New(advancedReader, writer);

    // This will output content of the input stream to the output stream.
    printer->Print();


    /*
     * Writer to reader.
     * If you want to read the result of a writer, you can transfer the
     * output stream of the writer to a input stream.
     */

    // transfer the output stream of the writer to a input stream.
    inputStream = twio::BufferInputStream::New(writer->Stream()->Yield());
    // Create a reader from the transferred input stream.
    twio::IReaderPtr reader = twio::Reader::New(std::move(inputStream));

    // Notice that, after Yield, the writer will no longer be able to write.
    // writer->WriteFormat("This will cause an error!\n");

    /*
     * Test file input and output.
     */

    // WriteFormat things into a file.
    twio::IOutputStreamPtr fileOutputStream = twio::FileOutputStream::New("test.txt");
    twio::IWriterPtr fileWriter = twio::Writer::New(std::move(fileOutputStream));

    printer = twio::Printer::New(reader, fileWriter);
    printer->Print();

    // Since the object doesn't reach its end of life, we need to close it manually.
    fileWriter->Close();

    // Create a reader from a file.
    twio::IReaderPtr fileReader = twio::Reader::New(twio::FileInputStream::New("test.txt"));
    writer = twio::Writer::New(twio::BufferOutputStream::New());
    printer = twio::Printer::New(fileReader, writer);

    printer->Print();

    UnwrapStream(writer->Stream(), finalBuffer);
    // Notice that, after unwrap, the writer will also be unable to write.
    // writer->WriteFormat("This will cause an error!\n");

    assert(strcmp(finalBuffer, BUFFER) == 0);

    printf("Congratulations! Everything is OK!\n");

    return 0;
}


void memory_test()
{
    // Normal use
    {
        auto reader = twio::Reader::New(twio::FileInputStream::New("test.txt"));
        auto writer = twio::Writer::New(twio::FileOutputStream::New("out.txt"));
        auto printer = twio::Printer::New(reader, writer);

        printer->Print();
    }

    // Normal use
    {
        auto reader = twio::Reader::New(twio::BufferInputStream::New(BUFFER));
        auto writer = twio::Writer::New(twio::BufferOutputStream::New());
        auto printer = twio::Printer::New(reader, writer);

        printer->Print();
    }

    // Use after yield
    {
        auto reader = twio::Reader::New(twio::BufferInputStream::New(BUFFER));
        auto writer = twio::Writer::New(twio::BufferOutputStream::New());
        auto printer = twio::Printer::New(reader, writer);

        printer->Print();

        auto stream = twio::BufferInputStream::New(writer->Stream()->Yield());
        auto newReader = twio::Reader::New(stream);
        auto newWriter = twio::Writer::New(twio::FileOutputStream::New("test.txt"));
        auto newPrinter = twio::Printer::New(reader, writer);

        newPrinter->Print();
    }
}


void console_test()
{
    auto reader = twio::Reader::New(twio::FileInputStream::New(stdin, false));
    auto writer = twio::Writer::New(twio::FileOutputStream::New(stdout, false));

    auto printer = twio::Printer::New(reader, writer);

    printer->Print();
}
