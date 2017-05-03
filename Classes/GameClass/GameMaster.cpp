#include "GameMaster.h"

#include "HelloWorldScene.h"

using namespace std;


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

	_players[HUMAN_PLAYER]->setPosition(24, 24);
	_players[HUMAN_PLAYER]->setHuman(true);

	_players[AI_PLAYER]->setPosition(24, 300);
	_players[AI_PLAYER]->setHuman(false);

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
	this->_table->setPosition(20.0f, 120.0f);
	//this->_table->setPosition(0.0f, 0.0f);

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
		scene->showMessage("COMPUTER TURN");
		auto delay = DelayTime::create(3);
		auto seq = Sequence::create(delay, CallFunc::create([this](){
			_players[AI_PLAYER]->autoAll();
		}), nullptr);
		this->runAction(seq);
	}
	else if (this->_playerPosition == HUMAN_PLAYER){
		scene->showMessage("PLAYER TURN");

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



void GameMaster::nextPlayer()
{
	if (this->checkEndGame())
		return;
	this->setCurrentPlayer((getCurrentPlayer() + 1) % this->_players.size());

	auto scene = (HelloWorld*)Director::getInstance()->getRunningScene();
	if (this->_playerPosition == HUMAN_PLAYER)
	{
		if (Deck::getInstance()->hasChess())
			scene->disableBtn("skip");
		else
			scene->enableBtn("skip");
		scene->enableBtn("draw");
	}
	else
	{
		scene->disableBtn("skip");
		scene->disableBtn("draw");
	}
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

bool GameMaster::checkEndGame()
{
	auto player = getWinner();
	if (player == nullptr)
		return false;
	if (Deck::getInstance()->hasChess() == false){
		if (!_players[HUMAN_PLAYER]->hasAvailableMove()
			&& !_players[AI_PLAYER]->hasAvailableMove()){
			player = getWinnerByScore();
		}
	}
	winner(player);
	_isRunning = false;
	_players[AI_PLAYER]->showAllChess();
	_deck->showAllChess();
	return true;
}

void GameMaster::winner(Player* player)
{
	if (player->isHuman())
	{
		this->showMessage("PLAYER WIN");
	}
	else 
	{
		this->showMessage("COMPUTER WIN");
	}
}

Player* GameMaster::getWinner()
{
	for (Player* p : _players)
	{
		// Người chơi hết cờ thì thắng
		if (p->hasChess() == false)
		{
			return p;
		}
	}
	
	return nullptr;
}

Player* GameMaster::getWinnerByScore()
{
	int* scores = new int[_players.size()];
	int min = 500;
	int minIndex = -1;
	for (int i = 0; i < _players.size(); i++)
	{
		scores[i] = _players[i]->getScore();
		if (scores[i] < min)
		{
			min = scores[i];
			minIndex = i;
		}
	}
	return _players.at(minIndex);
}

void GameMaster::showMessage(std::string msg)
{
	auto scene = (HelloWorld*)Director::getInstance()->getRunningScene()->getChildByName("game-layer");
	scene->showMessage(msg);
}

void GameMaster::shuffleDeck()
{
	//this->_deck->shuffle();
}

void GameMaster::replay()
{
	_players[HUMAN_PLAYER]->replay();
	_players[AI_PLAYER]->replay();
	_table->replay();
}


GameMaster* GameMaster::getInstance()
{
	if (instance == nullptr)
		instance = new GameMaster();

	return instance;
}

GameMaster::GameMaster()
{
	this->_isRunning = false;

}

GameMaster::~GameMaster()
{
}
