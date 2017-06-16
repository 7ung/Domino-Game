#pragma once

#include "Chess.h"
#include <list>
#include <vector>

class Table
{
	Table();
	std::list<Chess*> _tableChesses;

	static Table* instance;
public:
	static Table* getInstance();  

	inline std::vector<Chess*> getChesses()
	{
		 std::vector<Chess*> v{ std::begin(_tableChesses), std::end(_tableChesses) };
		 return v;
	}

	void addHead(Chess* chess);
	void addTail(Chess* chess);

	int getHeadRequired();
	int getTailRequired();

	void showState();
	
	inline bool hasChess(){
		return _tableChesses.size() != 0;
	}
	~Table();
};

