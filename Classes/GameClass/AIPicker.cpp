#include "AIPicker.h"
#include "Table.h"

#include <iostream>

using namespace std;

AIPicker* AIPicker::_instance;

AIPicker* AIPicker::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new AIPicker();
	}
	return _instance;
}

Chess* AIPicker::selectMove(vector<Chess*> handChess)
{
	//cout << "========================================" << endl;
	//cout << "AI Algorithm working..." << endl;
	//cout << "But I really don\'t know how to choose" << endl;
	//cout << "I will give you a random chess ^^" << endl;
	////auto chess = available.at(rand() % available.size());
	//cout << "AI Algorithm finish" << endl;
	//cout << "========================================" << endl;
	
	
	GameState* root = GameState::createRoot(handChess);
	root->initChild();
	Chess* chess = root->getRecommendedMove();
	return chess;
}


AIPicker::AIPicker()
{
	//for (size_t i = 0; i < 7; i++)
	//{
	//	this->_chessByNumber[i] = "0123456";
	//}
}


AIPicker::~AIPicker()
{
}
