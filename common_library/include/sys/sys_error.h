#ifndef SYS_ERROR_H
#define SYS_ERROR_H
#include <errno.h>
#include <string.h>
#include "sys/sys_config.h"
SYS_NAMESPACE_BEGIN

class ErrorKeeper
{
public:
    ErrorKeeper()
        :_errcode(errno)
    {
    }

    ErrorKeeper(int errcode)
        :_errcode(errcode)
    {
    }

    ~ErrorKeeper()
    {
        errno = _errcode;
    }

    operator int() const
    {
        return _errcode;
    }

private:
    int _errcode;
};

namespace Error
{
inline int code()
{
    return errno;
}

inline void set(int errcode)
{
    errno = errcode;
}

inline std::string to_string()
{
    return strerror(errno);
}

inline std::string to_string(int errcode)
{
    return strerror(errcode);
}
}

SYS_NAMESPACE_END
#endif // SYS_ERROR_H
