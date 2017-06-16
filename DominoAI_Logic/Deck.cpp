#include "stdafx.h"
#include "Deck.h"

#include <algorithm>
#include <iostream>

using namespace std;

Deck* Deck::_instance;

Deck* Deck::getInstance(){
	if (_instance == nullptr){
		cout << "Creating deck..." << endl;
		_instance = new Deck();
		_instance->init();
		cout << "Deck created ..." << endl;
	}
	return _instance;

}

void Deck::init()
{
	cout << "Deck is waiting for create chesses..." << endl;
	for (int i = 0; i <= 6; i++)
	{
		for (int j = i; j <= 6; j++)
		{
			this->chesses.push_back(Chess::create(i, j));
		}
	}
	cout << "Init Deck finish" << endl;
}

Chess* Deck::drawTop()
{
	Chess* last = this->chesses.back();
	this->chesses.pop_back();
	return last;
}


void Deck::shuffle(){
	cout << "Shuffling deck..." << endl;
	std::random_shuffle(this->chesses.begin(), this->chesses.end(), [](int i)-> int{
		return std::rand() % i;
	});

	//std::shuffle(this->chesses.begin(), this->chesses.end(),);
	cout << "Deck shuffled. " << " Deck size: " << this->chesses.size() << endl;

}

void Deck::showState()
{
	cout << "Deck state" << endl;
	cout << "Current chesses count: " << chesses.size() << endl;
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
