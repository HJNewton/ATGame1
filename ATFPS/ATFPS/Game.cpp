#include <iomanip>
#include <sstream>

#include "Game.h"
#include "Box.h"
#include "TexturedCube.h"
#include <memory>
#include "HarryMath.h"
#include "Surface.h"
#include "GDIPlusManager.h"

GDIPlusManager gdipm;

// http://www.abandonia.com/files/extras/23636_game_extra_1.pdf Control scheme based upon wolfenstein 3D manual

Game::Game():window(800,600, "ATFPS WINDOW")
{
	window.DisableCursor();

	/*std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 0.5f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.08f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

	for (auto i = 0; i < 75; i++)
	{
		texturedCube.push_back(std::make_unique<TexturedCube>(
			window.GFX(), rng, adist,
			ddist, odist, rdist
			));
	}

	texturedCube.push_back(std::make_unique<TexturedCube>(window.GFX(), rng, adist,ddist, odist, rdist));*/

	class Factory
	{
	public:
		Factory(Graphics& GFX)
			:
			gfx(GFX)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<TexturedCube>(
					gfx, rng, adist, ddist,
					odist, rdist);
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,0 };
	};

	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, Factory {window.GFX()});

	window.GFX().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 1000.0f)); // Final value changes render distance essentially
}

// Contains our message pump
int Game::Start()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}

		DoFrame();
	}
}

Game::~Game()
{}

// What should occur each frame
void Game::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;
	window.GFX().SetCam(cam.GetMatrix());
	window.GFX().ClearBuffer(0.07f, 0.0f, 0.12f);

	for (auto& d : drawables)
	{
		d->Update(dt);
		d->Draw(window.GFX());
	}

	if (!window.CursorEnabled())
	{
		if (window.kbd.KeyIsPressed('W')) // Walk forward with W
		{
			if(window.kbd.KeyIsPressed(VK_SHIFT)) // Sprint if shift is held
			{
				cam.Translate({ 0.0f,0.0f,dt * 1.65f });

			}

			cam.Translate({ 0.0f,0.0f,dt });
		}
		if (window.kbd.KeyIsPressed('S')) // Walk backwards with S
		{
			cam.Translate({ 0.0f,0.0f,-dt });
		}


		if (window.kbd.KeyIsPressed('A')) // Strafe left with A
		{
			cam.Translate({ -dt,0.0f,0.0f });
		}
		if (window.kbd.KeyIsPressed('D')) // Strafe right with D
		{
			cam.Translate({ dt,0.0f,0.0f });
		}


		if (window.kbd.KeyIsPressed(VK_LEFT)) // Rotate left with left arrow
		{
			if (window.kbd.KeyIsPressed(VK_CONTROL)) // Fast rotate left when holding right control and left arrow
			{
				cam.Rotate(-dt * 1.65f);
			}

			cam.Rotate(-dt);
		}
		if (window.kbd.KeyIsPressed(VK_RIGHT)) // Rotate right with right arrow
		{
			if (window.kbd.KeyIsPressed(VK_CONTROL)) // Fast rotate right when holding right control and right arrow
			{
				cam.Rotate(dt * 1.65f);
			}

			cam.Rotate(dt);
		}
	}

	window.GFX().EndFrame();
}