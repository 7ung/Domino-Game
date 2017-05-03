#pragma once

#include "Chess.h"

#include <vector>
#include "cocos2d.h"

USING_NS_CC;

class Deck: public Node
{
	std::vector<Chess*> chesses;
	Deck();

	static Deck* _instance;
public:

	static Deck* getInstance();

	bool init();
	void shuffle();
	Chess* drawTop();

	void showAllChess();
	void arrangeChess();

	void append(Chess* chess);
	void showState();

	inline bool hasChess(){
		return this->chesses.size() != 0;
	}

	~Deck();
};

