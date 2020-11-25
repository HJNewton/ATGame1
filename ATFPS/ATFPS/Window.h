#pragma once

#include <Windows.h>
#include <sstream>
#include <string>
#include <iostream>
#include "Mouse.h"
#include "Keyboard.h"
#include "Graphics.h"
#include <optional>
#include <memory>


// THE CLASS FOR INITIALISING AND CREATING WINDOWS

class Window
{
	private:
		class WindowClass
		{
		public:
			static const char* GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;
		private:
			WindowClass() noexcept;
			~WindowClass();
			WindowClass(const WindowClass&) = delete;
			WindowClass& operator = (const WindowClass&) = delete;
			static constexpr const char* wndClassName = "ATFPS Window";
			static WindowClass wndClass;
			HINSTANCE hInst;
		};

	public:
		Window(int width, int height, const char* name) noexcept;
		~Window();
		Window(const Window&) = delete;
		Window& operator = (const Window&) = delete;
		Graphics& GFX();
		static std::optional<int> ProcessMessages();
		void EnableCursor() noexcept;
		void DisableCursor() noexcept;
		bool CursorEnabled() const noexcept;

	private:
		void HideCursor() noexcept;
		void ShowCursor() noexcept;
		void ConfineCursor() noexcept;
		void FreeCursor() noexcept;
		static LRESULT CALLBACK HandleMSGSetup(HWND hWNd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMSGFunc(HWND hWNd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMSG(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	public:
		Keyboard kbd;
		Mouse mouse;

	private:
		bool cursorEnabled = false;
		int width;
		int height;
		HWND hWnd; // Handle to the window
		std::unique_ptr<Graphics> pGFX;
		//std::vector<BYTE> rawBuffer;
};
