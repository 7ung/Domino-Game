
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "GameClass\Chess.h"

USING_NS_CC;

#include <vector>

class Player : public Node
{
public:
	~Player();

	bool init();

	static Player* create();

	int maxSameChess();
	int maxChessValue();

	void move(int position);
	void moveAI();
	void moveFirstTurn();
	void drawChess();

	Chess* getChess(int i);
	Chess* getChessByNumber(int head, int tail);
	void removeChess(Chess*);
	void appendHand(Chess* chess);
	bool hasChess();

	void showAllChess();
	void arrangeChess();
	void showState();

	void autoAll();

private:
	Player();
	std::vector<Chess*> _handlingChesss;

	std::vector<Chess*> getAvailableChess();
};
#endif // !__PLAYER_H__
