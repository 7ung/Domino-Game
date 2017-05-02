
#include "Player.h"

#include "Table.h"

#include "Deck.h"
#include "AIPicker.h"

Player* Player::create()
{
	Player* player = new (std::nothrow) Player();
	if (player && player->init())
	{
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
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
				chess->retain();
				Table::getInstance()->addTail(chess);
				//cout << "Move first turn done" << endl;
				CCLOG("Move first turn done");
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
	chess->retain();
	if (chess->hasNumber(required[1]))
		Table::getInstance()->addTail(chess);
	else
		Table::getInstance()->addHead(chess);
}

void Player::drawChess()
{
	auto chess = Deck::getInstance()->drawTop();
	//cout << "Drew chess: ";
	CCLOG("Drew chess: ");
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
		//cout << "Not chess available. Draw chess." << endl;
		CCLOG("Not chess available. Draw chess.");
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
	this->removeChess(chess);

	if (chess->hasNumber(required[1]))
		Table::getInstance()->addTail(chess);
	else
		Table::getInstance()->addHead(chess);

	//cout << "Move AI done." << endl;
	CCLOG("Move AI done.");

}

void Player::autoAll()
{
	if (Table::getInstance()->hasChess() == false){
		this->moveFirstTurn();
	}
	else {
		this->moveAI();
	}
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
	removedChess->retain();
	this->removeChild(removedChess,false);
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

void Player::appendHand(Chess* chess)
{
	_handlingChesss.push_back(chess);
	this->addChild(chess);

	chess->setPosition(300, 300);
	chess->release();
}

bool Player::hasChess()
{
	return this->_handlingChesss.size() != 0;
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

void Player::arrangeChess()
{
	auto basePosition = this->getPosition();
	int chessSpace = 6;

	for (int i = 0; i < _handlingChesss.size(); i++)
	{
		_handlingChesss[i]->setRotation(90);

		auto targetPosition = Vec2(basePosition.x + i * (_handlingChesss[i]->getSprite()->getBoundingBox().size.width + chessSpace),
			basePosition.y);
		auto action = MoveTo::create(0.7, targetPosition);
		_handlingChesss[i]->runAction(action);
	}
}

void Player::showAllChess()
{
	for (Chess* ch : this->_handlingChesss)
	{
		ch->setFaceUp(true);
	}
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

void Player::showState()
{
	CCLOG("Current chesses count: %d", _handlingChesss.size());
	//cout << "Current chesses count: " << _handlingChesss.size() << endl;
	for (int i = 0; i < _handlingChesss.size(); ++i)
	{
		_handlingChesss[i]->show();
	}
}

bool Player::init()
{
	return Node::init();
}

Player::Player()
{
}


Player::~Player()
{
}