#include "Bullet.h"
#include "Player.h"
#include "Saucer.h"

Bullet::Bullet(Shot shot, bool playerShooted) :
		body(1.5f),
		position(std::move(shot.from)),
		speed(600.0f),
		timeAlive(0.0f),
		lifeTime(1.0f),
		alive(true),
		isFromPlayer(playerShooted)
{
	RotateUnitVector(this->direction, shot.angle);
	
	if(isFromPlayer)
		body.setFillColor(sf::Color::Green);
	else
		body.setFillColor(sf::Color::Red);	
}

///returns a reference to the bullet body
const sf::CircleShape& Bullet::GetBody() const {return body;}

bool Bullet::IsAlive() const {return alive;}

bool Bullet::IsPlayerOwned() const {return isFromPlayer;}


void Bullet::Update(float t, std::vector<Actor*>& actors) {
	position += direction * speed * t;				//new position
	//sets bullet to dead if it passes the screen
	if (IsPassingHorizontalBorder(position) || IsPassingVerticalBorder(position)) {
		alive = false;
		return;
	}
	body.setPosition(position);
	CheckCollision(actors);

	speed -= t / 30.0f; //braking over time
	timeAlive += t;

	CheckIfAlive();
}

void Bullet::CheckCollision(std::vector<Actor*>& actors) {
	if (actors.size() <= 0)
		return;

	for (auto& actor : actors){
		//bullets from player should not collide with player, and likewise for saucer
		if ((isFromPlayer && dynamic_cast<Player*>(actor)) || (!isFromPlayer && dynamic_cast<Saucer*>(actor))) {
			continue;
		}			
		
		float distance = Line::Distance(position, actor->GetPosition());
		//Shallow check
		if (distance <= actor->GetBody().GetRadius() + body.getRadius()) {
			//deep check
			if (actor->GetBody().IsPointInPolygon(position)) {
				actor->Destroy(isFromPlayer);
				alive = false;
				return;
			}
		}
	}	
}


///pushes the bullet forward "distance" amount 
void Bullet::PushForward(float distance){
	position += direction * distance;
}

void Bullet::CheckIfAlive(){
	if (timeAlive >= lifeTime || speed <= 0.0f)
		alive = false;
}