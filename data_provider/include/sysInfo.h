/*
 * Wazuh SysInfo
 * Copyright (C) 2015, Wazuh Inc.
 * November 11, 2020.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */


#ifndef _SYS_INFO_H
#define _SYS_INFO_H

// Define EXPORTED for any platform
#include "commonDefs.h"
// #ifdef WAZUH_UNIT_TESTING
// #define EXPORTED
// #else
// #ifdef _WIN32
// #ifdef WIN_EXPORT
// #define EXPORTED __declspec(dllexport)
// #else
// #define EXPORTED __declspec(dllimport)
// #endif
// #elif __GNUC__ >= 4
// #define EXPORTED __attribute__((visibility("default")))
// #else
// #define EXPORTED
// #endif
// #endif


#include "cJSON.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Obtains the hardware information from the current OS being analyzed.
 *
 * @param js_result Resulting json where the specific information will be stored.
 *
 * @return 0 on success, -1 otherwise.
 */
int sysinfo_hardware(cJSON** js_result);

/**
 * @brief Obtains the Operating System information from the current OS being analyzed.
 *
 * @param js_result Resulting json where the specific information will be stored.
 *
 * @return 0 on success, -1 otherwise.
 */
int sysinfo_os(cJSON** js_result);

/**
 * @brief Frees the \p js_data information.
 *
 * @param js_data Information to be freed.
 */
void sysinfo_free_result(cJSON** js_data);

typedef int(*sysinfo_os_func)(cJSON** jsresult);
typedef void(*sysinfo_free_result_func)(cJSON** jsresult);

#ifdef __cplusplus
}
#endif

#endif //_SYS_INFO_H
