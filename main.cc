#include <iostream>

#include "sysInfo.h"
#include "sysInfo.hpp"

// int sysinfo_os(cJSON **js_result)
// {
//     auto retVal{-1};

//     try
//     {
//         if (js_result)
//         {
//             SysInfo info;
//             const auto &os{info.os()};
//             *js_result = cJSON_Parse(os.dump().c_str());
//             retVal = 0;
//         }
//     }
//     // LCOV_EXCL_START
//     catch (...)
//     {
//     }

//     // LCOV_EXCL_STOP

//     return retVal;
// }

class SysInfoPrint
{
    public:
        SysInfoPrint() = default;
        void func()
        {

        }

    private:
        SysInfo my_sysinfo;
        nlohmann::json my_data;

};

int main()
{
    std::cout << "Hello\n";
}