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
#include <windowsx.h>
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

// TODO: reference additional headers your program requires here
#pragma comment(lib, ".\\lib\\ddraw.lib")
#pragma comment(lib, ".\\lib\\dxguid.lib")

#ifndef FP_CLIENT_DEBUG
#define FP_CLIENT_DEBUG
#endif

#ifdef FP_CLIENT_DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif
