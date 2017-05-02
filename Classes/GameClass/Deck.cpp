#include "Deck.h"

#include <algorithm>
#include <iostream>

using namespace std;

Deck* Deck::_instance;

Deck* Deck::getInstance(){
	if (_instance == nullptr){
		//cout << "Creating deck..." << endl;
		CCLOG("Creating deck...");
		_instance = new  (std::nothrow)  Deck();
		if (_instance && _instance->init())
		{
			_instance->autorelease();
			CCLOG("Deck created ...");
			return _instance;
		}
		CC_SAFE_DELETE(_instance);
		return nullptr;
		//cout << "Deck created ..." << endl;


	}
	return _instance;

}

bool Deck::init()
{
	if (Node::init() == false)
		return false;
	//cout << "Deck is waiting for create chesses..." << endl;

	CCLOG("Deck is waiting for create chesses...");
	for (int i = 0; i <= 6; i++)
	{
		for (int j = i; j <= 6; j++)
		{
			auto chess = Chess::create(i, j);
			this->addChild(chess);
			
			this->chesses.push_back(chess);
		}
	}

	arrangeChess();


	//cout << "Init Deck finish" << endl;
	CCLOG("Init Deck finish");
	return true;
}

void Deck::arrangeChess()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	Vec2 basePosition = Vec2(0.0f, visibleSize.height - 24);
	int chessSpace = 6;

	for (int i = 0; i < chesses.size(); i++)
	{
		Vec2 chessPositionIndex = Vec2(i / 14, i % 14);
		auto action = MoveTo::create(0.7,
			Vec2(basePosition.x + chessPositionIndex.x * (chesses[i]->getSprite()->getBoundingBox().size.width + chessSpace),
			basePosition.y - chessPositionIndex.y * (chesses[i]->getSprite()->getBoundingBox().size.height + chessSpace)));
		chesses[i]->runAction(action);
	}
}

Chess* Deck::drawTop()
{
	Chess* last = this->chesses.back();
	this->chesses.pop_back();
	last->retain();
	this->removeChild(last);
	return last;
}


void Deck::shuffle(){
	//cout << "Shuffling deck..." << endl;
	CCLOG("Shuffling deck...");
	std::random_shuffle(this->chesses.begin(), this->chesses.end(), [](int i)-> int{
		return std::rand() % i;
	});

	for (Chess* ch : this->chesses)
	{
		ch->setFaceUp(false);
	}

	CCLOG("Deck shuffled. Deck size: %d", this->chesses.size());
	//cout << "Deck shuffled. " << " Deck size: " << this->chesses.size() << endl;

}

void Deck::showState()
{
	//cout << "Deck state" << endl;
	CCLOG("Deck state");
	CCLOG("Current chesses count: %d", chesses.size());
	//cout << "Current chesses count: " << chesses.size() << endl;
	for (int i = 0; i < chesses.size(); ++i)
	{
		chesses[i]->show();
	}
}


Deck::Deck()
{
}


Deck::~Deck()
{
}
