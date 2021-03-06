#pragma once
#include "Bullet.hpp"
#include "Game.hpp"
#include "PlayerShip.hpp"
#include "Animatable.hpp"

namespace rstar
{
	class Enemy : public Animatable
	{
	public:
		Enemy(GameDataPtr data, std::vector<sf::Texture> const& textures, sf::Vector2f startPosition, unsigned lives,
			float frameTime, sf::Clock const& clock);

		bool IsDestroyed() const { return isDestroyed_; }
		bool IsToRemove() const { return toRemove_; }
		bool IsCharging() const { return isCharging_; }
		unsigned GetLives() const { return lives_; }
		virtual void Charge() { isCharging_ = true; }

		void Update() override;
		void Draw() const override;

		friend class InteractionsHandler;

	protected:
		unsigned lives_;

		bool isDestroyed_{ false };
		bool toRemove_{ false };
		bool isCharging_{ false };
		bool hit_{ false };

		float destroyAnimationTimeOffset_{ 0.f };

		unsigned short currentDestroyTexture_{ 0 };

		void animateDestroy();
		void updateTextures();
		virtual void handleMovement();
	};

	class Enemies
	{
	public:
		Enemies(GameDataPtr data, unsigned enemiesCount, float movementSpeed,
			float bulletsSpeed, float chargingSpeed, unsigned enemiesCharging,
			unsigned enemiesLives, sf::Vector2f firstEnemyPos, float space, sf::Clock const& lvlClockRef);

		unsigned int GetEnemiesCount() const { return enemiesCount_; }
		void Shoot(sf::Vector2f const& startPosition);

		void Update();
		void Draw() const;

		static DirectionX MoveDirection;
		static float MovementSpeed;
		static bool MoveForward;
		static float BulletsSpeed;
		static float ChargingSpeed;

		friend class InteractionsHandler;

	private:
		GameDataPtr data_;
		sf::Clock const& lvlClockRef_;

		std::vector<std::unique_ptr<Enemy>> enemies_;
		std::vector<std::unique_ptr<Bullet>> bullets_;
		unsigned enemiesCount_;

		const unsigned enemiesCharging_;

		float moveForwardTimeOffset_{ 0.f };
		float shotDelayTimeOffset_{ 0.f };

		void removeDestroyedEnemies();
		void removeMissedBullets();
		void handleCharging(unsigned int enemiesCharging);
		void reorderEnemies();
	};
}

