#include <ws2tcpip.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <winternl.h>
#include <ntstatus.h>
#include <iphlpapi.h>
#include <memory>
#include <list>
#include <set>
#include <system_error>
#include <winternl.h>
#include <ntstatus.h>
#include <netioapi.h>
#include "sysinfoapi.h"
#include "sysInfo.hpp"
#include "cmdHelper.h"
#include "stringHelper.h"
#include "registryHelper.h"
// #include "osinfo/sysOsInfoWin.h"
#include "windowsHelper.h"
#include "encodingWindowsHelper.h"

#include <iostream>


nlohmann::json SysInfo::getOsInfo() const
{
    nlohmann::json ret;
    // const auto spOsInfoProvider{
    //     std::make_shared<SysOsInfoProviderWindows>()};
    // SysOsInfo::setOsInfo(spOsInfoProvider, ret);
    std::cout << "getOsInfo called\n";
    return ret;
}
