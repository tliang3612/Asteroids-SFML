#include "Line.h"

///line object is not meant to be rendered, just for calculations
Line::Line(sf::Vector2f A, sf::Vector2f B) :
		pointA(std::move(A)),
		pointB(std::move(B))
{
	CalculateMidPoint();
	CalculateRotation();
	length = sqrt(pow(midX, 2) + pow(midY, 2));
}

void Line::CalculateRotation() {
	rotation = acos(midPoint.y / VectorsModule(midPoint));
}

void Line::CalculateMidPoint(){
	midX = pointB.x - pointA.x;
	midY = pointB.y - pointA.y;
	midPoint.x = midX;
	midPoint.y = midY;
}

float Line::Distance(const sf::Vector2f& pointA, const sf::Vector2f& pointB){
	float midX = pointB.x - pointA.x;
	float midY = pointB.y - pointA.y;
	return sqrt(pow(midX, 2) + pow(midY, 2));
}

float Line::Rotation(const sf::Vector2f& pointA, const sf::Vector2f& pointB){
	float midX = pointB.x - pointA.x;
	float midY = pointB.y - pointA.y;
	return acos(midY / VectorsModule(sf::Vector2f(midX, midY)));
}