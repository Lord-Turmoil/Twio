#include <twio/stream/FileInputStream.h>
#include <twio/utils/FileUtil.h>

TWIO_BEGIN


FileInputStream::FileInputStream(FILE* fp, bool takeOver)
    : _fp(fp), _takeOver(takeOver)
{
    TWIO_ASSERT(_fp);
    TWIO_ASSERT(!utils::IsReadOnly(_fp));
}

FileInputStream::FileInputStream(const char* path)
{
    _fp = utils::OpenFile(path, "r");

    // _fp must not be null
    TWIO_ASSERT(_fp);

    _takeOver = true;
}

std::shared_ptr<FileInputStream> New(FILE* fp, bool takeOver)
{
    return std::make_shared<FileInputStream>(fp, takeOver);
}

FileInputStream::~FileInputStream()
{
    Close();
}

void FileInputStream::Close()
{
    if (_fp)
    {
        utils::CloseFile(_fp);
        _fp = nullptr;
    }
}

bool FileInputStream::IsReady() const
{
    return _fp != nullptr;
}

size_t FileInputStream::Read(char* buffer, size_t size)
{
    TWIO_ASSERT(!IsReady());
    const size_t count = fread(buffer, sizeof(char), size, _fp);
    buffer[count] = '\0';
    return count;
}

size_t FileInputStream::Read(char* buffer)
{
    TWIO_ASSERT(!IsReady());

    const int ch = fgetc(_fp);
    if (ch == EOF)
    {
        if (*buffer)
        {
            *buffer = '\0';
        }
        return 0;
    }
    if (*buffer)
    {
        *buffer = static_cast<char>(ch);
    }

    return 1;
}

int FileInputStream::Read()
{
    TWIO_ASSERT(!IsReady());

    return fgetc(_fp);
}

TWIO_END
