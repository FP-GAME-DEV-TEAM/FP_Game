// FPClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FPClient.h"

#define MAX_LOADSTRING 64

// Global Variables:
HINSTANCE hInst;								// Current instance
HWND hMainWnd;									// Main window handle
HMODULE hRcModule;								// FPModule DLL

TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The main window class name

BOOL fWindowed = FALSE;							// Flag of whether the game is in window mode
BOOL fActive = FALSE;							// Flag of whether the game window is actived
RECT rcWindow;									// Rect of the game window
RECT rcViewport;								// Rect of the game viewport
DWORD dwLastTick;								// Time of last logical frame displayed

IGameEnv *gameEnv = NULL;						// Main game resource interface
IGameGraphics *gameGraphics = NULL;				// Main game graphics interface

// Define game resource interface
typedef HRESULT(WINAPI *ICreateEnv)(IGameEnv **IEnv);
typedef HRESULT(WINAPI *IDestroyEnv)();

typedef HRESULT(WINAPI *ICreateGraphics)(IGameGraphics **IGraphics);
typedef HRESULT(WINAPI *IDestroyGraphics)();


// Forward declarations of functions included in this code module:
BOOL				InitWindow(int, HACCEL*);
LRESULT CALLBACK	MainProc(HWND, UINT, WPARAM, LPARAM);
HRESULT				GameLoop();
HRESULT				GameInit();
HRESULT				GameExit();

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASSEX wcex;
	MSG msg;
	DWORD dwRet;
	HACCEL hAccel;
	BOOL fExit = FALSE;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_CLS_NAME, szWindowClass, MAX_LOADSTRING);

	// Register the window class
	hInst = hInstance; // Store instance handle in our global variable
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC | CS_DBLCLKS;
	wcex.lpfnWndProc = MainProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDS_APP_TITLE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;
	if (!RegisterClassEx(&wcex))
	{
		return -1;
	}

	// Perform application initialization:
	fWindowed = TRUE;
	if (!InitWindow(nCmdShow, &hAccel))
	{
		return -2;
	}

	// Start game logic
	FP_DEBUG_MSG(_T("Game starting now...\n"));
	if (FAILED(GameInit()))
	{
		MessageBox(hMainWnd, _T("Game starting failed!"), _T("Error"), MB_ICONERROR | MB_OK);
		return -3;
	}

	// Main message loop:
	HANDLE hMainTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	if (!hMainTimer)
	{
		FP_DEBUG_MSG(_T("CreateWaitableTimer failed (%d)\n"), GetLastError());
		return -4;
	}
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -1i64;
	LONG lTimeout = 1000 / GAME_FRAME_RATE * 2;
	SetWaitableTimer(hMainTimer, &liDueTime, lTimeout, NULL, NULL, 0);
	while (!fExit)
	{
		dwRet = MsgWaitForMultipleObjects(1, &hMainTimer, FALSE, INFINITE, QS_ALLINPUT);
		switch (dwRet)
		{
		case GAME_LOOP_TIMER:
			//³¡¾°±ä»»ÓëäÖÈ¾
			GameLoop();
			break;

		case GAME_LOOP_SYSMSG:
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (WM_QUIT == msg.message)
				{
					fExit = TRUE;
					// Release game logic
					GameExit();
					break;
				}
				if (0 == TranslateAccelerator(hMainWnd, hAccel, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			break;

		default:
			WaitMessage();
			break;
		}
	}
	CloseHandle(hMainTimer);
	FreeLibrary(hRcModule);
#ifdef FP_PROJECT_DEBUG
	system("PAUSE");
#endif
	return (int)msg.wParam;
}

BOOL InitWindow(int nCmdShow, HACCEL *phAccel)
{
	// Initialization of window instance
	HACCEL hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_MAIN_ACCEL));
	*phAccel = hAccel; // Store accelerate keys
	hMainWnd = CreateWindowEx(NULL, szWindowClass, szTitle, WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInst, NULL);
	//hMainWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInst, NULL);
	if (!hMainWnd)
	{
		return FALSE;
	}
	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);
	return TRUE;
}

HRESULT GameLoop()
{
	gameGraphics->LoopIORequest(GetTickCount());
	return S_OK;
}

HRESULT GameInit()
{
	// Load dependent lib(s)
	FP_DEBUG_MSG(_T("Loading FP DLL module...\n"));
	hRcModule = LoadLibrary(GAME_RESOURCE_DLL);
	if (NULL == hRcModule)
	{
		MessageBox(hMainWnd, _T("Can not find file \"FPModule.dll\"!"), _T("Error"), MB_ICONERROR | MB_OK);
		return E_NOINTERFACE;
	}

	// Init game resources
	if (NULL == gameEnv)
	{
		// Game Env
		ICreateEnv CreateEnv = (ICreateEnv)GetProcAddress(hRcModule, "InitGameEnv");
		if (FAILED(CreateEnv(&gameEnv)))
		{
			FP_DEBUG_MSG(_T("Failed to open Game Env!\n"));
			return E_FAIL;
		}
		FP_DEBUG_MSG(_T("Game Env has been initialized:\n\t@ %s\n"), gameEnv->GetRootPath());

		// Game graphics
		ICreateGraphics CreateGraphics = (ICreateGraphics)GetProcAddress(hRcModule, "InitGraphics");
		if (FAILED(CreateGraphics(&gameGraphics)))
		{
			FP_DEBUG_MSG(_T("Failed to initialize Game Graphics!\n"));
			return E_FAIL;
		}
	}

	// Init DirectDraw & prepare for game display
	if (NULL == lpdd)
	{
		if (FAILED(DirectDrawCreateEx(NULL, (VOID**)&lpdd, IID_IDirectDraw7, NULL)))
		{
			return E_FAIL;
		}
		FP_DEBUG_MSG(_T("DirectDraw has been created.\n"));
		if (FAILED(InitGameDisplay(fWindowed)))
		{
			FP_DEBUG_MSG(_T("Failed to start display mode!\n"));
			return E_FAIL;
		}
	}

	FP_DEBUG_MSG(_T("Game Started!\n"));
	return S_OK;
}

HRESULT GameExit()
{
	// Destroy game display
	if (lpdd)
	{
		lpdd->SetCooperativeLevel(hMainWnd, DDSCL_NORMAL);
	}
	SAFE_RELEASE(lpdd);
	FP_DEBUG_MSG(_T("DirectDraw has been destroyed.\n"));

	// Release game resources
	if (NULL != gameEnv)
	{
		// Game graphics
		IDestroyGraphics DestroyGraphics = (IDestroyGraphics)GetProcAddress(hRcModule, "ReleaseGraphics");
		if (FAILED(DestroyGraphics()))
		{
			FP_DEBUG_MSG(_T("Failed to release Game Graphics!\n"));
			return E_FAIL;
		}

		// Game Env
		gameEnv = NULL;
		FP_DEBUG_MSG(_T("Game Env has been released.\n"));
		IDestroyEnv DestroyEnv = (IDestroyEnv)GetProcAddress(hRcModule, "ReleaseGameEnv");
		if (FAILED(DestroyEnv()))
		{
			FP_DEBUG_MSG(_T("Failed to close Game Env!\n"));
			return E_FAIL;
		}
	}

	// Unload dependent lib(s)
	FP_DEBUG_MSG(_T("Unloading FP DLL module...\n"));
	FreeLibrary(hRcModule);

	// Do other cleanup
	FP_DEBUG_MSG(_T("Game Shutdown!\n"));
	return S_OK;
}
