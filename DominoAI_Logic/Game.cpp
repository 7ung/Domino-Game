#include "stdafx.h"
#include "Game.h"

#include "Pooling.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

Game* Game::instance;

Game* Game::getInstance()
{
	if (instance == nullptr)
		instance = new Game();

	Pooling::getInstance()->store("game", instance);
	return instance;
}

void Game::init(int playerNumber)
{
	cout << "Creating Game..." << endl;
	for (int i = 0; i < playerNumber; i++){
		this->addPlayer(Player::create());
	}
	this->initDeck();
	cout << "Game crated" << endl;
}

void Game::addPlayer(Player* player)
{

	this->_players.push_back(player);
	cout << "Player is added to game." << endl;
	cout << "Current number player: " << _players.size() << endl;

	this->_flags.push_back(false);
	Pooling::getInstance()->store("player " + std::to_string( _players.size() ), player);

}


void Game::initDeck()
{
	cout << "Game is waiting for create deck..." << endl;
	this->_deck = Deck::getInstance();
	this->_deck->shuffle();
	this->_table = Table::getInstance();
	cout << "Game creating deck finish" << endl;
	Pooling::getInstance()->store("deck", _deck);


}

void Game::startGame()
{
	cout << "====================================================" << endl;
	cout << "==================== GAME START ====================" << endl;
	cout << "Player is drawwing chesses..." << endl;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < this->_players.size(); j++)
		{
			Chess* chess = this->_deck->drawTop();
			this->_players[j]->appendHand(chess);
		}
	}
	cout << "Drawwing done";
	this->_isRunning = true;
	this->_playerPosition = findFirstPlayer();
	this->_flags[_playerPosition] = true;

	cout << "First player is player position: " << (this->_playerPosition + 1) << endl;
	cout << "Done." << endl;
}

void Game::showState()
{
	cout << "====================================================" << endl;
	cout << "==================== GAME STATE ====================" << endl;
	
	if (this->_isRunning)
		cout << "Game is running" << endl;
	else
		cout << "Game is not running" << endl;
	cout << "Current player turn " << (this->_playerPosition + 1) << endl;
	
	this->_table->showState();

	cout << "Number of player: " << _players.size() << endl;
	for (int i = 0; i < _players.size(); i++ )
	{
		cout << "Player Position: " << i + 1 << endl;
		_players[i]->showState();
	}

	this->_deck->showState();
	
	cout << "================== GAME STATE END ==================" << endl;

}

void Game::nextPlayer()
{
	this->_playerPosition = (_playerPosition + 1) % this->_players.size();
}

int Game::findFirstPlayer()
{
	cout << "Game finding first player turn..." << endl;
	vector<int> tester =  vector<int>(_players.size());
	for (size_t i = 0; i < _players.size(); i++)
	{
		tester[i] = _players[i]->maxSameChess();
		cout << "Player position " << (i + 1) << " - Max chess same number: " << tester[i] << endl;
	}

	auto rs = std::max_element(tester.begin(), tester.end());
	if (*rs == -1){
		cout << "Nobody have chess same number. Check max chess value";
		for (size_t i = 0; i < _players.size(); i++)
		{
			tester[i] = _players[i]->maxChessValue();
			cout << "Player position " << (i + 1) << " - Max chess value: " << tester[i] << endl;
		}
	}
	else {
		return std::distance(tester.begin(), rs);
	}
}

Game::Game()
{
}


Game::~Game()
{
}
