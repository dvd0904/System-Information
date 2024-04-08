#include "sysInfo.hpp"
#include "sysInfo.h"

nlohmann::json SysInfo::os()
{
    return getOsInfo();
}