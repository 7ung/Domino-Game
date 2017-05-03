
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

	void move(Chess* chess);
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

	void addEventClick(Chess* chess);
	
	void replay();

	void skipTurn();

	bool hasAvailableMove();

	int getScore();

	inline void setHuman(const bool& human)
	{
		this->_isHuman = human;
	}

	inline bool isHuman(){
		return _isHuman;
	}
private:
	Player();
	std::vector<Chess*> _handlingChesss;

	void getAvailableChess(std::vector<Chess*>& availableChess);

	bool _isHuman;
};
#endif // !__PLAYER_H__
