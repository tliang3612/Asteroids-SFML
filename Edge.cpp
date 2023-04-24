#include "Edge.h"

Edge::Edge(sf::Vector2f A, sf::Vector2f B) :
		body(sf::Vector2f(1.5f, Line::Distance(A, B))),
		line(A, B),
		offsetA(std::move(A)),
		offsetB(std::move(B))
{
	RotateBody();
	body.setPosition(line.pointA);
	body.setOrigin(0.0f, 0);
	body.setFillColor(sf::Color::White);
}

const sf::RectangleShape& Edge::GetBody() const {return body;}

const Line& Edge::GetLine() const {return line;}

void Edge::AddOffset(float deltaTime){
	offsetA *= 1.f + deltaTime * 0.3f;
	offsetB *= 1.f + deltaTime * 0.3f;
}

void Edge::ResetOffset(const VectorPair& pair){
	offsetA = pair.first;
	offsetB = pair.second;
}

void Edge::RotateBody() {
	double angle = line.rotation;
	angle *= -(180.0f / PI);
	if (line.pointA.x > line.pointB.x) {
		angle *= -1.0f;
	}
	body.setRotation(angle);
}

void Edge::Move(sf::Vector2f destination){
	line.pointA = destination + offsetA;
	line.pointB = destination + offsetB;
	RotateBody();
	body.setPosition(line.pointA);
}

void Edge::RotateAround(const sf::Vector2f& anchor, float angle){
	RotateVector(line.pointA, angle, anchor);
	RotateVector(line.pointB, angle, anchor);
	RotateVector(offsetA, angle);
	RotateVector(offsetB, angle);
	line.CalculateMidPoint();
	line.CalculateRotation();
	RotateBody();
	body.setPosition(line.pointA);
}
