#include "Saucer.h"

Saucer::Saucer(sf::Vector2f pos, bool big) : 
		Actor(pos, sf::Vector2f(0.0f, 0.0f), big ? BIG_SAUCER_PATTERN : SMALL_SAUCER_PATTERN),
		isBig(big),
		cooldown(0.0f)
{
	speed = big ? 150.f : 300.f;
	direction = (position.x > WIDTH / 2) ? sf::Vector2f(-1.9f, 0.9f) : sf::Vector2f( 1.9f, 0.9f);
	LoopSound(big ? Sound::SOUC_B : Sound::SOUC_S);
}

void Saucer::Update(float time, std::vector<Actor*>& actors){
	if (!actors.empty()) {
		float yDist = std::abs(actors.at(0)->GetPosition().y - position.y);
		if (yDist > HEIGHT * 0.2f) {
			if (actors.at(0)->GetPosition().y > position.y) 
				direction.y = 1.f;
			else 
				direction.y = -1.f;
		}
		else{
			direction.y = 0.f;
		}
	}
	if (IsPassingVerticalBorder(position + direction)) {
		direction.x *= -1.f;
		position.x += direction.x * 10.f;
	}
	Actor::Update(time, actors);
	cooldown += time;
}

bool Saucer::CanShoot(){
	if (cooldown >= 1.0f) {
		cooldown = 0.0f;
		return true;
	}
	return false;
}

void Saucer::Destroy(bool playerDestroy){
	EndSoundLoop(isBig ? Sound::SOUC_B : Sound::SOUC_S);
	Actor::Destroy(playerDestroy);
}

bool Saucer::IsBigSaucer() const {return isBig;}

Shot Saucer::GetShot(sf::Vector2f playerPos){
	if (isBig) {
		return Shot(position, RAND_ANGLE_360(gen));
	}
	float angle = Line::Rotation(playerPos, position) + RAND_DISPERSION(gen);
	if (playerPos.x < position.x) angle *= -1.f;
	return Shot(position, angle);
}