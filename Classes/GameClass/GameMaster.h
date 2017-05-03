#ifndef __GAME_MASTER_H__
#define __GAME_MASTER_H__

#include "cocos2d.h"
#include "Player.h"
#include "Deck.h"
#include "Table.h"

#include <vector>

USING_NS_CC;

#define AI_PLAYER 1
#define HUMAN_PLAYER 0

class GameMaster : public Node
{
public:
	~GameMaster();
	
	static GameMaster* getInstance();

	bool init(int playerNumber);

	void addPlayer(Player* player);
	void initDeck();
	void startGame();

	void nextPlayer();

	void showState();

	int findFirstPlayer();

	void shuffleDeck();

	inline int getCurrentPlayer()
	{
		return _playerPosition;
	}

	void setCurrentPlayer(const int& pos);

	void showMessage(std::string msg);

	inline Player* getHumanPlayer()
	{
		if (_players.size() <= HUMAN_PLAYER)
			return nullptr;
		return _players[HUMAN_PLAYER];
	}

	inline bool isRunning()
	{
		return _isRunning;
	}

	bool checkEndGame();

	void winner(Player* player);

	void replay();
private:
	std::vector<Player*>	_players;

	Deck* _deck;
	Table* _table;

	bool _isRunning;
	int _playerPosition;
	
	Player* getWinner();
	Player* getWinnerByScore();
	static GameMaster* instance;
	GameMaster();

};


#endif // !__GAME_MASTER_H__
