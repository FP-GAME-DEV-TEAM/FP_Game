// GameDisplay.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "..\\FPModule\\FPModule.h"
#include "FPClient.h"

extern HINSTANCE hInst;
extern HWND hMainWnd;
extern HMODULE hRcModule;
extern BOOL fWindowed;
extern BOOL fActive;

// Global Variables:
LPDIRECTDRAW7 lpdd = NULL;
LPDIRECTDRAWSURFACE7 lpddsMain = NULL;
LPDIRECTDRAWSURFACE7 lpddsBack = NULL;

PALETTEENTRY palMain[FP_STORE_PAL_COUNT];

IGameEnv *mainEnv;

typedef VOID(WINAPI *ICreateEnv)(IGameEnv **IEnv);

HRESULT Game_Init()
{
	// Load up game resources
	ICreateEnv CreateEnv = (ICreateEnv)GetProcAddress(hRcModule, _T("LoadGameEnv"));
	CreateEnv(&mainEnv);
	_tprintf(_T("%s\n"), mainEnv->GetBinPath());

	// Initialize the game display on window mode or fullscreen
	if (fWindowed == STAGE_MODE_WINDOWED)
	{
		return CreateWindowedDisplay(hMainWnd, STAGE_DEFAULT_WIDTH, STAGE_DEFAULT_HEIGHT);
	}
	else if (fWindowed == STAGE_MODE_FULLSCREEN)
	{
		return CreateFullScreenDisplay(hMainWnd, STAGE_DEFAULT_WIDTH, STAGE_DEFAULT_HEIGHT, STAGE_BBP_HIGH);
	}
	return E_FAIL;
}

HRESULT Game_Shutdown()
{
	DestroyGameDisplay();
	// Do other cleanup
	return S_OK;
}

static HRESULT CreateWindowedDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight)
{
	// Clean up previous DDraw stuff
	DestroyGameDisplay();
	// Init new DDraw stuff begins here
	if (FAILED(DirectDrawCreateEx(NULL, (VOID**)&lpdd, IID_IDirectDraw7, NULL)))
	{
		return E_FAIL;
	}
	// Set cooperative level
	if (FAILED(lpdd->SetCooperativeLevel(hWnd, DDSCL_NORMAL)))
	{
		return E_FAIL;
	}

	RECT  rcWork;
	RECT  rc;
	DWORD dwStyle;
	// If we are still a WS_POPUP window
	// We should convert to a normal app window so we look like a windows app.
	dwStyle = GetWindowStyle(hWnd);
	dwStyle &= ~WS_POPUP;
	dwStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);
	// Adapt window size
	SetRect(&rc, 0, 0, dwWidth, dwHeight);
	AdjustWindowRectEx(&rc, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd));
	SetWindowPos(hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	// Make sure our window does not hang outside of the work area
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
	GetWindowRect(hWnd, &rc);
	if (rc.left < rcWork.left) rc.left = rcWork.left;
	if (rc.top < rcWork.top)  rc.top = rcWork.top;
	SetWindowPos(hWnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	
	// Create the primary surface
	DDSURFACEDESC2 ddsd;
	DDRAW_INIT_STRUCT(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	if (FAILED(lpdd->CreateSurface(&ddsd, &lpddsMain, NULL)))
	{
		return E_FAIL;
	}
	// Create the backbuffer surface
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
	ddsd.dwWidth = dwWidth;
	ddsd.dwHeight = dwHeight;
	if (FAILED(lpdd->CreateSurface(&ddsd, &lpddsBack, NULL)))
	{
		return E_FAIL;
	}

	// Create clipper for the  primary surface
	LPDIRECTDRAWCLIPPER lpClipper;
	if (FAILED(lpdd->CreateClipper(0, &lpClipper, NULL)))
	{
		return E_FAIL;
	}
	if (FAILED(lpClipper->SetHWnd(0, hWnd)))
	{
		lpClipper->Release();
		return E_FAIL;
	}
	if (FAILED(lpddsMain->SetClipper(lpClipper)))
	{
		lpClipper->Release();
		return E_FAIL;
	}
	lpClipper->Release();

	// Update window flag
	fWindowed = TRUE;
	// Reload the icon for app window
	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON)));
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICONSM)));

	return S_OK;
}

HRESULT CreateFullScreenDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP)
{
	RECT  rc;
	DWORD dwStyle;
	// Set app window's style to WS_POPUP so that it can fit for full screen mode.
	dwStyle = GetWindowStyle(hWnd);
	dwStyle &= ~(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	dwStyle |= WS_POPUP;
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);
	// Adapt window size
	SetRect(&rc, 0, 0, dwWidth, dwHeight);
	AdjustWindowRectEx(&rc, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd));

	// Clean up previous DDraw stuff
	DestroyGameDisplay();
	// Init new DDraw stuff begins here
	if (FAILED(DirectDrawCreateEx(NULL, (VOID**)&lpdd, IID_IDirectDraw7, NULL)))
	{
		return E_FAIL;
	}
	// Set cooperative level
	if (FAILED(lpdd->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)))
	{
		return E_FAIL;
	}
	// Set the display mode
	if (FAILED(lpdd->SetDisplayMode(dwWidth, dwHeight, dwBPP, 0, 0)))
	{
		return E_FAIL;
	}

	// Create primary surface (with backbuffer attached)
	DDSURFACEDESC2 ddsd;
	DDRAW_INIT_STRUCT(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |
		DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
	ddsd.dwBackBufferCount = 1;
	if (FAILED(lpdd->CreateSurface(&ddsd, &lpddsMain, NULL)))
	{
		return E_FAIL;
	}

	// Get a pointer to the back buffer
	DDSCAPS2 ddscaps;
	ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	if (FAILED(lpddsMain->GetAttachedSurface(&ddscaps, &lpddsBack)))
	{
		return E_FAIL;
	}
	lpddsBack->AddRef();

	// Update window flag
	fWindowed = FALSE;

	return S_OK;
}

VOID DestroyGameDisplay()
{
	SAFE_RELEASE(lpddsBack);
	SAFE_RELEASE(lpddsMain);
	if (lpdd)
	{
		lpdd->SetCooperativeLevel(hMainWnd, DDSCL_NORMAL);
	}
	SAFE_RELEASE(lpdd);
}

