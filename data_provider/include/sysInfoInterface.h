#ifndef _SYS_INFO_INTERFACE
#define _SYS_INFO_INTERFACE

#include "json.hpp"

class ISysInfo
{
    public:
        ISysInfo() = default;
        // LCOV_EXCL_START
        virtual ~ISysInfo() = default;
        // LCOV_EXCL_STOP
        virtual nlohmann::json hardware() = 0;
        virtual nlohmann::json os() = 0;
};

#endif //_SYS_INFO_INTERFACE
