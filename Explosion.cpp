#include "Explosion.h"

Explosion::Explosion(sf::Vector2f p) :
		lifeTime(0.0f)
{
	sparks.reserve(NUM_OF_SPARKS);
	offsets.reserve(NUM_OF_SPARKS);

	for (unsigned i = 0; i < NUM_OF_SPARKS; ++i) {
		sparks.push_back(sf::CircleShape(1.5f));
		sparks.back().setFillColor(sf::Color::White);
		sparks.back().setPosition(p);
		offsets.push_back(sf::Vector2f(RAND_DIR(gen), RAND_DIR(gen)));
	}
}

void Explosion::Update(float deltaTime){
	for (unsigned i = 0; i < NUM_OF_SPARKS; ++i) {
		sparks.at(i).setPosition(sparks.at(i).getPosition() + offsets.at(i) * deltaTime * 350.0f);
		sparks.at(i).setFillColor(sf::Color(255, 255, 255, 255 - lifeTime * 170));
	}
	lifeTime += deltaTime;
}

void Explosion::Draw(sf::RenderWindow& window){
	for (auto& spark : sparks) {
		window.draw(spark);
	}
}

bool Explosion::isAlive(){
	if (lifeTime < SPARKS_LIFETIME) {
		return true;
	}
	return false;
}
