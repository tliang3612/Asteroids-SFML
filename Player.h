#pragma once
#include "Asteroid.h"

enum class RotateDirection{ NONE = 0, RIGHT = 1, LEFT = -1 };

class Player : public Actor {
private:
	Polygon flame;
	float rotation;
	float rotSpeed;
	float force;
	int lives;
	RotateDirection rotateDirection;
	bool canMove;
	bool thrustOn;
	bool moved;

public:
	Player(sf::Vector2f pos, float rotation);

	float GetRotation() const;
	int GetLives() const;
	bool CanMove() const;

	void Update(float time, std::vector<Actor*>& actors) override;
	void Move(float deltaTime);
	void Rotate(float deltaTime);
	void SetRotationDirection(RotateDirection newDir);
	void Thrust(bool on);
	void BonusLife();
	void Respawn();
	void Reset();
	void Draw(sf::RenderWindow& w) const override;
	void Destroy(bool playerDestroy = false) override;
};

