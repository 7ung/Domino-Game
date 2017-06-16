#include "stdafx.h"
#include "CmdControl.h"

#include "Game.h"
#include "Pooling.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

CmdControl* CmdControl::instance;

CmdControl* CmdControl::getInstance()
{
	if (instance == nullptr)
	{
		instance = new CmdControl();
	}
	return instance;
}

void CmdControl::handleCmd(string cmd)
{
	if (cmd == "")
		return;
	vector<string> param;
	string tmp;
	istringstream stream(cmd);
	while (getline(stream, tmp, ' '))
	{
		param.push_back(tmp);
	}

	handleAction(param);

	
}

void CmdControl::handleAction(vector<string> param)
{
	if (param[0] == "game")
	{
		auto game = (Game*)Pooling::getInstance()->load("game");
		if (param[1] == "show-state") {
			game->showState();
		}
		else if (param[1] == "start-game")
		{
			game->startGame();
		}
	}
	else if (param[0] == "player")
	{
		string objectname = "player " + param[1];
		auto player = (Player*)Pooling::getInstance()->load(objectname);

		if (param[2] == "move-first")
		{
			player->moveFirstTurn();
			auto game = (Game*)Pooling::getInstance()->load("game");
			game->showState();
		}
		else if (param[2] == "move-ai")
		{
			player->moveAI();
			auto game = (Game*)Pooling::getInstance()->load("game");
			game->showState();
		}
		else if (param[2] == "move"){
			int chessPosition = std::stoi(param[3]);
			player->move(chessPosition);
			auto game = (Game*)Pooling::getInstance()->load("game");
			game->showState();
		}
		else if (param[2] == "draw"){
			player->draw();
		}
	}
}

CmdControl::CmdControl()
{
}


CmdControl::~CmdControl()
{
}
