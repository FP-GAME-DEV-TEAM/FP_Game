// FPClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FPClient.h"

// Just for testing
void TestPrintPalette(LONG id, PALETTEENTRY *pArray);

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
			// Test switch palette
			const PALETTEENTRY *pPalet;
			if (fWindowed)
				gameGraphics->GetPalette(1, &pPalet); //测试IO
			else
				gameGraphics->GetPalette(2, &pPalet); //测试IO
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
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					FP_DEBUG_MSG(_T("The game process is forced to quit!\n"));
				}
				MessageBox(hWnd, _T("Switched to former display mode."), _T("Error"), MB_ICONWARNING | MB_OK);
			}
			return 0L;
		}
		break; // Continue with default processing

	case FPMSG_WINDOW_DEBUG_MSG:
		TestPrintPalette((LONG)wParam, (PALETTEENTRY *)lParam);
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

void TestPrintPalette(LONG id, PALETTEENTRY *pArray)
{
	FP_DEBUG_MSG(_T("Palette %d Data:"), id);
	for (int i = 0; i < FP_STORE_PAL_OPTIONAL; i++)
	{
		if (i % 4 == 0)
		{
			FP_DEBUG_MSG(_T("\n"));
		}
		FP_DEBUG_MSG(_T(" 0x%08x "), pArray[i]);
	}
	FP_DEBUG_MSG(_T("\n"));
}