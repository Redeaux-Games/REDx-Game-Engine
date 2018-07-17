#include <Core/Game.h>
#include <Core/Application.h>
#include <Engine/Graphics/API/Context.h>


namespace NuclearEngine {
	namespace Core {
		Game::Game()
		{
			gameinfo = nullptr;
		}
		Game::Game(GameInfo * yourinfo)
		{
			gameinfo = yourinfo;
		}
		Game::~Game()
		{
		}
		GameInfo * Game::GetGameInfo()
		{
			return gameinfo;
		}
		void Game::Initialize()
		{
		}
		
		void Game::Load()
		{
		}
		void Game::Update(float Deltatime)
		{
		}

		void Game::Render(float Deltatime)
		{
		}
		void Game::ExitRendering()
		{
		}
		void Game::Shutdown()
		{
		}
		void Game::LoadFinishUp()
		{
			Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);
			Core::Application::SetMouseInputMode(Core::MouseInputMode::Virtual);
			Core::Application::Display();
		}
		void Game::OnMouseScroll(double val1, double val2)
		{
		}
		void Game::OnMouseMovement(int xpos, int ypos)
		{
		}

		void Game::OnWindowResize(int width, int height)
		{
		}

		void Game::SetActiveScene(ECS::Scene * scene)
		{
			mScene = scene;
		}

		ECS::Scene * Game::GetActiveScene()
		{
			return mScene;
		}
	
	}
}