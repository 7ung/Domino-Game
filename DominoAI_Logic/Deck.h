#pragma once

#include "Chess.h"

#include <vector>

class Deck
{
	std::vector<Chess*> chesses;
	Deck();

	static Deck* _instance;

public:

	static Deck* getInstance();

	void init();
	void shuffle();
	Chess* drawTop();
	
	void showState();

	inline bool hasChess(){
		return this->chesses.size() != 0;
	}

	~Deck();
};

