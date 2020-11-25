#include <Windows.h> // Must include Windows.h
#include <sstream>
#include <string>
#include "Window.h"
#include "Game.h"

int CALLBACK WinMain( // The default entry point for a windows program
	HINSTANCE hInstance, // Pointers to structs that hold info about our program and where it's loaded into memory
	HINSTANCE hPrevInstance, // Always going to be null, can ignore it
	LPSTR	  lpCmdLine, // Command line, this is where user commands are received
	int		  nCmdShow) // Indication to the program how the window should be shown, can ignore it
{
	return Game{}.Start();
}