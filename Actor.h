#pragma once
#include "Polygon.h"

class Actor {
protected:
	Polygon body;
	sf::Vector2f position;
	sf::Vector2f direction;
	float speed;
	bool alive;
	bool killedByPlayer;
public:
	Actor(sf::Vector2f pos, sf::Vector2f dir, const std::vector<VectorPair>& pairs);

	virtual void Update(float time, std::vector<Actor*>& actors);
	bool CheckCollision(const std::vector<Actor*>& actors);
	virtual void Draw(sf::RenderWindow& w) const;
	virtual void Destroy(bool playerDestroy = false);
	
	const sf::Vector2f& GetPosition() const;
	const Polygon& GetBody() const;
	virtual bool IsAlive() const;
	bool IsDestroyedByPlayer() const;

	bool Collision(Polygon&a, Polygon& b);

};

