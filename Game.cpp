#include "Game.h"
#include "Saucer.h"

Game::Game(GameManager& gameManager, bool& gameOver):
		score("0", GetFont()),
		activeText(S_PRESS_ANY_KEY, GetFont()),
		initials("a__", GetFont(), 60),
		
		extraLife(sf::Vector2f(30.f, 70.f), STARSHIP_PATTERN),

		deltaTime(0),
		clock(std::chrono::high_resolution_clock::now()),

		window(nullptr),
		isGameOver(gameOver),
		gameManager(gameManager),
		currentState(GameState::START),
		isPlayerDead(false)
{
	FormatText(score);
	score.setPosition(5.0f, 5.0f);

	FormatText(activeText);
	TextToCenter(activeText);

	FormatText(initials);
	initials.setLetterSpacing(5.0f);
	TextToCenter(initials, 0.5f, 0.7f);

	leaders.reserve(LEADERBOARD_MAX_SIZE);
	VFX.reserve(ASTEROIDS_MAX_QUANTITY);
}

//main loop of the game
void Game::Run(){
	window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Asteroids", sf::Style::Fullscreen);
	window->setMouseCursorVisible(false);
	window->setFramerateLimit(144);

	while (window->isOpen()) {
		while (window->pollEvent(evt)) {
			CheckEvent();
		}

		window->clear();

		switch (currentState){
			case START:
				Start();
				break;
			case IN_GAME:
				InGame();
				break;
			case GAME_OVER:
				GameOver();
				break;
			case NEW_TOP_SCORE:
				NewTopScore();
				break;
			case LEADERBORD:
				LeaderBoard();
				break;
		}

		gameManager.Update(deltaTime.count());
		gameManager.Draw(*window);

		window->display();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			isGameOver = true;
			window->close();
		}

		deltaTime = std::chrono::high_resolution_clock::now() - clock;
		clock = std::chrono::high_resolution_clock::now();
	}
}

///called on every update on the start of the game
void Game::Start() {
	static float anim = 0.0f;
	anim += deltaTime.count();
	//a blinking effect for the title text at the start of the game
	if (anim > 0.5f) {
		window->draw(activeText);
		if (anim > 1.f) 
			anim = 0.f;
	}
}

//called on every update during in game
void Game::InGame(){
	//if player is dead, load the leaderboard
	if (!gameManager.IsPlayerAlive()) {
		ChangeOnOver();
		LoadLeaderBoard();
		return;
	}

	//draws the amount of player lives in the top-left corner of the screen
	for (unsigned i = 0; i < gameManager.GetPlayerLives(); ++i) {
		extraLife.Move(sf::Vector2f(30.0f * i + 30.0f, 70.0f));
		extraLife.Draw(*window);
	}
	
	score.setString(std::to_string(gameManager.GetScore()));

	window->draw(score);
}

void Game::GameOver() {
	static float passedTime = 0;
	passedTime += deltaTime.count();
	if (passedTime > 2.0f) {
		passedTime = 0.0f;
		if (leaders.size() < 10) {
			ChangeOnInitials();
			gameManager.Clear();
			return;
		}
		for (auto& iter : leaders) {
			if (gameManager.GetScore() > iter.first) {
				ChangeOnInitials();
				gameManager.Clear();
				return;
			}
		}
		gameManager.Clear();
		ChangeOnLeaderboard();
		return;
	}
	//gameManager.Draw(*window);
	window->draw(activeText);
}

void Game::NewTopScore(){
	window->draw(activeText);
	window->draw(initials);
}

void Game::LeaderBoard(){
	window->draw(score);
	window->draw(activeText);
}

void Game::ChangeOnStart(){
	currentState = START;
	VFX.clear();
	leaders.clear();
	activeText.setString(S_PRESS_ANY_KEY);
	TextToCenter(activeText);
}

void Game::StartGame(){
	currentState = IN_GAME;
	gameManager.StartGame();
}

void Game::ChangeOnOver(){
	currentState = GAME_OVER;
	activeText.setString(S_GAME_OVER);
	TextToCenter(activeText);
}

void Game::ChangeOnInitials(){
	currentState = NEW_TOP_SCORE;
	activeText.setString(S_NEW_HIGH_SCORE);
	TextToCenter(activeText);
	initials.setString("a__");
}

void Game::ChangeOnLeaderboard(){
	currentState = LEADERBORD;
	activeText.setString(S_HIGH_SCORE);
	int counter = 0;
	for (auto& iter : leaders) {
		++counter;
		activeText.setString(activeText.getString() + "\n"
							+ std::to_string(counter) + " "
							+ std::to_string(iter.first) + " "
							+ iter.second);
	}
	TextToCenter(activeText);
}

void Game::LoadLeaderBoard(){
	std::ifstream fin("res/leaders.txt");
	int size, score;
	std::string name;
	fin >> size;
	size = std::min(size, LEADERBOARD_MAX_SIZE);
	for (unsigned i = 0; i < size; ++i) {
		fin >> name >> score;
		leaders.push_back(std::make_pair(score, name));
	} 
	fin.close();
}

void Game::SaveLeaderBoard(){
	std::ofstream fout("res/leaders.txt");
	fout << leaders.size() << std::endl;
	for (auto& leader : leaders) {
		fout << leader.second << " " << leader.first << std::endl;
	}
	fout.close();
}

void Game::CheckEvent() {
	switch (evt.type) {
		case sf::Event::Closed:
			break;
		case sf::Event::KeyPressed:
			KeyboardPressCheck();
			break;
		case sf::Event::KeyReleased:
			KeyboardReleaseCheck();
			break;
	}
}

//
void Game::KeyboardPressCheck() {
	switch (currentState)	{
		case START:
			StartGame();
			return;
		case IN_GAME:
			if (gameManager.GetPlayer().CanMove()) {
				switch (evt.key.code) {
				case sf::Keyboard::J:
					gameManager.GetPlayer().Thrust(true);
					break;
				case sf::Keyboard::D:
					gameManager.GetPlayer().SetRotationDirection(RotateDirection::RIGHT);
					break;
				case sf::Keyboard::A:
					gameManager.GetPlayer().SetRotationDirection(RotateDirection::LEFT);
					break;
				}
			}
			break;
		case LEADERBORD:
			ChangeOnStart();
			return;
	}	
}

void Game::KeyboardReleaseCheck() {
	static int activeSymPos = 0;
	switch (currentState) {
		case IN_GAME:
			if (gameManager.GetPlayer().CanMove()) {
				switch (evt.key.code) {
					case sf::Keyboard::J:
						gameManager.GetPlayer().Thrust(false);
						break;
					case sf::Keyboard::D:
						gameManager.GetPlayer().SetRotationDirection(RotateDirection::NONE);
						break;
					case sf::Keyboard::A:
						gameManager.GetPlayer().SetRotationDirection(RotateDirection::NONE);
						break;
					case sf::Keyboard::L:
						gameManager.Shoot();
						break;
				}
			}
			break;
		case NEW_TOP_SCORE:
			switch (evt.key.code) {
				case sf::Keyboard::G:
					++activeSymPos;
					UpdateInitials(activeSymPos);
					break;
				case sf::Keyboard::D:
					UpdateInitials(activeSymPos, true);
					break;
				case sf::Keyboard::A:			
					UpdateInitials(activeSymPos, false);
					break;
				}
			if (activeSymPos > 2) {
				ChangeOnLeaderboard();
				activeSymPos = 0;
				return;		
			}
			break;
	}
}

void Game::UpdateLeaderbord(const std::string& newInitials){
	leaders.push_back(std::make_pair(gameManager.GetScore(), newInitials));
	std::sort(leaders.begin(), leaders.end(), []
											(std::pair<int, std::string> f,
												std::pair<int, std::string> s)
											{return f.first > s.first; });
	if (leaders.size() > 10) 
		leaders.pop_back();
	SaveLeaderBoard();
}

void Game::UpdateInitials(int pos, bool ink){
	static std::string l_initials("a__");
	static char activeSymbol = 65;
	static int activePos = 0;
	if (activePos != pos) {
		if (activePos < 2) 
			l_initials.at(activePos + 1) = 'a';
		initials.setString(l_initials);
		activePos = pos;
		activeSymbol = 65;
		if (pos > 2) {
			UpdateLeaderbord(l_initials);
			l_initials = "a__";
			activePos = 0;
		}
		return;
	}
	if (ink && (int)activeSymbol < 90) 
		++activeSymbol;
	else if (!ink && (int)activeSymbol > 65) 
		--activeSymbol;
	l_initials.at(pos) = activeSymbol;
	initials.setString(l_initials);
}
