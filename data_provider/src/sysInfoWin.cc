/*
 * Wazuh SysInfo
 * Copyright (C) 2015, Wazuh Inc.
 * October 7, 2020.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

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
// #include "defs.h"
// #include "debug_op.h"
#include "osinfo/sysOsInfoWin.h"
#include "windowsHelper.h"
#include "encodingWindowsHelper.h"

constexpr auto CENTRAL_PROCESSOR_REGISTRY {"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"};
const std::string UNINSTALL_REGISTRY{"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"};
constexpr auto SYSTEM_IDLE_PROCESS_NAME {"System Idle Process"};
constexpr auto SYSTEM_PROCESS_NAME {"System"};


std::string SysInfo::getSerialNumber() const
{
    std::string ret;

    if (Utils::isVistaOrLater())
    {
        static auto pfnGetSystemFirmwareTable{Utils::getSystemFirmwareTableFunctionAddress()};

        if (pfnGetSystemFirmwareTable)
        {
            const auto size {pfnGetSystemFirmwareTable('RSMB', 0, nullptr, 0)};

            if (size)
            {
                const auto spBuff{std::make_unique<unsigned char[]>(size)};

                if (spBuff)
                {
                    // Get raw SMBIOS firmware table
                    if (pfnGetSystemFirmwareTable('RSMB', 0, spBuff.get(), size) == size)
                    {
                        PRawSMBIOSData smbios{reinterpret_cast<PRawSMBIOSData>(spBuff.get())};
                        // Parse SMBIOS structures
                        ret = Utils::getSerialNumberFromSmbios(smbios->SMBIOSTableData, size);
                    }
                }
            }
        }
    }
    else
    {
        const auto rawData{Utils::exec("wmic baseboard get SerialNumber")};
        const auto pos{rawData.find("\r\n")};

        if (pos != std::string::npos)
        {
            ret = Utils::trim(rawData.substr(pos), " \t\r\n");
        }
        else
        {
            ret = "UNKNOWN_VALUE";
        }
    }

    return ret;
}

std::string SysInfo::getCpuName() const
{
    Utils::Registry reg(HKEY_LOCAL_MACHINE, CENTRAL_PROCESSOR_REGISTRY);
    return reg.string("ProcessorNameString");
}

int SysInfo::getCpuMHz() const
{
    Utils::Registry reg(HKEY_LOCAL_MACHINE, CENTRAL_PROCESSOR_REGISTRY);
    return reg.dword("~MHz");
}

int SysInfo::getCpuCores() const
{
    SYSTEM_INFO siSysInfo{};
    GetSystemInfo(&siSysInfo);
    return siSysInfo.dwNumberOfProcessors;
}

void SysInfo::getMemory(nlohmann::json& info) const
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);

    if (GlobalMemoryStatusEx(&statex))
    {
        info["ram_total"] = statex.ullTotalPhys / KByte;
        info["ram_free"] = statex.ullAvailPhys / KByte;
        info["ram_usage"] = statex.dwMemoryLoad;
    }
    else
    {
        throw std::system_error
        {
            static_cast<int>(GetLastError()),
            std::system_category(),
            "Error calling GlobalMemoryStatusEx"
        };
    }
}

nlohmann::json SysInfo::getOsInfo() const
{
    nlohmann::json ret;
    const auto spOsInfoProvider
    {
        std::make_shared<SysOsInfoProviderWindows>()
    };
    SysOsInfo::setOsInfo(spOsInfoProvider, ret);
    return ret;
}
