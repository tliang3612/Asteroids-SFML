#pragma once
#include "Asteroid.h"

class Bullet {
private:
	sf::CircleShape body;
	sf::Vector2f position;
	sf::Vector2f direction;
	float speed;
	float timeAlive;
	float lifeTime;
	bool alive;
	bool isFromPlayer;

public:
	const sf::CircleShape& GetBody() const;
	bool IsAlive() const;
	bool IsPlayerOwned() const;

	Bullet(Shot shot, bool isPlayerShot = false);

	void CheckCollision(std::vector<Actor*>& actors);
	void Update(float t, std::vector<Actor*>& actors);
	void PushForward(float distance);
	void CheckIfAlive();
};