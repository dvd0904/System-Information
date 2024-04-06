#ifdef WIN32

#ifndef _NETWORK_WINDOWS_HELPER_H
#define _NETWORK_WINDOWS_HELPER_H

#include <map>
#include <memory>
#include <vector>
#include <system_error>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <versionhelpers.h>
// #include "mem_op.h"
#include "stringHelper.h"
#include "encodingWindowsHelper.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wcast-function-type"

constexpr auto WORKING_ADAPTERS_INFO_BUFFER_SIZE
{
    15000
};
constexpr auto MAX_ADAPTERS_INFO_TRIES
{
    3
};

constexpr int BASEBOARD_INFORMATION_TYPE
{
    2
};

typedef struct RawSMBIOSData
{
    BYTE    Used20CallingMethod;
    BYTE    SMBIOSMajorVersion;
    BYTE    SMBIOSMinorVersion;
    BYTE    DmiRevision;
    DWORD   Length;
    BYTE    SMBIOSTableData[];
} RawSMBIOSData, *PRawSMBIOSData;

typedef struct SMBIOSStructureHeader
{
    BYTE Type;
    BYTE FormattedAreaLength;
    WORD Handle;
} SMBIOSStructureHeader;

typedef struct SMBIOSBaseboardInfoStructure
{
    BYTE Type;
    BYTE FormattedAreaLength;
    WORD Handle;
    BYTE Manufacturer;
    BYTE Product;
    BYTE Version;
    BYTE SerialNumber;
} SMBIOSBaseboardInfoStructure;

namespace Utils
{

    typedef UINT (WINAPI* GetSystemFirmwareTable_t)(DWORD, DWORD, PVOID, DWORD);
    static GetSystemFirmwareTable_t getSystemFirmwareTableFunctionAddress()
    {
        GetSystemFirmwareTable_t ret{nullptr};
        auto hKernel32 { GetModuleHandle(TEXT("kernel32.dll")) };

        if (hKernel32)
        {
            ret = reinterpret_cast<GetSystemFirmwareTable_t>(GetProcAddress(hKernel32, "GetSystemFirmwareTable"));
        }

        return ret;
    }

    typedef NETIOAPI_API (WINAPI* GetIfEntry2_t)(PMIB_IF_ROW2);
    static GetIfEntry2_t getIfEntry2FunctionAddress()
    {
        GetIfEntry2_t ret{nullptr};
        auto hIphlpapi { GetModuleHandle(TEXT("Iphlpapi.dll")) };

        if (hIphlpapi)
        {
            ret = reinterpret_cast<GetIfEntry2_t>(GetProcAddress(hIphlpapi, "GetIfEntry2"));
        }

        return ret;
    }

    static bool isVistaOrLater()
    {
        static const bool ret
        {
            IsWindowsVistaOrGreater()
        };
        return ret;
    }


    /* Reference: https://www.dmtf.org/sites/default/files/standards/documents/DSP0134_2.6.0.pdf */
    static std::string getSerialNumberFromSmbios(const BYTE* rawData, const DWORD rawDataSize)
    {
        std::string serialNumber;
        DWORD offset{0};

        if (nullptr != rawData)
        {
            std::unique_ptr<BYTE[]> tmpBuffer { std::make_unique<BYTE[]>(rawDataSize + 1) };
            memcpy(tmpBuffer.get(), rawData, rawDataSize);

            while (offset < rawDataSize && serialNumber.empty())
            {
                if (offset + sizeof(SMBIOSStructureHeader) >= rawDataSize)
                {
                    break;
                }

                SMBIOSStructureHeader header{};
                memcpy(&header, tmpBuffer.get() + offset, sizeof(SMBIOSStructureHeader));

                if (offset + header.FormattedAreaLength >= rawDataSize || offset + sizeof(SMBIOSBaseboardInfoStructure) >= rawDataSize)
                {
                    break;
                }

                if (BASEBOARD_INFORMATION_TYPE == header.Type)
                {
                    SMBIOSBaseboardInfoStructure info{};
                    memcpy(&info, tmpBuffer.get() + offset, sizeof(SMBIOSBaseboardInfoStructure));
                    offset += info.FormattedAreaLength;

                    for (BYTE i = 1; i < info.SerialNumber; ++i)
                    {
                        const char* tmp{reinterpret_cast<const char*>(tmpBuffer.get() + offset)};

                        if (offset < rawDataSize)
                        {
                            const auto len{ nullptr != tmp ? strlen(tmp) : 0 };
                            offset += len + sizeof(char);
                        }
                    }

                    if (offset < rawDataSize)
                    {
                        serialNumber = reinterpret_cast<const char*>(tmpBuffer.get() + offset);
                    }
                }
                else
                {
                    offset += header.FormattedAreaLength;

                    // Search for the end of the unformatted structure (\0\0)
                    while (offset + 1 < rawDataSize)
                    {
                        if (!(*(tmpBuffer.get() + offset)) && !(*(tmpBuffer.get() + offset + 1)))
                        {
                            offset += 2;
                            break;
                        }

                        offset++;
                    }
                }
            }
        }

        return serialNumber;
    }
}

#pragma GCC diagnostic pop

#endif // _NETWORK_WINDOWS_HELPER_H

#endif //WIN32
