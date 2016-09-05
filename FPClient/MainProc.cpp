// FPClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FPClient.h"

HRESULT SwitchDisplayMode(HWND hWnd);

// Functions below are just for testing
HRESULT TestPrintDebugMsg(WPARAM wParam, LPARAM lParam);
HRESULT TestIORequest();

LRESULT CALLBACK MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_TOGGLEDISPLAYMODE:
			// Toggle the fullscreen/window mode
			SwitchDisplayMode(hWnd);
			break;
		case IDM_USERTRIGGERESCAPE:
			TestIORequest();
			break;
		}
		break;

	case FPMSG_IO_READ_GRAPHICDATA:
	case FPMSG_IO_READ_ANIMEDATA:
		// Being called when IO data is back
		gameGraphics->IODataBack(message, wParam, (LPVOID)lParam);
		break;

	case FPMSG_WINDOW_DEBUG_MSG:
		TestPrintDebugMsg(wParam, lParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;

	case WM_SIZE:
		// Check to see if we are losing our window...
		if (SIZE_MAXHIDE == wParam || SIZE_MINIMIZED == wParam)
			fActive = FALSE;
		else
			fActive = TRUE;
		break;

	case WM_CLOSE:
		if (IDYES == MessageBox(hWnd, _T("确定要退出？"), _T("提示"), MB_ICONWARNING | MB_YESNO))
		{
			DestroyGameDisplay();
			DestroyWindow(hWnd);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HRESULT SwitchDisplayMode(HWND hWnd)
{
	fWindowed = !fWindowed;
	if (FAILED(InitGameDisplay(fWindowed)))
	{
		FP_DEBUG_MSG(_T("Failed to switch display mode!\n"));
		MessageBox(hWnd, _T("Failed to switch display mode!"), _T("Error"), MB_ICONERROR | MB_OK);
		// Back to original mode
		fWindowed = !fWindowed;
		if (FAILED(InitGameDisplay(fWindowed)))
		{
			FP_DEBUG_MSG(_T("Failed to switch display mode again!\n"));
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			FP_DEBUG_MSG(_T("The game process is forced to quit!\n"));
		}
		MessageBox(hWnd, _T("Switched to former display mode."), _T("Error"), MB_ICONWARNING | MB_OK);
	}
	return S_OK;
}

HRESULT TestPrintDebugMsg(WPARAM wParam, LPARAM lParam)
{
	/*Test switch palette
	FP_DEBUG_MSG(_T("Palette %d Data:"), wParam);
	for (LONG i = 0; i < FP_STORE_PAL_OPTIONAL; i++)
	{
		if (i % 4 == 0)
		{
			FP_DEBUG_MSG(_T("\n"));
		}
		FP_DEBUG_MSG(_T("%08x "), ((PALETTEENTRY*)lParam)[i]);
	}
	FP_DEBUG_MSG(_T("\n"));
	//*/

	//*Test Image IO Request
	FP_DEBUG_MSG(_T("Image %d Attribute: \n"), wParam);
	PFPImage pImage = (PFPImage)lParam;
	FP_DEBUG_MSG(_T("\tWidth:%d Height: %d \n"), pImage->width, pImage->height);
	FP_DEBUG_MSG(_T("Binary Data:"), wParam);
	LONG size = (pImage->width * pImage->height) / 4;
	for (LONG i = 0; i < size; i++)
	{
		if (i % 8 == 0)
		{
			FP_DEBUG_MSG(_T("\n"));
		}
		FP_DEBUG_MSG(_T("%08x "), pImage->data[i]);
	}
	FP_DEBUG_MSG(_T("\n"));
	//*/

	return S_OK;
}

HRESULT TestIORequest()
{
	/*Test switch palette
	LONG index = gameGraphics->GetCurrentPaletteIndex();
	const PALETTEENTRY *pPalet;
	index++;
	if (index >= FP_FILE_COUNT_PAL)
	{
		index = 0; 
	}
	gameGraphics->ChangePalette(index, &pPalet); //测试IO
	//*/

	//*Test Image IO Request
	PFPImage pImage = NULL;
	gameGraphics->GetImage(1234, &pImage);
	if (pImage != NULL)
	{
		PostMessage(hMainWnd, FPMSG_WINDOW_DEBUG_MSG, 0, (LPARAM)pImage);
	}
	//*/

	return S_OK;
}
