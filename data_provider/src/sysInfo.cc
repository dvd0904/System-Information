#include "sysInfo.hpp"
#include "sysInfo.h"
#include <iostream>

nlohmann::json SysInfo::hardware()
{
    nlohmann::json ret;
    ret["board_serial"] = getSerialNumber();
    ret["cpu_name"] = getCpuName();
    ret["cpu_cores"] = getCpuCores();
    ret["cpu_mhz"] = double(getCpuMHz());
    ret["machine_id"] = getMachineId();
    ret["machine_guid"] = getMachineGuid();
    // ret["disk_model"] = getDiskInfo("Model");
    // ret["disk_serialNumber"] = getDiskInfo("\"Serial Number\"");
    // ret["disk_firmwareVersion"] = getDiskInfo("\"Firmware Revision\"");
    // ret["disk_size"] = getDiskInfo("Size");
    getMemory(ret);
    return ret;
}

nlohmann::json SysInfo::os()
{
    return getOsInfo();
}