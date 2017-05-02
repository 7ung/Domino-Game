#include "Table.h"
#include "GameClass\GameMaster.h"

#include <iostream>
using namespace std;

Table* Table::instance;

Table* Table::getInstance()
{
	if (instance == nullptr)
	{
		instance = new  (std::nothrow)  Table();
		if (instance && instance->init())
		{
			instance->autorelease();
			return instance;
		}
		CC_SAFE_DELETE(instance);
		return nullptr;
	}
	return instance;
}

bool Table::init()
{
	return Node::init();
}

Table::Table()
{
}


Table::~Table()
{
}

void Table::addHead(Chess* chess)
{
	// Đảm bảo rằng con cờ có quân trùng với head của bàn cờ.
	if (this->hasChess())
		_ASSERT(chess->hasNumber(this->getHeadRequired()));
	// Nếu đặt cờ vào head thì tail của cờ bằng head của bàn cờ.
	if (chess->getTail() != this->getHeadRequired())
	{
		chess->switchNumber();
	}
	this->_tableChesses.push_front(chess);
	chess->setFaceUp(true);
	chess->release();
	GameMaster::getInstance()->nextPlayer();

}

void Table::addTail(Chess* chess)
{
	// Đảm bảo rằng con cờ có quân trùng với tail của bàn cờ.
	if (this->hasChess())
		_ASSERT(chess->hasNumber(this->getTailRequired()));
	// Nếu đặt cờ vào tail thì head của cờ bằng tail của bàn cờ.
	if (chess->getHead() != this->getTailRequired())
	{
		chess->switchNumber();
	}
	this->_tableChesses.push_back(chess);
	chess->setFaceUp(true);
	chess->release();
	GameMaster::getInstance()->nextPlayer();
}

int Table::getHeadRequired()
{
	if (_tableChesses.size() == 0)
		return -1;
	return this->_tableChesses.front()->getHead();
}

int Table::getTailRequired()
{
	if (_tableChesses.size() == 0)
		return -1;
	return this->_tableChesses.back()->getTail();
}

void Table::showState()
{
	cout << "Table state" << endl;
	cout << "Current chesses count: " << _tableChesses.size() << endl;
	for (auto it = _tableChesses.begin(); it != _tableChesses.end() ; it++)
	{
		(*it)->show();
	}
}