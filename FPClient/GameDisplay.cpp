// GameDisplay.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FPClient.h"

// Global Variables:
LPDIRECTDRAW7 lpdd = NULL;
LPDIRECTDRAWSURFACE7 lpddsMain = NULL;
LPDIRECTDRAWSURFACE7 lpddsBack = NULL;

PALETTEENTRY palMain[FP_STORE_PAL_COUNT];

// Declarations of internal functions
static HRESULT CreateWindowedDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight);
static HRESULT CreateFullScreenDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP);

HRESULT InitGameDisplay(BOOL flag)
{
	DWORD dwRet = E_FAIL;
	// Check if DirectDraw Object exist
	if (NULL == lpdd)
	{
		return dwRet;
	}

	// Clean up previous DDraw stuff
	DestroyGameDisplay();

	// Initialize the game display on window mode or fullscreen
	if (fWindowed == STAGE_MODE_WINDOWED)
	{
		dwRet = CreateWindowedDisplay(hMainWnd, STAGE_DEFAULT_WIDTH, STAGE_DEFAULT_HEIGHT);
		if (S_OK == dwRet)
		{
			fWindowed = TRUE;
			FP_DEBUG_MSG(_T("Switched to windowed display mode.\n"));
		}
	}
	else if (fWindowed == STAGE_MODE_FULLSCREEN)
	{
		dwRet = CreateFullScreenDisplay(hMainWnd, STAGE_DEFAULT_WIDTH, STAGE_DEFAULT_HEIGHT, STAGE_BPP_INDEX);
		if (S_OK == dwRet)
		{
			fWindowed = FALSE;
			FP_DEBUG_MSG(_T("Switched to full-screen display mode.\n"));
		}
	}
	return dwRet;
}

VOID DestroyGameDisplay()
{
	// Just RELEASE them, not DELETE them.
	SAFE_RELEASE(lpddsBack);
	SAFE_RELEASE(lpddsMain);
}

static HRESULT CreateWindowedDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight)
{
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
	dwStyle = GetWindowLong(hWnd, GWL_STYLE);
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
	SetClassLong(hWnd, GCL_HICONSM, (LONG)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICONSM)));
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LONG)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICONSM)));
	UpdateWindow(hWnd);

	return S_OK;
}

static HRESULT CreateFullScreenDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP)
{
	RECT  rc;
	DWORD dwStyle;
	// Set app window's style to WS_POPUP so that it can fit for full screen mode.
	dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	dwStyle &= ~(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	dwStyle |= WS_POPUP;
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);
	// Adapt window size
	SetRect(&rc, 0, 0, dwWidth, dwHeight);
	AdjustWindowRectEx(&rc, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd));

	// Set cooperative level
	if (FAILED(lpdd->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT)))
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
	SetClassLong(hWnd, GCL_HICONSM, NULL);
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, NULL);
	UpdateWindow(hWnd);

	return S_OK;
}
