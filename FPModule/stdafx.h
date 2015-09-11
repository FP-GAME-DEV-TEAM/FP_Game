// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma pack(1)

#include "targetver.h"

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

// Windows Header Files:
#include <windows.h>
#include <objbase.h>
#include <ddraw.h>

// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

// C++ RunTime Header Files
#include <string>
#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <hash_map>
#include <hash_set>

/* This macro is for dll exports */
#ifndef FP_MODULE_COMPILE
#define FP_MODULE_COMPILE
#endif

/* This macro is for dll debugging */
#define FP_MODULE_DEBUG
#define FP_DEBUG_PATH _T("F:\\Programming\\CGame\\FPMainWorkspace\\GameTest\\Null.exe")

