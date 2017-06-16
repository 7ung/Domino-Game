#pragma once

#include "Player.h"
#include "Deck.h"
#include "Table.h"
#include <vector>

class Game
{
	std::vector<Player*>	_players;
	std::vector<bool>		_flags;

	Deck* _deck;
	Table* _table;
	
	bool _isRunning;
	int _playerPosition;

	Game();
	static Game* instance;
public:

	static Game* getInstance();

	void init(int playerNumber);
	void addPlayer(Player* player);
	void initDeck();
	void startGame();
	
	void nextPlayer();
	
	void showState();

	int findFirstPlayer();

	~Game();
};

