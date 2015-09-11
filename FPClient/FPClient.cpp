// FPClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FPClient.h"

#define MAX_LOADSTRING 64

// Global Variables:
HINSTANCE hInst;								// Current instance
HWND hMainWnd;									// Main window handle
HMODULE hRcModule;								// FPModule dll

TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The main window class name

BOOL fWindowed = FALSE;							// Flag of whether the game is in window mode
BOOL fActive = FALSE;							// Flag of whether the game window is actived
RECT rcWindow;									// Rect of the game window
RECT rcViewport;								// Rect of the game viewport
DWORD dwLastTick;								// Time of last logical frame displayed


// Forward declarations of functions included in this code module:
BOOL				InitInstance(HINSTANCE, int, HACCEL*);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	DWORD dwRet;
	HACCEL hAccel;
	BOOL fExit = FALSE;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_CLS_NAME, szWindowClass, MAX_LOADSTRING);

	// Perform application initialization:
	fWindowed = TRUE;
	if (!InitInstance(hInstance, nCmdShow, &hAccel))
	{
		return -1;
	}

	// Load dependent lib(s)
	hRcModule = LoadLibrary(GAME_RESOURCE_DLL);
	if (NULL == hRcModule)
	{
		MessageBox(hMainWnd, _T("Can not find FPModule.dll!"), _T("Error"), MB_ICONERROR | MB_OK);
		return -2;
	}

	// Init main game logic
	Game_Init();

	// Main message loop:
	HANDLE hMainTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -1i64;
	LONG lTimeout = 1000 / GAME_FRAME_RATE;
	SetWaitableTimer(hMainTimer, &liDueTime, lTimeout, NULL, NULL, 0);
	while (!fExit)
	{
		dwRet = MsgWaitForMultipleObjects(1, &hMainTimer, FALSE, INFINITE, QS_ALLINPUT);
		switch (dwRet)
		{
		case GAME_LOOP_TIMER:
			//≥°æ∞±‰ªª”Î‰÷»æ
			break;

		case GAME_LOOP_SYSMSG:
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (WM_QUIT == msg.message)
				{
					fExit = TRUE;
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
	return (int)msg.wParam;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HACCEL *phAccel)
{
	WNDCLASSEX wcex;
	// Register the window class
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDS_APP_TITLE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;
	if (!RegisterClassEx(&wcex))
	{
		return E_FAIL;
	}

	// Initialization of window instance
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MAIN_ACCEL));
	*phAccel = hAccel; // Store accelerate keys
	hMainWnd = CreateWindowEx(NULL, szWindowClass, szTitle, WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
	hInst = hInstance; // Store instance handle in our global variable
	if (!hMainWnd)
	{
		return FALSE;
	}
	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);
	GetWindowRect(hMainWnd, &rcWindow);
	return TRUE;
}
