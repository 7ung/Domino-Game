#include "stdafx.h"
#include "Player.h"
#include "Table.h"
#include "AIPicker.h"
#include "Deck.h"

#include <iostream>

using namespace std;

Player* Player::create(){
	cout << "Creating Player..." << endl;
	Player* player = new Player();
	cout << "Player created." << endl;
	return player;

}

void Player::appendHand(Chess* chess)
{
	_handlingChesss.push_back(chess);
}

void Player::showState()
{
	cout << "Current chesses count: " << _handlingChesss.size() << endl;
	for (int i = 0; i < _handlingChesss.size(); ++i)
	{
		_handlingChesss[i]->show();
	}
}

int Player::maxSameChess()
{
	int rs = -1;
	for (size_t i = 0; i < _handlingChesss.size(); i++)
	{
		if (_handlingChesss[i]->isSameNumber() == false)
			continue;
		if (_handlingChesss[i]->getHead() <= rs)
			continue;
		rs = _handlingChesss[i]->getHead();
	}
	return rs;
}

int Player::maxChessValue()
{
	int rs = -1;
	int value = 0;
	for (size_t i = 0; i < _handlingChesss.size(); i++)
	{
		value = _handlingChesss[i]->getHead() + _handlingChesss[i]->getTail();
		if (value <= rs)
			continue;
		rs = value;
	}
	return value;
}

void Player::moveFirstTurn()
{
	int maxSameChess = this->maxSameChess();
	if (maxSameChess == -1)
	{
		int maxValue = this->maxChessValue();
	}
	else {
		for (size_t i = 0; i < this->_handlingChesss.size(); i++)
		{
			if (this->_handlingChesss[i]->isSameNumber() && _handlingChesss[i]->getHead() == maxSameChess)
			{
				Chess* chess = this->getChess(i);
				Table::getInstance()->addTail(chess);
				cout << "Move first turn done" << endl;
				return;
			}
		}
	}
}

void Player::move(int position)
{
	Chess* chess = this->getChess(position);
	int required[2] = {
		Table::getInstance()->getHeadRequired(),
		Table::getInstance()->getTailRequired()
	};
	if (chess->hasNumber(required[1]))
		Table::getInstance()->addTail(chess);
	else
		Table::getInstance()->addHead(chess);
}

void Player::draw()
{
	auto chess = Deck::getInstance()->drawTop();
	cout << "Drew chess: ";
	chess->show();
	this->appendHand(chess);
}

// Tạm thời thì random nước thôi.
void Player::moveAI()
{
	auto availableMove = this->getAvailableChess();
	Chess* chess;
	int required[2] = {
		Table::getInstance()->getHeadRequired(),
		Table::getInstance()->getTailRequired()
	};
	if (availableMove.empty())
	{
		cout << "Not chess available. Draw chess." << endl;
		this->draw();
		return moveAI();
	}
	else if (availableMove.size() == 1){
		chess = availableMove.at(0);
	}
	else {
		chess = AIPicker::getInstance()->selectMove(this->_handlingChesss);
		chess = getChessByNumber(chess->getHead(), chess->getTail());
	}

	if (chess->hasNumber(required[1]))
		Table::getInstance()->addTail(chess);
	else
		Table::getInstance()->addHead(chess);
	
	this->removeChess(chess);
	cout << "Move AI done." << endl;

}

void Player::removeChess(Chess* removedChess)
{
	for (auto it = this->_handlingChesss.begin(); it != _handlingChesss.end(); it++)
	{
		if ((*it) == removedChess){
			_handlingChesss.erase(it);
			return;
		}
	}
}

Chess* Player::getChess(int index)
{
	auto chess = _handlingChesss[index];
	this->_handlingChesss.erase(_handlingChesss.begin() + index);
	return chess;
}

Chess* Player::getChessByNumber(int head, int tail)
{
	for (Chess* ch : this->_handlingChesss)
	{
		if (ch->getHead() == head && ch->getTail() == tail)
		{
			return ch;
		}
		if (ch->getTail() == head && ch->getHead() == tail)
		{
			return ch;
		}
	}
}

std::vector<Chess*> Player::getAvailableChess()
{
	if (Table::getInstance()->hasChess() == false)
		return this->_handlingChesss;
	std::vector<Chess*> rs;
	int required[2] = { 
		Table::getInstance()->getHeadRequired(),
		Table::getInstance()->getTailRequired()
	};
	for (auto it = this->_handlingChesss.begin(); it != _handlingChesss.end(); it++)
	{
		if ((*it)->hasNumber(required[0]) || (*it)->hasNumber(required[1]))
		{
			rs.push_back(*it);
		}
	}
	return rs;
}

Player::Player()
{
}


Player::~Player()
{
}
