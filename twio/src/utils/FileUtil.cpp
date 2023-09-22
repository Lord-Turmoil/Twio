#include <twio/utils/FileUtil.h>

#if _TWIO_FOR_WIN32
// ... To be continued... :(
#else
#include <unistd.h>
#endif

TWIO_BEGIN

namespace util
{
    FILE* OpenFile(const char* path, const char* mode)
    {
        FILE* fp;

        int ret = fopen_s(&fp, path, mode);
        if (ret != 0)
        {
            return nullptr;
        }

        return fp;
    }

#if _TWIO_FOR_WIN32
    bool IsReadOnly(FILE* fp)
    {
        // :(
        return true;
    }
#else
    bool IsReadOnly(FILE* fp)
    {
        int fd = fileno(file);
        int mode = fcntl(fd, F_GETFL);

        return mode == 32768;
    }
#endif

#if _TWIO_FOR_WIN32
    bool IsWriteOnly(FILE* Fp)
    {
        // :(
        return true;
    }
#else
    bool IsWriteOnly(FILE* fp)
    {
        int fd = fileno(file);
        int mode = fcntl(fd, F_GETFL);

        return mode == 32769;
    }
#endif
};


TWIO_END