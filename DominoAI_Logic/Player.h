#pragma once

#include "Chess.h"

#include <vector>

class Player
{
private:
	std::vector<Chess*> _handlingChesss;
	Player();

	std::vector<Chess*> getAvailableChess();
public:
	static Player* create();
	void appendHand(Chess* chess);

	int maxSameChess();
	int maxChessValue();

	void move(int position);
	void moveAI();
	void moveFirstTurn();
	void draw();

	Chess* getChess(int i);
	Chess* getChessByNumber(int head, int tail);
	void removeChess(Chess*);
	void showState();

	inline bool hasChess(){
		return this->_handlingChesss.size() != 0;
	}
	~Player();
};

