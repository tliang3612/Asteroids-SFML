#pragma once
#include "GameManager.h"
#include "Explosion.h"

enum GameState { START, IN_GAME, GAME_OVER, NEW_TOP_SCORE, LEADERBORD };

class Game {
private:
	sf::Text score;
	sf::Text activeText;
	sf::Text initials;

	Polygon extraLife;
	std::vector<Explosion> VFX;
	std::vector<std::pair<int, std::string>> leaders;
	
	std::chrono::duration<float> deltaTime;
	std::chrono::steady_clock::time_point clock;
	
	sf::Event evt;

	sf::RenderWindow* window;
	GameManager& gameManager;
	bool& isGameOver;
	
	GameState currentState;
	bool isPlayerDead;
	
public:
	Game(GameManager& gameManager, bool& gameOver);
	void Run();

	void Start();
	void InGame();
	void GameOver();
	void NewTopScore();
	void LeaderBoard();

	void ChangeOnStart();
	void StartGame();
	void ChangeOnOver();
	void ChangeOnInitials();
	void ChangeOnLeaderboard();

	void LoadLeaderBoard();
	void SaveLeaderBoard();

	void CheckEvent();
	void KeyboardPressCheck();
	void KeyboardReleaseCheck();
	void UpdateLeaderbord(const std::string& newInitials);
	void UpdateInitials(int pos, bool ink = false);
};