#pragma once

#ifndef _TWIO_WRITER_H_
#define _TWIO_WRITER_H_

#include <twio/core/IWriter.h>
#include <twio/stream/IStream.h>

TWIO_BEGIN


class Writer : public IWriter
{
public:
    explicit Writer(IOutputStreamPtr stream);
    ~Writer() override;

    static std::shared_ptr<Writer> New(const IOutputStreamPtr& stream);

    size_t Write(const char* buffer, size_t size) override;
    size_t Write(const char* buffer) override;
    size_t Write(char ch) override;

    IOutputStreamPtr Stream() const override;

private:
    IOutputStreamPtr _stream;
};

using WriterPtr = std::shared_ptr<Writer>;

TWIO_END

#endif // _TWIO_WRITER_H_