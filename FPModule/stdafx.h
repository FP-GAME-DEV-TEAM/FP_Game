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
#include <process.h>

// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

// C++ RunTime Header Files
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <set>


/* This macro is for dll debugging */
#ifndef FP_MODULE_DEBUG
#define FP_MODULE_DEBUG
#define FP_DEBUG_PATH _T("F:\\FantasyParadise\\Game\\test.exe")
#endif 

/* This macro is for project debugging */
#ifndef FP_PROJECT_DEBUG
#define FP_PROJECT_DEBUG
#endif

/* This macro is for dll exports */
#ifndef FP_MODULE_COMPILE
#define FP_MODULE_COMPILE
#endif
