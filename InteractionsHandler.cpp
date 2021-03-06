#include "InteractionsHandler.hpp"
#include "Enemy.hpp"
#include "PlayerShip.hpp"
#include "Boss.hpp"

namespace rstar
{
	void InteractionsHandler::PlayerAndEnemies(Enemies &e, PlayerShip &ship, unsigned pointsForEnemy, float enemiesChanceToShoot)
	{
		bullets(e, ship, pointsForEnemy);
		bodiesIntersection(e, ship, pointsForEnemy);
		enemiesShooting(e, ship, enemiesChanceToShoot);
	}

	void InteractionsHandler::PlayerAndPowerUp(PlayerShip& ship, PowerUpShip* powerUp)
	{
		if (powerUp)
		{
			powerUpHit(ship, powerUp);
		}
	}

	void InteractionsHandler::PlayerAndBoss(PlayerShip& ship, Boss* boss, unsigned pointsForBoss)
	{
		if (boss)
		{
			bullets(ship, boss, pointsForBoss);
			bodiesIntersection(ship, boss);
		}
	}


	void InteractionsHandler::bullets(Enemies &e, PlayerShip &ship, unsigned pointsForEnemy)
	{
		for (auto &enemy : e.enemies_)
		{
			if (ship.bullet_ &&
				!enemy->IsDestroyed() && ship.bullet_->GetBounds().intersects(enemy->GetBounds()))
			{
				ship.bullet_.reset(nullptr);

				if (--enemy->lives_ == 0)
				{
					enemy->isDestroyed_ = true;
					enemy->isCharging_ = false;
					ship.score_ += pointsForEnemy;
				}
				else
				{
					enemy->hit_ = true;
				}
			}

		}

		e.bullets_.erase(std::remove_if(begin(e.bullets_), end(e.bullets_),
			[&](auto &bullet)
			{
				auto hit = bullet->GetBounds().intersects(ship.GetBounds());
				if (ship.GetLives() != 0 && hit)
				{
					ship.hit_ = true;
					ship.currentHitTexture_ = 0;
					--ship.lives_;
				}

				return hit;
			}
		), end(e.bullets_));
		
	}

	void InteractionsHandler::bullets(PlayerShip& ship, Boss* boss, unsigned pointsForBoss)
	{
		if (ship.bullet_ && !boss->IsDestroyed()
			&& ship.bullet_->GetBounds().intersects(boss->GetBounds()))
		{
			ship.bullet_.reset(nullptr);
			if (--boss->lives_ == 0)
			{
				boss->isDestroyed_ = true;
				boss->isCharging_ = false;
				ship.score_ += pointsForBoss;
			}
			else
			{
				boss->hit_ = true;
				boss->hpBarAnimationFrame_ = 0;
			}
		}

		boss->bullets_.erase(std::remove_if(begin(boss->bullets_), end(boss->bullets_),
			[&](auto &bullet)
			{
				auto hit = bullet->GetBounds().intersects(ship.GetBounds());
				if (ship.GetLives() != 0 && hit)
				{
					ship.hit_ = true;
					ship.currentHitTexture_ = 0;
					ship.lives_ = BOSS_BULLETS_POWER > ship.lives_ ? 0 : ship.lives_ - BOSS_BULLETS_POWER;
				}

				return hit;
			}
		), end(boss->bullets_));

	}

	void InteractionsHandler::bodiesIntersection(Enemies &e, PlayerShip &ship, unsigned pointsForEnemy)
	{
		for (auto &enemy : e.enemies_)
		{
			if (!enemy->IsDestroyed() && enemy->GetBounds().intersects(ship.GetBounds()))
			{
				if (--enemy->lives_ == 0)
				{
					enemy->isDestroyed_ = true;
					enemy->isCharging_ = false;
				}
				else
				{
					enemy->hit_ = true;
				}
				ship.hit_ = true;
				--ship.lives_;
			}

			if (enemy->IsOutOfScreen())
			{
				enemy->toRemove_ = true;
				ship.score_ -= pointsForEnemy;
			}
		}
	}
		
	void InteractionsHandler::bodiesIntersection(PlayerShip& ship, Boss* boss)
	{
		if (!boss->IsDestroyed() && boss->GetBounds().intersects(ship.GetBounds()))
		{
			if(--boss->lives_ == 0)
			{
				boss->isDestroyed_ = true;
				boss->isCharging_ = false;
			}

			ship.hit_ = true;
			--ship.lives_;
		}
	}

	


	void InteractionsHandler::enemiesShooting(Enemies &e, PlayerShip& ship, float chanceToShoot)
	{
		for (auto &enemy : e.enemies_)
		{
			if (!enemy->IsDestroyed()
				&& e.lvlClockRef_.getElapsedTime().asSeconds() > ENEMIES_START_SHOOT_DELAY
				&& e.lvlClockRef_.getElapsedTime().asSeconds() - e.shotDelayTimeOffset_ > ENEMIES_SHOT_DELAY
				&& abs(enemy->GetPosition().x - enemy->GetBounds().width - ship.GetPosition().x) < ENEMIES_WIDTH)
			{
				if (Random<float>(0, 100) <= chanceToShoot)
				{
					e.Shoot(sf::Vector2f{ enemy->GetPosition().x - enemy->GetBounds().width / 2.f, enemy->GetPosition().y });
					e.shotDelayTimeOffset_ = e.lvlClockRef_.getElapsedTime().asSeconds();
				}
			}
		}
	}

	void InteractionsHandler::powerUpHit(PlayerShip& ship, PowerUpShip* powerUp)
	{
		if (ship.bullet_)
		{
			if (ship.bullet_->GetBounds().intersects(powerUp->GetBounds()))
			{
				powerUp->PowerUp(ship);
				powerUp->isDestroyed_ = true;
				ship.bullet_.reset(nullptr);
			}
		}
	}

}
