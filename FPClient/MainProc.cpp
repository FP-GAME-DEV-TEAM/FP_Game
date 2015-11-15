// FPClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FPClient.h"

extern BOOL fActive;
extern BOOL fWindowed;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				MessageBox(hWnd, TEXT("Game_Init() failed!") TEXT("The sample will now exit."),
					TEXT("DirectDraw Sample"), MB_ICONERROR | MB_OK);
				PostMessage(hWnd, WM_CLOSE, 0, 0);
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
		if (IDYES == MessageBox(hWnd, "确定要退出？", "提示", MB_ICONWARNING | MB_YESNO))
		{
			DestroyWindow(hWnd);
		}
		break;

	case WM_DESTROY:
		DestroyGameDisplay();
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
