#pragma once
#include "Actor.h"

class Saucer : public Actor {
private:
	float cooldown;
	bool isBig;
public:
	Saucer(sf::Vector2f pos, bool isBig);
	void Update(float time, std::vector<Actor*>& actors) override;
	bool CanShoot();
	void Destroy(bool playerDestroy = false) override;
	bool IsBigSaucer() const;
	Shot GetShot(sf::Vector2f playerPos);
};