// GameDisplay.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FPClient.h"

// Global Variables:
LPDIRECTDRAW7 lpdd = NULL;
LPDIRECTDRAWSURFACE7 lpddsMain = NULL;
LPDIRECTDRAWSURFACE7 lpddsBack = NULL;
LPDIRECTDRAWSURFACE7 lpddsTiles = NULL;
LPDIRECTDRAWSURFACE7 lpddsObjects = NULL;

PALETTEENTRY palMain[FP_STORE_PAL_COUNT];

// Declarations of internal functions
static HRESULT CreateWindowedDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight);
static HRESULT CreateFullScreenDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP);

HRESULT InitGameDisplay(DWORD dwType)
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
	if (dwType == STAGE_MODE_WINDOWED)
	{
		dwRet = CreateWindowedDisplay(hMainWnd, STAGE_WINDOW_WIDTH, STAGE_WINDOW_HEIGHT);
		if (S_OK == dwRet)
		{
			FP_DEBUG_MSG(_T("Switched to windowed display mode.\n"));
		}
	}
	else if (dwType == STAGE_MODE_FULLSCREEN)
	{
		dwRet = CreateFullScreenDisplay(hMainWnd, STAGE_DEFAULT_WIDTH, STAGE_DEFAULT_HEIGHT, STAGE_BBP_HIGH);
		if (S_OK == dwRet)
		{
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
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CKSRCBLT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
	ddsd.dwWidth = dwWidth;
	ddsd.dwHeight = dwHeight;
	if (FAILED(lpdd->CreateSurface(&ddsd, &lpddsBack, NULL)))
	{
		return E_FAIL;
	}

	// Set surface color key
	DDCOLORKEY ddCK;
	ddCK.dwColorSpaceLowValue = 0;
	ddCK.dwColorSpaceHighValue = 0;
	if (lpddsBack->SetColorKey(DDCKEY_SRCBLT, &ddCK))
	{
		return E_FAIL;
	}

	// Create clipper for the primary surface
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
	lpClipper = NULL;

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
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT | DDSD_CKSRCBLT;
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
	//lpddsBack->AddRef();

	// Set surface color key
	DDCOLORKEY ddCK;
	ddCK.dwColorSpaceLowValue = 0;
	ddCK.dwColorSpaceHighValue = 0;
	if (lpddsBack->SetColorKey(DDCKEY_SRCBLT, &ddCK))
	{
		return E_FAIL;
	}

	// Update window flag
	SetClassLong(hWnd, GCL_HICONSM, NULL);
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, NULL);
	UpdateWindow(hWnd);

	return S_OK;
}

HRESULT GameDisplayLoop(HWND hWnd, PFPImage pImage, LONG destX, LONG destY)
{
	HRESULT hResult;
	DDBLTFX ddBltFx;
	memset(&ddBltFx, 0, sizeof(ddBltFx));
	ddBltFx.dwSize = sizeof(ddBltFx);
	ddBltFx.dwROP = SRCCOPY;

	RECT rcMain = { 0 };
	GetClientRect(hWnd, &rcMain);
	ClientToScreen(hWnd, ((LPPOINT)&rcMain) + 0);
	ClientToScreen(hWnd, ((LPPOINT)&rcMain) + 1);
	//FP_DEBUG_MSG(_T("CLINET RECT: (%d, %d) - (%d, %d)\n"), rcMain.top, rcMain.bottom, rcMain.left, rcMain.right);

	RECT rcBack = { 0 };
	DDSURFACEDESC2 ddsd;
	DDRAW_INIT_STRUCT(ddsd);
	if (FAILED(hResult = lpddsBack->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
	{
		FP_DEBUG_MSG(_T("Lock Error: 0x%08x\n"), hResult);
		return E_FAIL;
	}
	LPPALETTEENTRY lpPalet = NULL;
	if (FAILED(gameGraphics->ChangePalette(FP_PALETTE_DAY, &lpPalet)))
	{
		return E_FAIL;
	}
	LPBYTE lpPixel = (LPBYTE)ddsd.lpSurface;
	LPBYTE lpData = (LPBYTE)pImage->data;
	LONG n = 0;
	for (LONG i = 0; i < pImage->height; i++)
	{
		n = (i + destY) * ddsd.lPitch + destX * sizeof(PALETTEENTRY);
		for (LONG j = 0; j < pImage->width; j++, n += 4)
		{
			LPBYTE lpPtr = &lpPixel[n];
			*(lpPtr + 0) = lpPalet[lpData[(pImage->height - i - 1) * pImage->width + j]].peRed;
			*(lpPtr + 1) = lpPalet[lpData[(pImage->height - i - 1) * pImage->width + j]].peGreen;
			*(lpPtr + 2) = lpPalet[lpData[(pImage->height - i - 1) * pImage->width + j]].peBlue;
			*(lpPtr + 3) = 0;
		}
	}
	lpddsBack->Unlock(NULL);

	lpddsMain->Blt(&rcMain, lpddsBack, NULL, DDBLT_WAIT, &ddBltFx);
	return S_OK;
}