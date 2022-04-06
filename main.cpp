#define UNICODE
#include <windows.h>

// The name of the window class.
#define WINDOW_CLASS (TEXT("=^.^="))

// The title of the window.
#define WINDOW_TITLE (TEXT("Hello, GCC!"))

// The style of the window.
#define WINDOW_STYLE (WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX))

// The window procedure callback.
static LRESULT CALLBACK window_procedure(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_CLOSE:
		case WM_KEYDOWN:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(wnd, msg, wp, lp);
}

extern "C" void WinMainCRTStartup()
{
	HINSTANCE instance;
	RECT      area;
	WNDCLASS  wclass;
	HWND      handle;
	MSG       msg;
	int       width  = 640;
	int       height = 480;

	// Get Application Instance
	instance = GetModuleHandle(nullptr);

	// Get Window Area
	area.left   = 0;
	area.top    = 0;
	area.right  = width;
	area.bottom = height;

	AdjustWindowRect(&area, WINDOW_STYLE, false);

	width  = area.right  - area.left;
	height = area.bottom - area.top;

	// Register Class
	wclass.style         = 0;
	wclass.lpfnWndProc   = window_procedure;
	wclass.cbClsExtra    = 0;
	wclass.cbWndExtra    = 0;
	wclass.hInstance     = instance;
	wclass.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
	wclass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wclass.hbrBackground = nullptr;
	wclass.lpszMenuName  = nullptr;
	wclass.lpszClassName = WINDOW_CLASS;

	if (!RegisterClass(&wclass))
		goto hell;

	// Create Window
	handle =
		CreateWindowEx(
			0,
			WINDOW_CLASS,
			WINDOW_TITLE,
			WINDOW_STYLE,
			(GetSystemMetrics(SM_CXSCREEN) - width)  >> 1,
			(GetSystemMetrics(SM_CYSCREEN) - height) >> 1,
			width,
			height,
			nullptr,
			nullptr,
			instance,
			nullptr
		);

	// Check Window
	if (handle == nullptr)
		goto hell;

	// Show Window
	ShowWindow(handle, SW_NORMAL);

	// Message Loop
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (msg.message == WM_QUIT)
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Success
	ExitProcess(0);

hell:
	// Failure
	MessageBoxA(nullptr, "failed to create a window somehow lol", nullptr, MB_ICONERROR);
	ExitProcess(1);
}