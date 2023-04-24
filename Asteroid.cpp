#include "Asteroid.h"

Asteroid::Asteroid(sf::Vector2f pos, sf::Vector2f dir, int state_) : 
		Actor(std::move(pos), std::move(dir), {}),
		state(state_),
		alive(true) 
{
	speed *= 3.f / state; //moves faster the smaller the asteroid
	std::vector<sf::Vector2f> points;	
	points.reserve(ASTEROID_MAX_SIZE);

	//generating points with random x offset, y == 0
	for (unsigned i = 0; i < ASTEROID_MAX_SIZE; ++i) {
		sf::Vector2f point(RAND_ASTEROID_RADIUS(gen) * state, 0);
		points.push_back(point);
	}

	float prevAngle = 0;
	//rotate all points around a random angle
	for (unsigned i = 1; i < ASTEROID_MAX_SIZE; ++i) {
		float angle = RAND_ANGLE_HALF_RAD(gen);
		RotateVector(points[i], prevAngle + angle);
		prevAngle += angle;
	}

	std::vector<VectorPair> pairs;
	pairs.reserve(ASTEROID_MAX_SIZE);

	//creates the polygon pattern based on the random points made
	for (unsigned i = 1; i < ASTEROID_MAX_SIZE; ++i) {
		pairs.push_back(VectorPair(points[i-1], points[i]));
	}
	
	//last pair connects the first and last point, making it closed
	pairs.push_back(VectorPair(points.back(), points.front()));

	body = Polygon(position, pairs);
}

int Asteroid::GetState() const {return state;}

Values Asteroid::GetValue() const {
	switch (state) {
		case 1: 
			return Values::ASTR_S;
		case 2: 
			return Values::ASTR_M;
		case 3: 
			return Values::ASTR_B;
		default:
			return Values::ASTR_S;
	}
}
