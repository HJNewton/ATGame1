#pragma once
#include "Window.h"
#include "Camera.h"
#include "Timer.h"

class Game
{
	public:
		Game();
		int Start();
		~Game();
	private:
		void DoFrame();
	private:
		Window window;
		Timer timer;
		float speed_factor = 1.0f;
		Camera cam;
		std::vector<std::unique_ptr<class Box>> boxes;
		std::vector<std::unique_ptr<class TexturedCube>> texturedCube;

		std::vector<std::unique_ptr<class Drawable>> drawables;
		static constexpr size_t nDrawables = 180;
};