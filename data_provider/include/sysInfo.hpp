#ifndef _SYS_INFO_HPP
#define _SYS_INFO_HPP

// #include "sysInfoInterface.h"
#include "json.hpp"


constexpr auto KByte{1024};

class SysInfo
{
    public:
        SysInfo() = default;
        // LCOV_EXCL_START
        ~SysInfo() = default;
        // LCOV_EXCL_STOP
        nlohmann::json hardware();
        nlohmann::json os();
    private:
        std::string getSerialNumber() const;
        std::string getCpuName() const;
        int getCpuMHz() const;
        int getCpuCores() const;
        void getMemory(nlohmann::json& info) const;
        nlohmann::json getOsInfo() const;
        std::string getMachineId() const;
        std::string getMachineGuid() const;

};

#endif //_SYS_INFO_HPP

// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Cryptography