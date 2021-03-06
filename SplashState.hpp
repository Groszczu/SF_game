#pragma once
#include "Game.hpp"

namespace rstar
{
	// first state allows player to start the game
	class SplashState final : public State
	{
	public:
		SplashState(GameDataPtr data);

		void HandleInput() override;
		void Update() override;
		void Draw() override;

	private:
		GameDataPtr data_;
		sf::Clock clock_;
		sf::Sprite background_;
		sf::Sprite startButton_;

		bool mouseOverButton_{ false };

		void handleButtonAnimation();
	};
}
