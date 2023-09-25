# Tiny Wrapped Input/Output

> Copyright &copy; Tony's Studio 2018 - 2023

---

[![Build](https://github.com/Lord-Turmoil/Twio/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=main)](https://github.com/Lord-Turmoil/Twio/actions/workflows/cmake-multi-platform.yml)

## 1. Overview

Tiny Wrapped Input/Output, a.k.a. Twio, is a utility IO lib for C++. It provides a miniature encapsulation for read and write in console, buffer and file.

Twio separates input and output with stream and reader & writer. A stream represents the actual input/output stream, and reader or writer will then do IO using it.

To avoid memory leak, all raw pointers are wrapped with standard smart pointers.

## 2. Basic Usage

### 2.0 Quick Preview

Here is a quick demo of how to use Twio to wrap input and output.

```cpp
#include <Twio.h>

int main()
{
    const char BUFFER[] = "ABC\nDEFGHI\nJKLMNOPQR\nSTUVWXYZ\n";
    // Create an input stream from a null-terminated buffer.
    auto inputStream = twio::BufferInputStream::New(BUFFER);
    // Create an advanced reader from the input stream that record line and char info.
    auto reader = twio::Reader::New(inputStream);
    auto outputStream = twio::FileOutputStream::New("out.txt");
    auto writer = twio::Writer::New(outputStream)
        int ch;
    while ((ch = reader->Read()) != EOF)
    {
        if (ch == '\n') continue;
        writer->Write(ch);
    }
    
    return 0;
}
```

You don't need to under stand how stream works, or even how reader or writer is implemented. The only thing you need to care is the interface provided by `IReader` and `IWriter`, it is the interface that matters.

For all streams, reader and writers, a static `New` method is provided to quickly create an instance managed by `std::shared_ptr`.

### 2.1 Input Stream

There are two types of input streams, buffer input and file input.

#### 2.1.1 Buffer Input Stream

There are three ways to initialize a buffer input stream. The third one is a little tricky, and will be introduced later in ###.

```cpp
// Initialize via a given buffer with a fixed size.
BufferInputStream(const char* buffer, size_t size);
// Initialize via a null-terminated string.
BufferInputStream(const char* buffer);
// Initialize via a redirect request, which must be a buffer request for now.
BufferInputStream(RedirectRequestPtr request);
```

Notice that, buffer input stream will make a copy of the `buffer` you pass to it. So any modification to the buffer will not affect an already initialized input stream.

#### 2.1.2 File Input Stream

There are two ways to initialize a file input stream.

```cpp
// Initialize via an open file.
FileInputStream(FILE* fp, bool takeOver = true);
// Initialize via a file path, will close it automatically.
explicit FileInputStream(const char* path);
```

For the first method, it is recommended to leave `takeOver` true to let the stream close the file automatically. However, there is one exception, is that when you use it to read `stdin` (which is a file, too), you may need to set it false.

### 2.2 Output Stream

The same as input stream, there are two types of output stream.

#### 2.2.1 Buffer Output Stream

Buffer output stream will allocate a dynamic buffer on the heap, with a default block size of `1024`. When the buffer is full, it will grow by `DEFUALT_BLOCK_SIZE` bytes to satisfy the new size.

```cpp
BufferOutputStream(size_t size = DEFAULT_BLOCK_SIZE);
```

#### 2.2.2 File Output Stream

File output stream is almost the same as file input stream, just changed to write operation. Also, you may want to set `takeOver` false if you use `stdout` for console write or `stderr` for error logging.

```cpp
FileOutputStream(FILE* fp, bool takeOver = true);
FileOutputStream(const char* path);
```

### 2.3 Reader

Twio provides two readers, the basic `IReader`, and `IAdvancedReader` with line and character number enabled.

```cpp
class IReader
{
public:
    // Read at most 'size' characters into buffer.
    virtual size_t Read(char* buffer, size_t size) = 0;
    // Read one character.
    virtual int Read() = 0;
    // Rewind reading by 1.
    virtual int Rewind() = 0;
    // Get the stream it holds.
    virtual IInputStreamPtr Stream() const = 0;
    // Close the reader manually.
    virtual void Close() = 0;
};

class IAdvancedReader : public IReader
{
public:    
    // Get current line number.
    virtual int Line() const = 0;
    // Get current character number.
    virtual int Char() const = 0;
};
```

You can rewind a reader to history position to read old data from the stream. Though the implementation is trivial, it is achieved by a buffer of size 1024. Or you can implement your own version. **It is the interface that matters!** 😁

Both of the two default implementations takes an input stream as constructor parameter.

```cpp
Reader(IInputStreamPtr stream);
AdvancedReader(IInputStreamPtr stream);
```

### 2.4 Writer

```cpp
class IWriter
{
public:
    // Write a buffer to output stream.
    virtual size_t Write(const char* buffer, size_t size) = 0;
    // Write a null-terminated string to output stream.
    virtual size_t Write(const char* buffer) = 0;
    // Write a character to output stream.
    virtual size_t Write(char ch) = 0;
    // Get the output stream it holds.
    virtual IOutputStreamPtr Stream() const = 0;
    // Close the writer manually.
    virtual void Close() = 0;
};
```

---

## 3. Advanced Usage

### 3.1 Printer

You can use a `IPrinter` to write an input stream into an output stream.

```cpp
class IPrinter
{
public:
    virtual void Print() = 0;
};
```

The default implementation of `IPrinter` takes a reader and a writer, and will write everything in the input stream to the output stream.

```cpp
Printer(IReaderPtr reader, IWriterPtr writer);
```

> Of course you can wire console input and output together, and enjoy the fun. 😋

### 3.2 Redirect

Redirect is the trickiest thing here. It is easy to wire input to output, but not easy to wire output to input. So I have to implement this redirect mechanism to achieve this.

Basically, you can redirect the result of a writer to a reader by the following code.

```cpp
auto stream = twio::BufferInputStream::New(writer->Stream()->Yield());
auto reader = twio::Reader::New(stream);
```

`writer->Stream()` will get the output stream that the writer holds, and then `Yield()` will make it give up the stream and create a `RedirectRequest`. Then, the input stream can accept this request, and make it an input buffer. Later on, it can be used as a normal input stream.

> The writer will be **unable** to write after giving up its stream using `Yield()`.

### 3.3 Unwrapper

Similar to redirect, you can also unwrap an output stream to a raw buffer. You should provide the `buffer`, and make sure it has sufficient space.

```cpp
void UnwrapStream(const IOutputStreamPtr& stream, char* buffer)
```

For example, you can take the output buffer of a writer to a raw buffer.

```cpp
twio::UnwrapStream(writer->Stream(), finalBuffer);
```

> The writer will also be invalid after unwrap!

---

## 4. Limitations

Generally speaking, Twio is convenient, but not that perfect. And it can only support redirecting between buffer stream for now. 😞

It seems there will be a little memory leak when using file stream, but this will only occur once on the first use.
