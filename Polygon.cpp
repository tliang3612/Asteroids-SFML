#include "Polygon.h"

Polygon::Polygon(const sf::Vector2f& pos, const std::vector<VectorPair>& pairs) :
		position(pos),
		rotation(0.0f),
		radius(0.0f)
{
	edges.reserve(pairs.size());

	for (auto pair : pairs) {

		radius = std::max(radius, std::abs(pair.first.x));
		radius = std::max(radius, std::abs(pair.first.y));
		radius = std::max(radius, std::abs(pair.second.x));
		radius = std::max(radius, std::abs(pair.second.y));

		edges.push_back(Edge(pair.first, pair.second));
	}

	for (auto& edge : edges) {
		edge.Move(position);
	}
}

void Polygon::Draw(sf::RenderWindow& window) const{
	for (auto edge : edges) {
		window.draw(edge.GetBody());
	}
	
}

void Polygon::Move(sf::Vector2f destination){
	sf::Vector2f diff = destination - position;
	position = destination;
	for (auto& edge : edges) {
		edge.Move(position);
	}
}

void Polygon::Rotate(float angle){
	if (angle != rotation) {
		for (auto& edge : edges) {
			edge.RotateAround(position, angle - rotation);
		}
		rotation = angle;
	}
}

///Point in polygon collision
bool Polygon::IsPointInPolygon(sf::Vector2f& point) const {
	int pointsOnRight = 0;

	//count how many times a raycast from the point hits an edge
	for (Edge edge : edges)
	{
		auto startPoint = edge.GetLine().pointA;
		auto endPoint = edge.GetLine().pointB;

		//check if the point within the range of the edge
		if ((startPoint.y >= point.y && endPoint.y < point.y) || (startPoint.y < point.y && endPoint.y >= point.y)) {

			float intersecX = startPoint.x + ((point.y - startPoint.y) * (endPoint.x - startPoint.x)) / (endPoint.y - startPoint.y);

			//check if the point hits the edge
			if (startPoint.x >= point.x && endPoint.x >= point.x || intersecX >= point.x) {
				++pointsOnRight;
			}
		}
	}

	//if the amount of times hit is odd, the point is in a polygon
	if (pointsOnRight % 2 == 1) {
		return true;
	}

	return false;
}


void Polygon::Explode(float deltaTime){
	for (auto& edge : edges) {
		edge.AddOffset(deltaTime * 2.0f); //exagerate the explosion a bit
	}
}

void Polygon::Reset(const sf::Vector2f& pos, const std::vector<VectorPair>& pairs){
	position = pos;
	rotation = 0.0f;
	if (pairs.size() != edges.size())
		return;

	for (unsigned i = 0; i < pairs.size(); ++i) {
		edges.at(i).ResetOffset(pairs.at(i));
		edges.at(i).Move(position);
		edges.at(i).RotateAround(position, rotation);
	}
}

float Polygon::GetRotation() const {return rotation;}

float Polygon::GetRadius() const {return radius;}

const std::vector<Edge>& Polygon::GetEdges() const {return edges;}


