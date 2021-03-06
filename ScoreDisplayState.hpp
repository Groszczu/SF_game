#pragma once
#include "State.hpp"
#include "Game.hpp"

namespace rstar
{
	class ScoreDisplayState : public State
	{
	public:
		ScoreDisplayState(GameDataPtr data, int playerScore, std::string fileName);

		void HandleInput() override;
		void Update() override;
		void Draw() override;

	private:
		GameDataPtr data_;
		int const playerScore_;
		std::string const fileName_;
		
		std::string playerNick_{};

		sf::Text playerNickTxt_;
		sf::Text playerScoreTxt_;
		sf::Text scoreTableTxt_;
		sf::Text pressEnterTxt_;

		sf::Sprite background_;

		std::vector<std::pair<std::string, int>> scoreTable_;

		sf::Clock animationClock_;

		bool nameEntered_{ false };
		bool scoreCalculated_{ false };

		void loadScores();
		void writeScores() const;
		void generateScoreTable();
		void animateTxt();
	};
}
