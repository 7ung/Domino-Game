
#include "Chess.h"

#include <string>
using namespace std;

Chess* Chess::create(int head, int tail)
{
	CCLOG("Creating chess... Head: %d, Tail: %d", head, tail);
	CCLOG("Chess created");

	Chess* chess = new (std::nothrow) Chess(head, tail);
	if (chess && chess->init())
	{
		chess->autorelease();
		return chess;
	}
	
	CC_SAFE_DELETE(chess);
	return nullptr;
}

const map<int, string> mapFileName = {
		{ 0, "domino_01.png" }, { 1, "domino_02.png" }, { 2, "domino_03.png" }, { 3, "domino_04.png" },
		{ 4, "domino_05.png" }, { 5, "domino_06.png" }, { 6, "domino_07.png" }, { 12, "domino_08.png" },
		{ 13, "domino_09.png" }, { 14, "domino_10.png" }, { 15, "domino_11.png" }, { 16, "domino_12.png" },
		{ 23, "domino_13.png" }, { 24, "domino_14.png" }, { 25, "domino_15.png" }, { 26, "domino_16.png" },
		{ 34, "domino_17.png" }, { 35, "domino_18.png" }, { 36, "domino_19.png" }, { 45, "domino_20.png" },
		{ 46, "domino_21.png" }, { 56, "domino_22.png" }, { 11, "domino_23.png" }, { 22, "domino_24.png" },
		{ 33, "domino_25.png" }, { 44, "domino_26.png" }, { 55, "domino_27.png" }, { 66, "domino_28.png" },
	};

bool Chess::init()
{
	if (Node::init() == false)
		return false;

	// this
	//this->setAnchorPoint(Vec2::UNIT_Y);
	
	// sprite
	_sprite = Sprite::create(mapFileName.at(this->getHead() * 10 + this->getTail()));
	_sprite->setScale(0.4f);
	this->addChild(_sprite);
	
	// face down sprite
	_faceDown = Sprite::create("domino_01.png");
	_faceDown->setVisible(false);
	_faceDown->setScale(0.4f);

	this->addChild(_faceDown);


	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [this](Touch* touch, Event* _event)->bool{

		auto chessPosition = this->getParent()->convertToWorldSpace(this->getPosition());
		int width = this->getSprite()->getBoundingBox().size.height;
		int height = this->getSprite()->getBoundingBox().size.width;
		auto chessBound = Rect(chessPosition.x - width / 2, chessPosition.y - height / 2, 
			width, height);

		auto touchLocation = this->getParent()->convertToWorldSpace(touch->getLocation());
		if (chessBound.containsPoint(touch->getLocation())){
			if (onClickEvent != nullptr){
				onClickEvent(this);
			}
		}
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, _sprite);

	return true;
}

void Chess::setFaceUp(bool isFaceUp)
{
	_faceDown->setVisible(!isFaceUp);
	_sprite->setVisible(isFaceUp);
}

void Chess::show()
{
	//cout << "[" << this->getHead() << " - " << this->getTail() << "]" << endl;
	CCLOG("[%d - %d]", this->getHead(), this->getTail());
}

bool Chess::isSameNumber()
{
	return (this->getHead() == this->getTail());
}

void Chess::switchNumber()
{
	_isSwitch = !_isSwitch;
	this->getSprite()->setFlipX(_isSwitch);
	this->getSprite()->setFlipY(_isSwitch);
}

bool Chess::hasNumber(const int& number)
{
	return ((this->getHead() == number) || (this->getTail() == number));
}

int Chess::getHead()
{
	return numbers[_isSwitch ? 1 : 0];
}

int Chess::getTail()
{
	return numbers[_isSwitch ? 0 : 1];
}

Chess* Chess::clone()
{
	auto chess = Chess::create(this->getHead(), this->getTail());
	return chess;
}

Chess::Chess(int head, int tail)
{
	numbers[0] = head;
	numbers[1] = tail;
	_isSwitch = false;
}

Chess::~Chess()
{
}