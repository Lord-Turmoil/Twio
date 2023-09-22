#ifndef _TWIO_IREADER_H_
#define _TWIO_IREADER_H_

#include <twio/Macros.h>

TWIO_BEGIN


class IReader
{
public:
    virtual ~IReader() = default;

    virtual size_t Read(char* buffer, size_t size) = 0;
    virtual int Read() = 0;

    virtual int Rewind() = 0;
};


class IAdvancedReader : public IReader
{
public:
    ~IAdvancedReader() override = default;

    virtual int Line() const;
    virtual int Char() const;
};


TWIO_END

#endif // _TWIO_IREADER_H_
