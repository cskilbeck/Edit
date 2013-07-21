//////////////////////////////////////////////////////////////////////
// Create one texture per line
// On resize resize all the textures
// File load (whole thing for now)
// Monitor keyboard
// If up/down held for key repeat interval, go into vsynced loop
// draw the textures into the backbuffer and flip
// circular buffer of texture pointers, refresh the one going round the loop with the right line of text

#include "stdafx.h"
#include "D3D.h"
#include "Editor.h"

//////////////////////////////////////////////////////////////////////

#define MAX_LOADSTRING 100

//////////////////////////////////////////////////////////////////////

HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HACCEL hAccelTable;

//////////////////////////////////////////////////////////////////////

D3D					d3d;
Texture				texture;
Font				font;

//////////////////////////////////////////////////////////////////////

ATOM				RegisterWindowClass(HINSTANCE hInstance);
bool				CreateWindowInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

bool				HandleMessages();
void				Render();

//////////////////////////////////////////////////////////////////////

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EDITOR, szWindowClass, MAX_LOADSTRING);

	RegisterWindowClass(hInstance);

	if (!CreateWindowInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITOR));

	font.Create(TEXT("Consolas"), 10);
	texture.Create(d3d.Width(), font.Height());

	HDC dc = texture.GetDC();
	if(dc != null)
	{
		HFONT oldFont = SelectFont(dc, font);
		SetBkMode(dc, TRANSPARENT);
		SetTextColor(dc, 0xffffff);
		TextOut(dc, 0, 0, TEXT("RegisterWindowClass"), 19);
		SelectFont(dc, oldFont);
		texture.ReleaseDC();
	}

	while(HandleMessages())
	{
		Render();
		d3d.Present();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////

bool HandleMessages()
{
	MSG msg;

	WaitMessage();
	while (PeekMessage(&msg, null, 0, 0, true))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
			{
				return false;
			}
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////

void Render()
{
	d3d.Clear(0x00402000);
	texture.Activate();
	d3d.DrawATriangle((float)texture.Width(), (float)font.Height());
}

//////////////////////////////////////////////////////////////////////

ATOM RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EDITOR));
	wcex.hCursor		= LoadCursor(null, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_EDITOR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//////////////////////////////////////////////////////////////////////

bool CreateWindowInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance;

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, null, null, hInstance, null);

   if (!hWnd)
   {
      return false;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		d3d.Open(hWnd);
		break;

	case WM_SIZE:
		d3d.Resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		d3d.Close();
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
