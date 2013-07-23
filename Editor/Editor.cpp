//////////////////////////////////////////////////////////////////////

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
Quad				quad;
Texture				texture;
vector<Line>		lines;
Font				font;

uint				topLine;
uint				screenLines;
uint				maxLine;
bool				scroll = false;
int					scrollDirection;

//////////////////////////////////////////////////////////////////////

ATOM				RegisterWindowClass(HINSTANCE hInstance);
bool				CreateWindowInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

bool				HandleMessages();
void				Render();
void				ScrollDown();
void				ScrollUp();

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

	screenLines = (d3d.Height() + font.Height() - 1) / font.Height();

	TRACE(TEXT("%d\n"), screenLines);

	quad.Create(d3d.GetDevice(), 2);

	size_t fileSize;
	byte *file = LoadFile(TEXT("Editor.cpp"), &fileSize);
	byte *ptr = file;
	char const *p = (char *)ptr;

	// scan the text file for the lines
	lines.clear();
	uint index = 0;
	while(*p)
	{
		Line l;
		p = l.Initialize(p, index++);
		lines.push_back(l);
	}

	texture.Create(d3d.Width(), d3d.Height());

	topLine = 0;
	maxLine = lines.size() - screenLines;

	// draw the initial page of lines into the texture
	for(uint i=0; i<min(screenLines, lines.size()); ++i)
	{
		lines[i + topLine].Render(texture, ((i + topLine) % screenLines) * font.Height(), font, i + topLine);
	}

	Render();

	while(HandleMessages())
	{
		if(scroll)
		{
			if(scrollDirection == -1)
			{
				ScrollUp();
			}
			else
			{
				ScrollDown();
			}
		}
		Render();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////

bool HandleMessages()
{
	MSG msg;

	if(!scroll)
	{
		WaitMessage();
	}
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

void Render()
{
	d3d.Clear(0x00402000);
	d3d.SetupRenderState();

	texture.Activate();

	int numQuads = 1;

	float vScale = 1.0f / screenLines;

	// draw the top part
	int tline = topLine % screenLines;
	int height = screenLines - tline;
	int remain = height % screenLines;

	float u = 0;
	float v = vScale * tline;

	float h = (float)font.Height() * height;

	//quad.Set(0, 0, 0, (float)d3d.Width(), (float)d3d.Height(), 0, 0, 1, 1, 0xffff00ff);
	quad.Set(0, 0, 0, (float)d3d.Width(), h, 0, v, 1, 1, 0xffffffff);

	if(remain > 0)
	{
		quad.Set(1, 0, h, (float)d3d.Width(), screenLines * font.Height() - h, 0, 0, 1, v, 0xff00ffff);
		++numQuads;
	}

	quad.Draw(numQuads);
	d3d.Present();
}

//////////////////////////////////////////////////////////////////////

void ScrollDown()
{
	if(topLine > 0)
	{
		scrollDirection = 1;
		--topLine;
		int tLine = (topLine + screenLines) % screenLines;
		lines[topLine].Render(texture, tLine * font.Height(), font, topLine);
	}
	else
	{
		scroll = false;
	}
}

//////////////////////////////////////////////////////////////////////

void ScrollUp()
{
	if(topLine < maxLine)
	{
		scrollDirection = -1;
		int tLine = topLine % screenLines;
		lines[topLine + screenLines].Render(texture, tLine * font.Height(), font, topLine);
		++topLine;
	}
	else
	{
		scroll = false;
	}
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

	case WM_KEYDOWN:
		{
			//TRACE(TEXT("%d\n"), lParam & 0x40000000);
			if(lParam & 0x40000000)
			{
				scroll = true;
			}
			if(!scroll)
			{
				switch(wParam)
				{
				case VK_DOWN:
					ScrollUp();
					break;

				case VK_UP:
					ScrollDown();
					break;
				}
			}
		}
		break;

	case WM_KEYUP:
		scroll = false;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		{
			texture.Destroy();
			quad.Destroy();
			d3d.Close();
			PostQuitMessage(0);
		}
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
