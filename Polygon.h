#pragma once
#include "Edge.h"

class Polygon{
private:
	std::vector<Edge> edges;
	std::vector<sf::Vector2f> vertices;
	sf::Vector2f position;
	float minX;
	float maxX;
	float radius;
	float rotation;
public:
	Polygon();
	Polygon(const sf::Vector2f& pos_, const std::vector<VectorPair>& pairs);
	void Draw(sf::RenderWindow& w) const;
	void Move(sf::Vector2f dest);
	void Rotate(float angle);
	void Explode(float deltaTime);
	void Reset(const sf::Vector2f& pos_, const std::vector<VectorPair>& pairs);

	float GetRotation() const;
	float GetRadius() const;
	const std::vector<Edge>& GetEdges() const;
	bool HasCollisionWith(const Line& line) const;
	bool IsPointInPolygon(sf::Vector2f& point) const;
};