#pragma once
#include "Functions.h"

struct Line {
	sf::Vector2f pointA;
	sf::Vector2f pointB;
	sf::Vector2f midPoint;
	double rotation;
	float midX;
	float midY;
	float length;

	Line(sf::Vector2f A, sf::Vector2f B);

	void CalculateRotation();
	void CalculateMidPoint();

	static float Distance(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
	static float Rotation(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
};