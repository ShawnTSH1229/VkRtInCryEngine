//--------------------------------------------------------------------------------------
// Copyright (c) 2019-2020, NVIDIA CORPORATION.  All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto.  Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//--------------------------------------------------------------------------------------
// File: NGFX_Injection.h
//
// Distributed as part of the NVIDIA Nsight Graphics SDK.
//--------------------------------------------------------------------------------------

#ifndef NGFX_INJECTION_H_DEF
#define NGFX_INJECTION_H_DEF

#include <stdint.h>

#if defined(__cplusplus) && defined(_WIN32)
#define NGFX_Injection_API __declspec(dllexport)
#else
#define NGFX_Injection_API
#endif

#define NGFX_Injection_API_VersionMajor 0
#define NGFX_Injection_API_VersionMinor 8
#define NGFX_Injection_API_VersionPatch 0
#define NGFX_Injection_API_Version ((NGFX_Injection_API_VersionMajor * 1000) + (NGFX_Injection_API_VersionMinor * 10) + NGFX_Injection_API_VersionPatch)

#define NGFX_Injection_API_MK_NUM(major, minor, patch) major##.##minor##.##patch
#define NGFX_Injection_API_MK_STR_INNER(arg) #arg
#define NGFX_Injection_API_MK_STR(arg) NGFX_Injection_API_MK_STR_INNER(arg)
#define NGFX_Injection_API_VersionString NGFX_Injection_API_MK_STR(NGFX_Injection_API_VersionMajor) "." NGFX_Injection_API_MK_STR(NGFX_Injection_API_VersionMinor) "." NGFX_Injection_API_MK_STR(NGFX_Injection_API_VersionPatch)

#ifdef _WIN32
typedef wchar_t NGFX_Injection_PathChar;
#else
typedef char NGFX_Injection_PathChar;
#endif

typedef enum NGFX_Injection_Result
{
    NGFX_INJECTION_RESULT_OK = 0,
    NGFX_INJECTION_RESULT_FAILURE = -1,
    NGFX_INJECTION_RESULT_INVALID_ARGUMENT = -2,
    NGFX_INJECTION_RESULT_INJECTION_FAILED = -3,
    NGFX_INJECTION_RESULT_ALREADY_INJECTED = -4,
    NGFX_INJECTION_RESULT_NOT_INJECTED = -5,
    NGFX_INJECTION_RESULT_DRIVER_STILL_LOADED = -6,
    NGFX_INJECTION_RESULT_INVALID_PROJECT = -7,
} NGFX_Injection_Result;

typedef enum NGFX_Nsight_SKU
{
    NGFX_NSIGHT_SKU_UNKNOWN,
    NGFX_NSIGHT_SKU_PUBLIC,
    NGFX_NSIGHT_SKU_PRO,
    NGFX_NSIGHT_SKU_INTERNAL,
} NGFX_Nsight_SKU;

typedef struct NGFX_Injection_InstallationInfo
{
    NGFX_Nsight_SKU sku;
    uint16_t versionMajor;
    uint16_t versionMinor;
    uint16_t versionPatch;
    const NGFX_Injection_PathChar* installationPath;
} NGFX_Injection_InstallationInfo;

typedef enum NGFX_Injection_ActivityType
{
    NGFX_INJECTION_ACTIVITY_UNKNOWN = 0,
    NGFX_INJECTION_ACTIVITY_FRAME_DEBUGGER = 1,
    //NGFX_INJECTION_ACTIVITY_FRAME_PROFILER = 2, // this activity has been removed from Nsight Graphics
    NGFX_INJECTION_ACTIVITY_GENERATE_CPP_CAPTURE = 3,
    NGFX_INJECTION_ACTIVITY_GPU_TRACE = 4,
    NGFX_INJECTION_ACTIVITY_PYLON_CAPTURE = 5,
} NGFX_Injection_ActivityType;

typedef struct NGFX_Injection_Activity
{
    NGFX_Injection_ActivityType type;
    const char* description;
} NGFX_Injection_Activity;

#ifdef __cplusplus
extern "C" {
#endif

NGFX_Injection_API NGFX_Injection_Result NGFX_Injection_EnumerateInstallations(uint32_t* pCount, NGFX_Injection_InstallationInfo* pInstallations);

NGFX_Injection_API NGFX_Injection_Result NGFX_Injection_EnumerateActivities(const NGFX_Injection_InstallationInfo* pInstallation, uint32_t* pCount, NGFX_Injection_Activity* pActivities);

NGFX_Injection_API NGFX_Injection_Result NGFX_Injection_SetActivitySettingsFromProjectFile(const NGFX_Injection_InstallationInfo* pInstallation, const NGFX_Injection_PathChar* pProjectFilePath);

NGFX_Injection_API NGFX_Injection_Result NGFX_Injection_SetActivitySettingsFromProjectJson(const NGFX_Injection_InstallationInfo* pInstallation, const char* pJson);

NGFX_Injection_API NGFX_Injection_Result NGFX_Injection_InjectToProcess(const NGFX_Injection_InstallationInfo* pInstallation, const NGFX_Injection_Activity* pActivity);

NGFX_Injection_API NGFX_Injection_Result NGFX_Injection_ExecuteActivityCommand();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef NGFX_INJECTION_H_DEF