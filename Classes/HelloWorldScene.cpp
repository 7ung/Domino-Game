#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "GameClass\Chess.h"
#include "GameClass\GameMaster.h"

#include "ui\CocosGUI.h"
#include <string>

USING_NS_CC;



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
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	auto gamemaster = GameMaster::getInstance();
	gamemaster->init(2);
	gamemaster->setPosition(Vec2(0.0f, 0.0f));

    // add the sprite as a child to this layer
	this->addChild(gamemaster);


	// UI

	auto button = ui::Button::create();
	button->setTitleText("Shuffle");
	button->setPosition(Vec2(visibleSize.width - 220.0f, visibleSize.height - 24.0f));
	button->setTitleFontName("fonts/WEST.TTF");
	button->setTitleFontSize(18);
	button->setColor(Color3B(51, 153, 204));
	button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED){
			GameMaster::getInstance()->startGame();
		}
	});
	this->addChild(button);
    return true;
}

void HelloWorld::showMessage(std::string msg)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto label = Label::create(msg, "fonts/WEST.TTF", 52, Size::ZERO,
		TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setColor(Color3B(0, 102, 102));
	label->setPosition(visibleSize.width / 2, visibleSize.height / 2);

	auto delay = DelayTime::create(3.0f);
	auto seq = Sequence::create(delay, RemoveSelf::create(), nullptr);
	label->runAction(seq);

	this->addChild(label);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
