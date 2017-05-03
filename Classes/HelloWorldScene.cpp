#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "GameClass\Chess.h"
#include "GameClass\GameMaster.h"

#include "ui\CocosGUI.h"
#include <string>
#include <cstdlib>
#include <ctime>

USING_NS_CC;
using namespace std;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
	layer->setName("game-layer");

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(240, 240, 240, 255)) )
    {
        return false;
    }
	std::srand(unsigned(std::time(0)));

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	auto gamemaster = GameMaster::getInstance();
	gamemaster->init(2);
	gamemaster->setPosition(Vec2(0.0f, 0.0f));

    // add the sprite as a child to this layer
	this->addChild(gamemaster);


	// UI
	const Color3B activeColor = Color3B(51, 153, 204);
	const Color3B inactiveColor = Color3B(153, 204, 255);

	auto button = ui::Button::create();
	button->setTitleText("START");
	button->setPosition(Vec2(visibleSize.width - 220.0f, visibleSize.height - 25.0f));
	button->setTitleFontName("fonts/WEST.TTF");
	button->setTitleFontSize(18);
	button->setColor(activeColor);
	button->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED){
			if (GameMaster::getInstance()->isRunning())
				return;

			GameMaster::getInstance()->startGame();
			this->disableBtn("start");
			this->enableBtn("replay");
		}
	});
	button->setName("start");
	this->addChild(button);

	auto drawChessBtn = ui::Button::create();
	drawChessBtn->setTitleText("DRAW");
	drawChessBtn->setPosition(Vec2(visibleSize.width - 220.0f, visibleSize.height - 75.0f));
	drawChessBtn->setTitleFontName("fonts/WEST.TTF");
	drawChessBtn->setTitleFontSize(18);
	drawChessBtn->setColor(activeColor);
	drawChessBtn->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED){
			if (GameMaster::getInstance()->isRunning() == false)
				return;
			if (Deck::getInstance()->hasChess() == false)
				return;
			auto player = GameMaster::getInstance()->getHumanPlayer();
			if (player == nullptr)
				return;
			if (player->hasAvailableMove() || Table::getInstance()->hasChess() == false){
				GameMaster::getInstance()->showMessage("NOT ALLOW TO DRAW");
				return;
			}
			player->drawChess();
			if (Deck::getInstance()->hasChess() == false)
			{
				this->disableBtn("draw");
			}
			else
			{
				this->enableBtn("skip");
			}
		}
	});
	drawChessBtn->setName("draw");
	this->addChild(drawChessBtn);

	// replay button 
	auto replayBtn = ui::Button::create();
	replayBtn->setTitleText("REPLAY");
	replayBtn->setPosition(Vec2(visibleSize.width - 220.0f, visibleSize.height - 50.0f));
	replayBtn->setTitleFontName("fonts/WEST.TTF");
	replayBtn->setTitleFontSize(18);
	replayBtn->setColor(activeColor);
	replayBtn->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED){
			GameMaster::getInstance()->replay();
			Deck::getInstance()->showAllChess();
			this->disableBtn("replay");
			this->enableBtn("start");
		}
	});
	replayBtn->setName("replay");
	this->addChild(replayBtn);

	// skip btn
	auto skipBtn = ui::Button::create();
	skipBtn->setTitleText("SKIP");
	skipBtn->setPosition(Vec2(visibleSize.width - 220.0f, visibleSize.height - 100.0f));
	skipBtn->setTitleFontName("fonts/WEST.TTF");
	skipBtn->setTitleFontSize(18);
	skipBtn->setColor(activeColor);
	skipBtn->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED){
			if (GameMaster::getInstance()->isRunning() == false)
				return;
			auto player = GameMaster::getInstance()->getHumanPlayer();
			if (player == nullptr)
				return;
			if (player->hasAvailableMove() || Deck::getInstance()->hasChess()){
				GameMaster::getInstance()->showMessage("NOT ALLOW TO SKIP");
				return;
			}
			player->skipTurn();
			
		}
	});
	skipBtn->setName("skip");
	this->addChild(skipBtn);

	// border
	auto upperLine = DrawNode::create(4);
	auto startPoint = Vec2(0.0f, visibleSize.height - 110);
	auto endPoint = Vec2(visibleSize.width - 280, visibleSize.height - 110);

	upperLine->drawLine(startPoint, endPoint,Color4F(0.02, 0.02, 0.02, 1.0));
	this->addChild(upperLine, 10);

	auto lowerLine = DrawNode::create(4);
	startPoint = Vec2(0.0f, 110.0f);
	endPoint = Vec2(visibleSize.width - 280, 110.0f);

	lowerLine->drawLine(startPoint, endPoint, Color4F(0.02, 0.02, 0.02, 1.0));
	this->addChild(lowerLine, 10);


    return true;
}

void HelloWorld::showMessage(std::string msg)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto label = Label::create(msg, "fonts/WEST.TTF", 46, Size::ZERO,
		TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setColor(Color3B(0, 102, 102));
	label->setPosition(visibleSize.width / 2 - 50, visibleSize.height / 2 + 150);

	auto delay = DelayTime::create(3.0f);
	auto seq = Sequence::create(delay, RemoveSelf::create(), nullptr);
	label->runAction(seq);
	label->setName("label");
	this->addChild(label);
}

void HelloWorld::disableBtn(string name)
{
	auto child = this->getChildByName(name);
	if (child == nullptr)
		return;
	child->setColor(Color3B(153, 204, 255));
	((ui::Button*) child)->setEnabled(false);
	((ui::Button*) child)->setBright(false);
}

void HelloWorld::enableBtn(string name)
{
	auto child = this->getChildByName(name);
	if (child == nullptr)
		return;
	child->setColor(Color3B(51, 153, 204));
	((ui::Button*) child)->setEnabled(true);
	((ui::Button*) child)->setBright(true);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
