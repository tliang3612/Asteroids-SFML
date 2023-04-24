#include "Actor.h"

Actor::Actor(sf::Vector2f pos, sf::Vector2f dir, const std::vector<VectorPair>& pairs) :
		body(position, pairs),
		position(std::move(pos)),
		direction(std::move(dir)),
		speed(DEFAULT_ACTOR_SPEED),
		alive(true),
		killedByPlayer(false)
	    //debugText("test", GetFont())
{
}

void Actor::Update(float time, std::vector<Actor*>& actors) {
	position += direction * speed * time;
	HandleScreenBorder(position);
	body.Move(position);
}

bool Actor::CheckCollision(const std::vector<Actor*>& actors){	
	for (auto actor : actors) {
		if (actor != this) {	
			float radSum = body.GetRadius() + actor->GetBody().GetRadius();	//sum of radius
			float distance = Line::Distance(position, actor->GetPosition());
			//shallow check
			if (radSum > distance) {
				//deep check
				if (Collision(body, actor->body)) {
					actor->Destroy();
					return true;
				}
			}		
		}
	}
	return false;
}

bool Actor::Collision(Polygon& a, Polygon& b) {
	std::vector<sf::Vector2f> verticesA;
	std::vector<sf::Vector2f> verticesB;

	//store all vertices of polygonA
	for (Edge edge : a.GetEdges()){
		verticesA.push_back(edge.GetLine().pointA);
	}

	//store all vertices of polygonB
	for (Edge edge : b.GetEdges()){
		verticesB.push_back(edge.GetLine().pointA);
	}

	auto normalsA = CalculateAllNormals(verticesA);
	auto normalsB = CalculateAllNormals(verticesB);
	
	//get all the normal vectors
	std::vector<sf::Vector2f> allNormals;
	allNormals.insert(allNormals.end(), normalsA.begin(), normalsA.end());
	allNormals.insert(allNormals.end(), normalsB.begin(), normalsB.end());

	for (sf::Vector2f normal : allNormals) {
		float paMin = FLT_MAX;
		float paMax = FLT_MIN;

		for (sf::Vector2f vertex : verticesA){
			//project each vertex of polygonA onto the normal vector using dot product
			float projection = DotProduct(vertex, normal);

			//take the lowest/greatest projection as paMin/paMax
			paMin = std::min(paMin, projection);
			paMax = std::max(paMax, projection);
		}

		float pbMin = FLT_MAX;
		float pbMax = FLT_MIN;

		for (sf::Vector2f vertex : verticesB) {
			//project each vertex of polygonB onto the normal vector using dot product
			float projection = DotProduct(vertex, normal);

			pbMin = std::min(pbMin, projection);
			pbMax = std::max(pbMax, projection);
		}

		//return false is there is a seperation
		if (paMax < pbMin || pbMax < paMin) {
			return false;
		}
		//continue the checks for collision
	}

	return true;
}

void Actor::Draw(sf::RenderWindow& window) const {
	body.Draw(window);
}

void Actor::Destroy(bool playerDestroy){
	alive = false;
	killedByPlayer = playerDestroy;
}

const sf::Vector2f& Actor::GetPosition() const {return position;}

const Polygon& Actor::GetBody() const { return body; }

bool Actor::IsAlive() const {return alive;}

bool Actor::IsDestroyedByPlayer() const {return killedByPlayer;}
