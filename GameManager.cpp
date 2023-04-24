#include "GameManager.h"

GameManager::GameManager() :
		player(new Player(PLAYER_DEFAULT_POS, 0.0f)),
		saucer(nullptr),
		saucerSpawned(false),
		gameRunning(false)
{
	bullets.reserve(BULLETS_MAX_CAPACITY);
	actors.reserve(ASTEROIDS_MAX_QUANTITY);

	actors.clear();
	bullets.clear();

	GenerateAsteroids(0.0f, 0.0f); //Generate asteroids on start screen
}

//returns the player
Player& GameManager::GetPlayer() const {return *player;}

///returns the score
int GameManager::GetScore() const {return score;}

///Gets the lives of the player
int GameManager::GetPlayerLives() const {return player->GetLives();}

///returns true if the player is alive
bool GameManager::IsPlayerAlive() const {
	return player->GetLives() > 0;
}

///starts the game 
void GameManager::StartGame(){
	player->Reset();
	score = 0;
	gameRunning = true;
	saucerSpawned = false;

	std::lock_guard lg(gmMutex);
	actors.clear();
	actors.push_back(player);
}

///create a new shot given the position and rotation of the player
void GameManager::Shoot() {
	std::lock_guard lg(gmMutex);
	shots.push(Shot(player->GetPosition(), player->GetRotation()));
}

///update
void GameManager::Update(float deltaTime) {
	if (gameRunning)
	{
		//small delay before generating initial asteroids
		GenerateAsteroids(deltaTime, 1.5f);
		SpawnSaucer(deltaTime);
	}

	if (IsExplosionsActive()) {
		VFX.push_back(explosions.front());
		explosions.pop();
	}

	//handles the explosion vfx 
	for (auto explosion = VFX.begin(); explosion != VFX.end(); ++explosion) {
		explosion->Update(deltaTime);
		if (!explosion->isAlive()) {
			VFX.erase(explosion);
			break;
		}
	}

	//checks if were any shots that were shot last frame
	if (!shots.empty()) {
		std::lock_guard lg(gmMutex);
		//fires all shots 
		while (!shots.empty()) {
			Fire(shots.front(), 1, true);
			shots.pop();
		}
	}
	
	//loop through all actors
	for (auto actor : actors) {
		//if the actor is an asteroid
		if (dynamic_cast<Asteroid*>(actor)) {
			//if the asteroid was destroyed last frame
			if (!actor->IsAlive()) {
				int stage = dynamic_cast<Asteroid*>(actor)->GetState();
				//add a new explosion based on asteroid position
				explosions.push(actor->GetPosition());
				PlaySound(Sound(1 + stage));
				if (stage > 1) 
					CrackAsteroid(actor->GetPosition(), --stage);

				if(actor->IsDestroyedByPlayer()) 
					UpdateScore(dynamic_cast<Asteroid*>(actor)->GetValue());
			}
		}
	}

	//if saucer was destroyed last frame, handle saucer death
	if (saucerSpawned && !saucer->IsAlive()) {
		saucerSpawned = false;
		explosions.push(saucer->GetPosition());
		if(saucer->IsDestroyedByPlayer()) 
			UpdateScore(saucer->IsBigSaucer() ? Values::SAUC_B : Values::SAUC_S);
	}	

	//remove dead bullets last frame
	std::vector<Bullet>::iterator deadBullets = remove_if(bullets.begin(), bullets.end(), [](Bullet b) {
		return !b.IsAlive();
		});

	bullets.erase(deadBullets, bullets.end());

	//remove dead actors from last frame
	std::vector<Actor*>::iterator dead_actors = remove_if(actors.begin(), actors.end(), [](Actor* a) {
		return !a->IsAlive();
	});

	actors.erase(dead_actors, actors.end());
	
	
	if (player->CanMove()) {
		if (saucerSpawned && saucer->CanShoot()) {
			Fire(saucer->GetShot(player->GetPosition()), 1);
		}
	}
	for (auto actor : actors) {
		actor->Update(deltaTime, actors);
	}
	for (auto& bullet : bullets) {
		bullet.Update(deltaTime, actors);
	}
}

///fires the bullet 
void GameManager::Fire(Shot shot, float pushDistance, bool isPlayerShot) {
	if (bullets.size() < BULLETS_MAX_CAPACITY) {
 		bullets.push_back(Bullet(shot, isPlayerShot));
		bullets.back().PushForward(pushDistance); //pushes the bullet 
		PlaySound(Sound::FIRE);
	}
}

///Generates asteroids based on waitTime
void GameManager::GenerateAsteroids(float deltaTime, float waitTime){
	static sf::Vector2f pos, dir;
	static float passedTime = 0.0f;
	int quantity = (score > 40000) ? 15 : 10;
	//generate asteroids if there are no asteroids
	if (actors.size() < 2) {
		if ((passedTime += deltaTime) >= waitTime) {
			passedTime = 0.0f;
			for (unsigned i = 0; i < quantity; ++i) {
				if (RAND_BOOL(gen)) {
					pos.x = RAND_BOOL(gen) ? (WIDTH) : 0.0f;
					pos.y = RAND_Y(gen);
				}
				else {
					pos.x = RAND_X(gen);
					pos.y = RAND_BOOL(gen) ? (HEIGHT) : 0.0f;
				}
				dir.x = RAND_DIR(gen);
				dir.y = RAND_DIR(gen);
				Asteroid* asteroid = new Asteroid(pos, dir);
				actors.push_back(asteroid);
			}
		}
		
	}
}

///spawns a saucer if none have spawned yet
void GameManager::SpawnSaucer(float deltaTime){
	static float passedTime;
	static sf::Vector2f startPos;
	if (!saucerSpawned) {
		passedTime += deltaTime;
		if(passedTime > SAUCER_SPAWN_DELAY && actors.size() > 1) {
			passedTime = 0.0f;
			bool spawnBig = !(score > 40000 || RAND_BOOL(gen));
			startPos.x = RAND_BOOL(gen) ? WIDTH : 0.f;
			startPos.y = RAND_Y(gen);
			saucer = new Saucer(startPos, spawnBig);
			actors.push_back(saucer);
			saucerSpawned = true;
		}
	}
}

///creates two new asteroids with random directions
void GameManager::CrackAsteroid(const sf::Vector2f& pos, int state) {
	static sf::Vector2f dir;
	for (unsigned i = 0; i < 2; ++i){ 
		dir.x = RAND_DIR(gen);
		dir.y = RAND_DIR(gen);
		actors.push_back(new Asteroid(pos+dir*10.f, dir, state)); //small offset 
	}
}

///Draw the game contents
void GameManager::Draw(sf::RenderWindow& window) {
	std::lock_guard guard(gmMutex);

	for (auto& bullet : bullets) {
		window.draw(bullet.GetBody());
	}
	//draws all actors 
	for (auto actor : actors) {
		if (gameRunning) {
			actor->Draw(window);
		}
		else if (!dynamic_cast<Player*>(actor)) { //draws all non-player actors on update
			actor->Draw(window);
		}
	}

	//draws the explosion vfx
	for (auto& explosion : VFX) {
		explosion.Draw(window);
	}

}


void GameManager::UpdateScore(Values points){
	static int bonusLife = 0;
	score += (int)points;
	if (bonusLife < score/10000) {
		player->BonusLife();
		++bonusLife;
	}
	if (score > 99990) 
		score = 0;
}

void GameManager::Clear(){
	std::lock_guard lg(gmMutex);
	if (saucerSpawned) 
		saucer->Destroy();

	saucerSpawned = false;
	saucer = nullptr;
	bullets.clear();
	actors.clear();
	explosions = std::queue<sf::Vector2f>();
	gameRunning = false;
}

bool GameManager::IsExplosionsActive() const {
	return !explosions.empty();
}

const sf::Vector2f& GameManager::PopExplosion(){
	static sf::Vector2f res;
	res = explosions.front();
	explosions.pop();
	return res;
}
