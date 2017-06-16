// DominoAI_Logic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <ctime>        // std::time
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

#include "Game.h"
#include "CmdControl.h"


int _tmain(int argc, _TCHAR* argv[])
{
	std::srand(unsigned(std::time(0)));
	cout << "=====================================================" << endl;
	cout << "=================== GAME CREATION ===================" << endl;
	Game* game = Game::getInstance();
	game->init(2);
	cout << "================= GAME CREATION END =================" << endl;

	char cmd[50];
	while (1){
		cin.getline(cmd, sizeof(cmd));
		CmdControl::getInstance()->handleCmd(string(cmd));
	}
	return 0;
}





