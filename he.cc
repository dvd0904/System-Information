#include <windows.h>
#include <string>
#include <iostream>


std::string getMachineId() {
    HKEY hKey;
    LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
    if (lResult != ERROR_SUCCESS) {
        throw std::runtime_error("Error opening registry key.");
    }

    CHAR machineGuid[256];
    DWORD dwSize = sizeof(machineGuid);
    lResult = RegQueryValueEx(hKey, "MachineGuid", NULL, NULL, (LPBYTE)&machineGuid, &dwSize);
    RegCloseKey(hKey);

    if (lResult != ERROR_SUCCESS) {
        throw std::runtime_error("Error querying registry value.");
    }

    return std::string(machineGuid);
}


// std::string getMachineId() {
//     HKEY hKey;
//     LONG lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
//     if (lResult != ERROR_SUCCESS) {
//         throw std::runtime_error("Error opening registry key.");
//     }

//     CHAR machineGuid[256];
//     DWORD dwSize = sizeof(machineGuid);
//     lResult = RegQueryValueExA(hKey, "MachineGuid", NULL, NULL, (LPBYTE)&machineGuid, &dwSize);
//     RegCloseKey(hKey);

//     if (lResult != ERROR_SUCCESS) {
//         throw std::runtime_error("Error querying registry value.");
//     }

//     return std::string(machineGuid);
// }

int main() {
    try {
        std::string machineId = getMachineId();
        // Use machineId as needed
        std::cout << machineId;
    }
    catch (const std::exception& ex) {
        // Handle errors
        return 1;
    }

    return 0;
}
