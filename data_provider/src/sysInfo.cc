#include "sysInfo.hpp"
#include "sysInfo.h"

struct CJsonDeleter
{
    void operator()(char* json)
    {
        cJSON_free(json);
    }
    void operator()(cJSON* json)
    {
        cJSON_Delete(json);
    }
};

nlohmann::json SysInfo::hardware()
{
    nlohmann::json ret;
    ret["board_serial"] = getSerialNumber();
    ret["cpu_name"] = getCpuName();
    ret["cpu_cores"] = getCpuCores();
    ret["cpu_mhz"] = double(getCpuMHz());
    getMemory(ret);
    return ret;
}

nlohmann::json SysInfo::os()
{
    return getOsInfo();
}

#ifdef __cplusplus
extern "C" {
#endif

int sysinfo_hardware(cJSON** js_result)
{
    auto retVal { -1 };

    try
    {
        if (js_result)
        {
            SysInfo info;
            const auto& hw          {info.hardware()};
            *js_result = cJSON_Parse(hw.dump().c_str());
            retVal = 0;
        }
    }
    // LCOV_EXCL_START
    catch (...)
    {}

    // LCOV_EXCL_STOP

    return retVal;
}

int sysinfo_os(cJSON** js_result)
{
    auto retVal { -1 };

    try
    {
        if (js_result)
        {
            SysInfo info;
            const auto& os          {info.os()};
            *js_result = cJSON_Parse(os.dump().c_str());
            retVal = 0;
        }
    }
    // LCOV_EXCL_START
    catch (...)
    {}

    // LCOV_EXCL_STOP

    return retVal;
}

void sysinfo_free_result(cJSON** js_data)
{
    if (*js_data)
    {
        cJSON_Delete(*js_data);
    }
}

#ifdef __cplusplus
}
#endif
