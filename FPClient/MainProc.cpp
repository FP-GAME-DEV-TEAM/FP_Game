// FPClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FPClient.h"

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
			}
			return 0L;
		}
		break; // Continue with default processing

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
