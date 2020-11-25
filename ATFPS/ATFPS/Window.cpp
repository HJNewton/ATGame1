#define no_init_all

#include "Window.h"
#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept // Constructor
	:
	hInst(GetModuleHandle(nullptr)) // Gets handle to instance
{
	WNDCLASSEX wc = { 0 }; // Good practice to zero out a config structure before filling it
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMSGSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc); // Registers the class by passing a pointer
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

// Window Stuff
Window::Window(int width, int height, const char* name) noexcept 
{
	// Calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	// Create window & get hWnd
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	// Create graphics object
	pGFX = std::make_unique<Graphics>(hWnd);

	// Register mouse 
	//RAWINPUTDEVICE rid;
	//rid.usUsagePage = 0x01; // Mouse page
	//rid.usUsage = 0x02; // Mouse usage
	//rid.dwFlags = 0;
	//rid.hwndTarget = nullptr;	
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

Graphics & Window::GFX()
{
	return *pGFX;
}

LRESULT WINAPI Window::HandleMSGSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Use create paramter passed from CreateWindow() to store window class pointer
	if (msg == WM_NCCREATE)
	{
		// Extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// Set WinAPI-managed iser data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// Set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMSGFunc));
		// Forward message to window class handler
		return pWnd->HandleMSG(hWnd, msg, wParam, lParam);
	}

	// If we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMSGFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// Forward message to window class handler
	return pWnd->HandleMSG(hWnd, msg, wParam, lParam);
}

void Window::EnableCursor() noexcept
{
	cursorEnabled = true;
	ShowCursor();
	FreeCursor();
}

void Window::DisableCursor() noexcept
{
	cursorEnabled = false;
	HideCursor();
	ConfineCursor();
}

void Window::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}

void Window::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}

void Window::ConfineCursor() noexcept
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void Window::FreeCursor() noexcept
{
	ClipCursor(nullptr);
}

bool Window::CursorEnabled() const noexcept
{
	return cursorEnabled;
}

LRESULT Window::HandleMSG(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept // The new message handler, contains Keyboard inputs in my case because I can't be bothered to make keyboard class atm
{
	switch (msg)
	{
		case WM_CLOSE:
			PostQuitMessage(27); // Will return 27 if the window exits successfully 
			return 0;
		case WM_KEYDOWN: // The functionality for KeyDown
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
			break;
		case WM_KEYUP: // The functionality for KeyUp
			kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		case WM_CHAR: // Gets the currently typed character essentially
			kbd.OnChar(static_cast<unsigned char>(wParam));
			break;
		case WM_LBUTTONDOWN: // Functionality for left mouse button down
			{
				POINTS pt = MAKEPOINTS(lParam);
				std::ostringstream oss;
				oss << "(" << pt.x << "," << pt.y << ")"; // Get's the X and Y coords of the mouse in the window when left mouse is pressed
				SetWindowText(hWnd, oss.str().c_str()); // Set window title to current mouse coords

				if (!cursorEnabled) // Reconfines the cursor if you tab back in and click
				{
					ConfineCursor();
					HideCursor();
				}
		}
		return 0;
		case WM_ACTIVATE:
			if (!cursorEnabled) 
			{
				if (wParam & WA_ACTIVE) // Confines cursor to screen
				{
					ConfineCursor();
					HideCursor();
				}
				else
				{
					FreeCursor();
					ShowCursor();
				}
			}
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return{};
}

