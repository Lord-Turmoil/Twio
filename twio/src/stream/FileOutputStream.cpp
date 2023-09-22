#include <twio/stream/FileOutputStream.h>
#include <twio/utils/FileUtil.h>

TWIO_BEGIN


FileOutputStream::FileOutputStream(FILE* fp, bool takeOver)
    : _fp(fp), _takeOver(takeOver)
{
    TWIO_ASSERT(_fp);
    TWIO_ASSERT(!utils::IsWriteOnly(_fp));
}

FileOutputStream::FileOutputStream(const char* path)
{
    _fp = utils::OpenFile(path, "w");

    // _fp must not be null
    TWIO_ASSERT(_fp);

    _takeOver = true;
}

std::shared_ptr<FileOutputStream> New(FILE* fp, bool takeOver)
{
    return std::make_shared<FileOutputStream>(fp, takeOver);
}

FileOutputStream::~FileOutputStream()
{
    Close();
}

void FileOutputStream::Close()
{
    if (_fp)
    {
        utils::CloseFile(_fp);
        _fp = nullptr;
    }
}

bool FileOutputStream::IsReady() const
{
    return _fp != nullptr;
}

size_t FileOutputStream::Write(const char* buffer, size_t size)
{
    TWIO_ASSERT(IsReady());
    return fwrite(buffer, sizeof(char), size, _fp);
}

size_t FileOutputStream::Write(char ch)
{
    TWIO_ASSERT(IsReady());

    const int ret = fputc(ch, _fp);

    return (ret == EOF) ? 0 : 1;
}

TWIO_END
