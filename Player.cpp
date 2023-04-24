#include "Player.h"

Player::Player(sf::Vector2f pos, float currentRot) :
		Actor(pos, sf::Vector2f(0.f, 0.f), STARSHIP_PATTERN),
		flame(std::move(pos), FLAME_PATTERN),
		rotation(currentRot),
		rotSpeed(5.f),
		force(0.f),
		lives(3),
		rotateDirection(RotateDirection::NONE),
		canMove(true),
		thrustOn(false),
		moved(false)
{
	speed = 100.f;
	RotateUnitVector(direction, currentRot);
}

float Player::GetRotation() const { return rotation; }

int Player::GetLives() const { return lives; }

bool Player::CanMove() const { return canMove; }

void Player::Thrust(bool on){
	thrustOn = on;
	if (thrustOn) {
		RotateUnitVector(direction, rotation);
		force = PLAYER_THRUST_FORCE;
		LoopSound(Sound::THRUST);
	}
	else {
		EndSoundLoop(Sound::THRUST);
	}
}


void Player::Update(float deltaTime, std::vector<Actor*>& actors) {
	static float passedTime = 0.0f;
	if (canMove) {
		Move(deltaTime);
		Rotate(deltaTime);
		if (moved && CheckCollision(actors)) 
			Destroy();
	}
	else if ((passedTime += deltaTime) >= 1.5f) { //respawn after 1.5 delay
		passedTime = 0.0f;
		if (lives < 1) {
			alive = false;
			return;
		}
		Respawn();
	}
	else { //play explode animation
		body.Explode(deltaTime);
		body.Move(position);
	}
}

///Moves the player given a fixed time
void Player::Move(float deltaTime){
	if (force > 0.0f) {
		if (thrustOn) 
			RotateUnitVector(direction, rotation); //sets the direction based on rotation
		else 
			force -= 0.3f * deltaTime; //brake over time when not thrusting

		position += direction * force * speed * deltaTime;
		HandleScreenBorder(position);
		//move body and flame to calculated position
		body.Move(position);
		flame.Move(position);
		moved = true;
	}
}

///Rotates the player given a fixed time
void Player::Rotate(float deltaTime){
	if (rotateDirection != RotateDirection::NONE) {
		rotation += rotSpeed * deltaTime * (int)rotateDirection;
		body.Rotate(rotation);
		flame.Rotate(rotation);
		moved = true;
	}
}

void Player::SetRotationDirection(RotateDirection newDir){
	rotateDirection = newDir;
}

void Player::BonusLife(){
	++lives;
	PlaySound(Sound::EXTRA);
}

void Player::Respawn(){
	position = PLAYER_DEFAULT_POS;
	body.Reset(position, STARSHIP_PATTERN);
	flame.Rotate(0.0f);
	RotateUnitVector(direction, 0.0f);
	canMove = true;
	thrustOn = false;
	moved = false;
	rotateDirection = RotateDirection::NONE;
	rotation = 0.0f;
	force = 0.0f;
}

void Player::Reset(){
	lives = 3;
	alive = true;
	Respawn(); 
}

void Player::Draw(sf::RenderWindow& window) const {
	static bool flameDrawed = false;
	Actor::Draw(window);
	if (canMove) {
		if (thrustOn && !flameDrawed) 
			flame.Draw(window);
		flameDrawed = !flameDrawed;
	}
}

void Player::Destroy(bool playerDestroy){
	--lives;
	EndSoundLoop(Sound::THRUST);
	canMove = false;
}
