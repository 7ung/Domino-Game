
#include "Player.h"

#include "Table.h"

#include "Deck.h"
#include "GameMaster.h"
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
				Table::getInstance()->addTail(chess);
				//cout << "Move first turn done" << endl;
				this->arrangeChess();
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
	this->arrangeChess();

	if (chess->hasNumber(required[1]))
		Table::getInstance()->addTail(chess);
	else
		Table::getInstance()->addHead(chess);
}

void Player::move(Chess* chess)
{
	int required[2] = {
		Table::getInstance()->getHeadRequired(),
		Table::getInstance()->getTailRequired()
	};
	this->removeChess(chess);
	this->arrangeChess();

	if (chess->hasNumber(required[1]))
		Table::getInstance()->addTail(chess);
	else
		Table::getInstance()->addHead(chess);
}

void Player::drawChess()
{
	auto chess = Deck::getInstance()->drawTop();
	if (chess == nullptr)
	{
		return;
	}
	//cout << "Drew chess: ";
	CCLOG("Drew chess: ");
	chess->show();
	this->appendHand(chess);
	this->arrangeChess();
}

void Player::moveAI()
{
	vector<Chess*> availableMove;
	this->getAvailableChess(availableMove);
	while (availableMove.empty())
	{
		if (Deck::getInstance()->hasChess())
			this->drawChess();
		else
			this->skipTurn();
		this->getAvailableChess(availableMove);
	}
	Chess* chess;
	if (availableMove.size() == 1){
		chess = availableMove.at(0);
	}
	else {
		chess = AIPicker::getInstance()->selectMove(this->_handlingChesss);
		chess = getChessByNumber(chess->getHead(), chess->getTail());
	}
	this->removeChess(chess);
	this->move(chess);

	//cout << "Move AI done." << endl;
	CCLOG("Move AI done.");

}

bool Player::hasAvailableMove()
{
	int head = Table::getInstance()->getHeadRequired();
	int tail = Table::getInstance()->getTailRequired();

	for (Chess* ch : this->_handlingChesss)
	{
		if (ch->hasNumber(head) || ch->hasNumber(tail))
		{
			return true;
		}
	}
	return false;
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

			removedChess->onClickEvent = nullptr;
			removedChess->retain();
			removedChess->removeFromParentAndCleanup(false);

			return;
		}
	}
}

Chess* Player::getChess(int index)
{
	auto chess = _handlingChesss[index];
	this->_handlingChesss.erase(_handlingChesss.begin() + index);

	chess->onClickEvent = nullptr;
	chess->retain();
	chess->removeFromParentAndCleanup(false);
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

void Player::getAvailableChess(std::vector<Chess*>& availableChess)
{
	if (Table::getInstance()->hasChess() == false){
		availableChess.insert(availableChess.end(),
			_handlingChesss.begin(), _handlingChesss.end());
		return;
	}
	
	int required[2] = {
		Table::getInstance()->getHeadRequired(),
		Table::getInstance()->getTailRequired()
	};
	for (auto it = this->_handlingChesss.begin(); it != _handlingChesss.end(); it++)
	{
		if ((*it)->hasNumber(required[0]) || (*it)->hasNumber(required[1]))
		{
			availableChess.push_back(*it);
		}
	}
}

void Player::appendHand(Chess* chess)
{
	_handlingChesss.push_back(chess);
	this->addChild(chess);
	addEventClick(chess);
	chess->release();

	if (this->isHuman())
	{
		chess->setFaceUp(true);
	}
}


void Player::addEventClick(Chess* chess)
{
	chess->onClickEvent = [](Chess* ch){
		auto player = (Player*)ch->getParent();
		bool isHumanTurn = GameMaster::getInstance()->getCurrentPlayer() == HUMAN_PLAYER;
		bool isGameRunning = GameMaster::getInstance()->isRunning();
		if (isGameRunning && isHumanTurn && player->isHuman()){
			if (Table::getInstance()->hasChess() == false){
				return player->move(ch);
			}
			int headRequired = Table::getInstance()->getHeadRequired();
			int tailRequired = Table::getInstance()->getTailRequired();
			if (!ch->hasNumber(headRequired) && !ch->hasNumber(tailRequired)){
				GameMaster::getInstance()->showMessage("NOT AVAILABLE MOVE");
				return;
			}
			player->move(ch);
		}
	};
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
	if (_handlingChesss.empty())
		return;
	auto basePosition = this->getPosition();
	int chessSpace = 6;

	for (int i = 0; i < _handlingChesss.size(); i++)
	{
		_handlingChesss[i]->setRotation(90);
		float widthSpace = _handlingChesss[i]->getSprite()->getBoundingBox().size.height + chessSpace;
		auto targetPosition = Vec2(basePosition.x + i * widthSpace, basePosition.y);
		auto action = MoveTo::create(0.7, targetPosition);
		_handlingChesss[i]->stopAllActions();
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

void Player::skipTurn()
{
	GameMaster::getInstance()->nextPlayer();
}

void Player::replay()
{
	if (_handlingChesss.empty())
		return;
	while (_handlingChesss.empty() == false)
	{
		auto ch = this->getChess(0);
		Deck::getInstance()->append(ch);
	}
}

int Player::getScore()
{
	int sum = 0;
	for (Chess* ch : _handlingChesss)
	{
		sum += ch->getHead();
		sum += ch->getTail();
	}
	return sum;
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