#include "stdafx.h"
#pragma once
#include "Functions.h"

class Explosion{
private:
	std::vector<sf::CircleShape> sparks;
	std::vector<sf::Vector2f>	 offsets;
	float lifeTime;
public:
	Explosion(sf::Vector2f);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& w);
	bool isAlive();
};

