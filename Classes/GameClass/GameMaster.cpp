#include "GameMaster.h"

#include "HelloWorldScene.h"

using namespace std;

#define AI_PLAYER 1
#define HUMAN_PLAYER 0

GameMaster* GameMaster::instance;

bool  GameMaster::init(int playerNumber)
{
	if (Node::init() == false)
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	//cout << "Creating Game..." << endl;
	CCLOG("Creating Game...");
	for (int i = 0; i < playerNumber; i++){
		auto player = Player::create();
		this->addPlayer(player);
	}

	_players[0]->setPosition(24, 24);
	_players[1]->setPosition(24, 300);

	this->initDeck();
	CCLOG("Game crated");
	return true;
	//cout << "Game crated" << endl;
}

void GameMaster::addPlayer(Player* player)
{

	this->_players.push_back(player);
	this->addChild(player);
	CCLOG("Player is added to game.");
	CCLOG("Current number player: %d", _players.size());
	//cout << "Player is added to game." << endl;
	//cout << "Current number player: " << _players.size() << endl;

}


void GameMaster::initDeck()
{
	CCLOG("Game is waiting for create deck...");
	//cout << "Game is waiting for create deck..." << endl;
	auto visibleSize = Director::getInstance()->getVisibleSize();

	this->_deck = Deck::getInstance();
	this->_deck->setPosition(visibleSize.width - 140, 0.0f);
	this->addChild(_deck);

	this->_table = Table::getInstance();
	this->_table->setPosition(Vec2::ZERO);
	this->addChild(_table);
	CCLOG("Game creating deck finish");
	//cout << "Game creating deck finish" << endl;


}

void GameMaster::startGame()
{
	CCLOG("====================================================");
	CCLOG("==================== GAME START ====================");
	CCLOG("Player is drawwing chesses...");
	//cout << "====================================================" << endl;
	//cout << "==================== GAME START ====================" << endl;
	//cout << "Player is drawwing chesses..." << endl;

	this->_deck->shuffle();

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < this->_players.size(); j++)
		{
			Chess* chess = this->_deck->drawTop();
			this->_players[j]->appendHand(chess);
		}
	}
	CCLOG("Drawwing done");
	//cout << "Drawwing done";
	this->_isRunning = true;
	this->setCurrentPlayer(findFirstPlayer());

	for (int j = 0; j < this->_players.size(); j++)
	{
		this->_players[j]->arrangeChess();
	}
	_players[0]->showAllChess();


	this->_deck->arrangeChess();

	CCLOG("First player is player position: %d", this->getCurrentPlayer() + 1);
	CCLOG("Done.");
	//cout << "First player is player position: " << (this->_playerPosition + 1) << endl;
	//cout << "Done." << endl;
}

void GameMaster::setCurrentPlayer(const int& pos)
{
	this->_playerPosition = pos;
	auto scene = (HelloWorld*)Director::getInstance()->getRunningScene()->getChildByName("game-layer");

	if (this->_playerPosition == AI_PLAYER)
	{
		scene->showMessage("AI turn");
		auto delay = DelayTime::create(3);
		auto seq = Sequence::create(delay, CallFunc::create([this](){
			_players[AI_PLAYER]->autoAll();
		}), nullptr);
		this->runAction(seq);
	}
	else if (this->_playerPosition == HUMAN_PLAYER){
		scene->showMessage("Human player");

	}
}

void GameMaster::showState()
{
	CCLOG("====================================================");
	CCLOG("==================== GAME STATE ====================");
	//cout << "====================================================" << endl;
	//cout << "==================== GAME STATE ====================" << endl;

	if (this->_isRunning)
		CCLOG("Game is running");
	//cout << "Game is running" << endl;
	else
		CCLOG("Game is not running");
	//cout << "Game is not running" << endl;
	CCLOG("Current player turn %d", this->getCurrentPlayer() + 1);
	//cout << "Current player turn " << (this->_playerPosition + 1) << endl;

	this->_table->showState();

	CCLOG("Number of player: %d", _players.size());
	//cout << "Number of player: " << _players.size() << endl;
	for (int i = 0; i < _players.size(); i++)
	{
		CCLOG("Player Position: %d", i + 1);
		//cout << "Player Position: " << i + 1 << endl;
		_players[i]->showState();
	}

	this->_deck->showState();

	CCLOG("================== GAME STATE END ==================");
	//cout << "================== GAME STATE END ==================" << endl;

}

void GameMaster::showMessage(std::string msg)
{
}


void GameMaster::nextPlayer()
{
	this->setCurrentPlayer((getCurrentPlayer() + 1) % this->_players.size());
}

int GameMaster::findFirstPlayer()
{
	CCLOG("Game finding first player turn...");
	//cout << "Game finding first player turn..." << endl;
	vector<int> tester = vector<int>(_players.size());
	for (size_t i = 0; i < _players.size(); i++)
	{
		tester[i] = _players[i]->maxSameChess();
		CCLOG("Player position %d - Max chess same number: %d", i + 1, tester[i]);
		//cout << "Player position " << (i + 1) << " - Max chess same number: " << tester[i] << endl;
	}

	auto rs = std::max_element(tester.begin(), tester.end());
	if (*rs == -1){
		CCLOG("Nobody have chess same number. Check max chess value");
		//cout << "Nobody have chess same number. Check max chess value";
		for (size_t i = 0; i < _players.size(); i++)
		{
			tester[i] = _players[i]->maxChessValue();
			CCLOG("Player position %d  - Max chess value: %d", i + 1, tester[i]);
			//cout << "Player position " << (i + 1) << " - Max chess value: " << tester[i] << endl;
		}
	}
	else {
		return std::distance(tester.begin(), rs);
	}
}

void GameMaster::shuffleDeck()
{
	//this->_deck->shuffle();
}


GameMaster* GameMaster::getInstance()
{
	if (instance == nullptr)
		instance = new GameMaster();

	return instance;
}

GameMaster::GameMaster()
{
}

GameMaster::~GameMaster()
{
}
