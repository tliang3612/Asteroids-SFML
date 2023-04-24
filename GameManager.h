#pragma once
#include "Bullet.h"
#include "Player.h"
#include "Saucer.h"
#include "Explosion.h"

class GameManager {
private:
	std::vector<Bullet> bullets;
	std::vector<Actor*> actors;

	std::queue<Shot> shots;
	std::queue<sf::Vector2f> explosions;
	std::vector<Explosion> VFX;

	mutable std::mutex gmMutex;

	Player* player;
	Saucer* saucer;

	int score = 0;
	bool saucerSpawned;
	bool gameRunning;

public:
	GameManager();
	~GameManager() = default;

	Player& GetPlayer() const;
	int GetScore() const;
	int GetPlayerLives() const;
	bool IsPlayerAlive() const;

	void StartGame();
	void Shoot();
	void Update(float time);
	void Fire(Shot sho, float pushDist, bool playerShoot = false);
	void GenerateAsteroids(float deltaTime, float waitTime);
	void SpawnSaucer(float deltaTime);
	void CrackAsteroid(const sf::Vector2f& pos, int state);
	void Draw(sf::RenderWindow& window);
	void UpdateScore(Values val);
	void Clear();

	bool IsExplosionsActive() const;
	const sf::Vector2f& PopExplosion();
};
