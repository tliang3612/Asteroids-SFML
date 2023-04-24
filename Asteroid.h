#pragma once
#include "Actor.h"

class Asteroid : public Actor {
private:
	int state;
	bool alive;
public:
	Asteroid(sf::Vector2f pos, sf::Vector2f dir, int state = 3);
	int GetState() const;
	Values GetValue() const;
};

