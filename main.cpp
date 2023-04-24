#include "Game.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>

int main(){

	HWND hWnd = GetConsoleWindow();

	ShowWindow(hWnd, SW_HIDE);
	LoadResources();

	GameManager gameManager;
	
	bool gameOver = false;

	Game game(gameManager, gameOver);

	std::thread IoThread(&Game::Run, game);		//input output thread
	IoThread.detach();

	while (!gameOver){
		
	}

	return 0;
}