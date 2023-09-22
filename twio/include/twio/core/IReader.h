#ifndef _TWIO_IREADER_H_
#define _TWIO_IREADER_H_

#include <twio/Macros.h>
#include <memory>

TWIO_BEGIN


class IReader
{
public:
    virtual ~IReader() = 0;

    virtual size_t Read(char* buffer, size_t size) = 0;
    virtual int Read() = 0;

    virtual int Rewind() = 0;
};

inline IReader::~IReader() = default;

using IReaderPtr = std::shared_ptr<IReader>;


class IAdvancedReader : public IReader
{
public:
    ~IAdvancedReader() override = 0;

    virtual int Line() const;
    virtual int Char() const;
};

inline IAdvancedReader::~IAdvancedReader() = default;

using IAdvancedReaderPtr = std::shared_ptr<IAdvancedReader>;


TWIO_END

#endif // _TWIO_IREADER_H_
