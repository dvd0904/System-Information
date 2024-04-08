#ifndef _SYS_OS_INFO_WINDOWS_H
#define _SYS_OS_INFO_WINDOWS_H

#include <string>
#include <memory>
#include "json.hpp"

#define UNKNOWN_VALUE ""

class SysOsInfoProviderWindows
{
    public:
        SysOsInfoProviderWindows();
        ~SysOsInfoProviderWindows() = default;
        std::string name() const;
        std::string version() const;
        std::string majorVersion() const;
        std::string minorVersion() const;
        std::string build() const;
        std::string release() const;
        std::string displayVersion() const;
        std::string machine() const;
        std::string nodeName() const;
    private:
        const std::string m_majorVersion;
        const std::string m_minorVersion;
        const std::string m_build;
        const std::string m_version;
        const std::string m_release;
        const std::string m_displayVersion;
        const std::string m_name;
        const std::string m_machine;
        const std::string m_nodeName;
};


class SysOsInfo
{
    public:
        SysOsInfo() =  default;
        ~SysOsInfo() = default;
        static void setOsInfo(const SysOsInfoProviderWindows &osInfoProvider, nlohmann::json& output)
        {
            output["os_name"] = osInfoProvider.name();
            output["os_major"] = osInfoProvider.majorVersion();
            output["os_minor"] = osInfoProvider.minorVersion();
            output["os_build"] = osInfoProvider.build();
            output["os_version"] = osInfoProvider.version();
            output["hostname"] = osInfoProvider.nodeName();
            output["os_release"] = osInfoProvider.release();
            output["os_display_version"] = osInfoProvider.displayVersion();
            output["architecture"] = osInfoProvider.machine();
        }
};



#endif //_SYS_OS_INFO_WINDOWS_H