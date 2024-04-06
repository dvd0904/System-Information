#ifndef _SYS_INFO_HPP
#define _SYS_INFO_HPP


#include "sysInfoInterface.h"

constexpr auto KByte{1024};

class SysInfo: public ISysInfo
{
public:
    SysInfo() = default;
    // LCOV_EXCL_START
    virtual ~SysInfo() = default;
    // LCOV_EXCL_STOP
    nlohmann::json hardware();
    nlohmann::json os();
private:
    virtual std::string getSerialNumber() const;
    virtual std::string getCpuName() const;
    virtual int getCpuMHz() const;
    virtual int getCpuCores() const;
    virtual void getMemory(nlohmann::json& info) const;
    virtual nlohmann::json getOsInfo() const;

};

#endif //_SYS_INFO_HPP
