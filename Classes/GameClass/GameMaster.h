#ifndef __GAME_MASTER_H__
#define __GAME_MASTER_H__

#include "cocos2d.h"
#include "Player.h"
#include "Deck.h"
#include "Table.h"

#include <vector>

USING_NS_CC;

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

private:
	std::vector<Player*>	_players;

	Deck* _deck;
	Table* _table;

	bool _isRunning;
	int _playerPosition;
	
	static GameMaster* instance;
	GameMaster();

	void showMessage(std::string msg);
};


#endif // !__GAME_MASTER_H__
