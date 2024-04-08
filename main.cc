#include <iostream>

#include "sysInfo.h"
#include "sysInfo.hpp"

constexpr auto JSON_PRETTY_SPACES
{
    2
};


class SysInfoPrint
{
    public:
        SysInfoPrint() = default;
        void func()
        {
            my_data["hw"] = my_sysinfo.hardware();
            my_data["os"] = my_sysinfo.os();
        }

        void printData()
        {
            std::cout << my_data.dump(JSON_PRETTY_SPACES) << std::endl;
        }

    private:
        SysInfo my_sysinfo;
        nlohmann::json my_data;

};

int main()
{
    // std::cout << "Hello\n";
    SysInfoPrint printer;
    printer.func();
    printer.printData();
}