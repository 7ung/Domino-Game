#include "stdafx.h"
#include "Chess.h"
#include <iostream>

using namespace std;

Chess* Chess::create(int head, int tail)
{
	cout << "Creating chess..." << "head: " << head << " tail: " << tail << endl;
	Chess* chess = new Chess(head, tail);
	cout << "Chess created" << endl;

	return chess;

}

void Chess::show()
{
	cout << "[" << this->getHead() << " - " << this->getTail() << "]" << endl;
}

bool Chess::isSameNumber()
{
	return (this->getHead() == this->getTail());
}

int Chess::getHead()
{
	return numbers[_isSwitch ? 1 : 0];
}

int Chess::getTail()
{
	return numbers[_isSwitch ? 0 : 1];
}

void Chess::switchNumber()
{
	_isSwitch = !_isSwitch;
}

bool Chess::hasNumber(const int& number)
{
	return ((this->getHead() == number) || (this->getTail() == number));
}

Chess* Chess::clone()
{
	return new Chess(this->getHead(), this->getTail());
}

Chess::Chess(int head, int tail) // 
{
	numbers[0] = head;
	numbers[1] = tail;
	_isSwitch = false;
}


Chess::~Chess()
{
}
