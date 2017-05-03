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
	this->addChild(chess);
	chess->release();
	chess->setRotation(0);
	this->arrangeChess();
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
	this->addChild(chess);
	chess->release();
	chess->setRotation(0);
	this->arrangeChess();
	GameMaster::getInstance()->nextPlayer();
}

void Table::arrangeChess()
{
	if (_tableChesses.empty())
		return;

	int i = 0;
	float spaceChess = 6;
	int width = _tableChesses.front()->getSprite()->getBoundingBox().size.width + spaceChess;
	int height = _tableChesses.front()->getSprite()->getBoundingBox().size.height + spaceChess;
	Vec2 taretPosition;

	const int angle[28] =
	{
		0, 0, 0, 0, 0, 0, -90,
		180, 180, 180, 180, 180, 180, -90,
		0, 0, 0, 0, 0, 0, -90,
		180, 180, 180, 180, 180, 180, -90

	};
	const Vec2 pos[28] = 
	{
		Vec2(1, 1), Vec2(2, 1), Vec2(3, 1), Vec2(4, 1), Vec2(5, 1), Vec2(6, 1), Vec2(6.8, 1.5),
		Vec2(6, 2), Vec2(5, 2), Vec2(4, 2), Vec2(3, 2), Vec2(2, 2), Vec2(1, 2), Vec2(0.2, 2.5),
		Vec2(1, 3), Vec2(2, 3), Vec2(3, 3), Vec2(4, 3), Vec2(5, 3), Vec2(6, 3), Vec2(6.8, 3.5),
		Vec2(6, 4), Vec2(5, 4), Vec2(4, 4), Vec2(3, 4), Vec2(2, 4), Vec2(1, 4), Vec2(0.2, 4.5),
	};
	for (Chess* ch : _tableChesses)
	{
		auto action = MoveTo::create(0.5, Vec2(pos[i].x * width, pos[i].y * height));
		ch->stopAllActions();
		ch->runAction(action);
		ch->setRotation(angle[i]);
		i++;
	}
}


void Table::removeChess(Chess* removedChess)
{
	for (auto it = this->_tableChesses.begin(); it != _tableChesses.end(); it++)
	{
		if ((*it) == removedChess){
			_tableChesses.erase(it);
			removedChess->retain();
			removedChess->removeFromParentAndCleanup(false);
			return;
		}
	}
}


void Table::replay()
{
	if (_tableChesses.empty())
		return;
	while (_tableChesses.empty() == false)
	{
		auto ch = this->_tableChesses.front();
		_tableChesses.erase(_tableChesses.begin());
		ch->retain();
		ch->removeFromParentAndCleanup(false);
		Deck::getInstance()->append(ch);
	}
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