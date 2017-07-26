#pragma once

#include "Chess.h"
#include "Table.h"

#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

class AIPicker
{


	// hahah
	// hihi

	// Những quân cờ đã biết. (Những quân cờ chắc chắn đối thủ không có)
	// Bao gồm cờ trên tay và cờ trên bàn.
	//std::vector<Chess*> _knownChess;
	//std::map<int, std::string> _chessByNumber;

	AIPicker();
	static AIPicker* _instance;

	void updateKnownChess(std::vector<Chess*> handChess, std::vector<Chess*> tableChess);
	void updateDeliveredChess();
public:
	static AIPicker* getInstance();

	Chess* selectMove(std::vector<Chess*> handChess);
	~AIPicker();

	class GameState
	{
	private:
		Chess* _select;
		vector<Chess*> _handChess;
		vector<Chess*> _oppHandChess;
		int _headRequired;
		int _tailRequired;

		vector<Chess*> _availableMove;
		bool	_isMyTurn;

		vector<GameState*> _children;
		int _level;
		float _index;
	public:
		static GameState* createRoot(vector<Chess*> handChess)
		{
			vector<Chess*> oppHandChess;
			for (int i = 0; i < 7; i++)
			{
				for (int j = i; j < 7; j++)
				{
					if (isExistsFrom(handChess, i, j))
						continue;
					if (isExistsFrom(Table::getInstance()->getChesses(), i, j))
						continue;
					oppHandChess.push_back(Chess::create(i, j));
				}
			}
			vector<Chess*> hand;
			for (Chess* ch : handChess){
				hand.push_back(ch->clone());
			}
			int headRequired = Table::getInstance()->getHeadRequired();
			int tailRequired = Table::getInstance()->getTailRequired();
			auto gameState = new GameState(hand, oppHandChess, headRequired, tailRequired);
			gameState->_isMyTurn = true;
			return gameState;
		}

		static bool isExistsFrom(const vector<Chess*> source, int first, int second)
		{
			for (Chess* ch : source)
			{
				if (ch->getHead() == first && ch->getTail() == second)
					return true;
				if (ch->getTail() == first && ch->getHead() == second)
					return true;
			}
			return false;
		}
		inline GameState(vector<Chess*> handChess, vector<Chess*> oppHandChess, int headRequired, int tailRequired)
		{
			_handChess.insert(_handChess.end(), handChess.begin(), handChess.end());
			_oppHandChess.insert(_oppHandChess.end(), oppHandChess.begin(), oppHandChess.end());
			_headRequired = headRequired;
			_tailRequired = tailRequired;

			_level = 0;
		}

		inline void getAvailableMove(vector<Chess*>& avaibleMove)
		{
			if (_isMyTurn){
				for (Chess* ch : _handChess)
				{
					if (ch->hasNumber(_headRequired) || ch->hasNumber(_tailRequired))  
					{
						avaibleMove.push_back(ch);
					}
				}
			}
			else {
				for (Chess* ch : _oppHandChess)
				{
					if (ch->hasNumber(_headRequired) || ch->hasNumber(_tailRequired))
					{
						avaibleMove.push_back(ch);
					}
				}
			}
		}

		inline void initChild()
		{
			if (this->_level >= 7 || _handChess.size() <= 1)
				return;
			getAvailableMove(this->_availableMove);

			for (Chess* ch : this->_availableMove)
			{
				GameState* child = nullptr;
				if (ch->getHead() == _headRequired){
					child = new GameState(
						_handChess, _oppHandChess,
						ch->getTail(), _tailRequired);
					child->_select = ch;
					this->addChild(child, ch);
				}
				if (ch->getTail() == _headRequired && ch->isSameNumber() == false){
					child = new GameState(
						_handChess, _oppHandChess,
						ch->getHead(), _tailRequired);
					child->_select = ch;
					this->addChild(child, ch);
				}
				if (_headRequired == _tailRequired)
					continue;
				if (ch->getHead() == _tailRequired){
					child = new GameState(
						_handChess, _oppHandChess,
						_headRequired, ch->getTail());
					child->_select = ch;
					this->addChild(child, ch);
				}
				if (ch->getTail() == _tailRequired && ch->isSameNumber() == false)
				{
					child = new GameState(
						_handChess, _oppHandChess,
						_headRequired, ch->getHead());
					child->_select = ch;
					this->addChild(child, ch);
				}

			}

		}

		inline void addChild(GameState* child, Chess* ch)
		{
			_ASSERT(child != nullptr);
			if (_isMyTurn){
				auto ownerList = &child->_handChess;
				ownerList->erase(std::remove(ownerList->begin(), ownerList->end(), ch), ownerList->end());
			}
			else {
				auto ownerList = &child->_oppHandChess;
				ownerList->erase(std::remove(ownerList->begin(), ownerList->end(), ch), ownerList->end());
			}
			child->_level = this->_level + 1;
			child->_isMyTurn = !this->_isMyTurn;
			_children.push_back(child);
			child->initChild();
			child->index();
		}

		inline void index()
		{
			this->_index = 0;
			if (this->_availableMove.empty()){
				if (_isMyTurn){
					this->_index = -10;
				}
				else {
					this->_index = +10;
				}
			}
			if (this->_isMyTurn)
				this->_index += 3 * this->_availableMove.size();
			else
				this->_index -= this->_availableMove.size();
			if (this->_children.empty() == false){
				float sum = 0;
				for (GameState* state : this->_children)
				{
					sum += state->_index;
				}
				this->_index += sum / this->_children.size();
			}
			
			this->_index * (2.0 - 0.1 * this->_level);
		}
		
		inline Chess* getRecommendedMove()
		{
			Chess* chess = _children.at(0)->_select;
			float max = _children.at(0)->_index;
			for (auto it = _children.begin()+1; it != _children.end(); it++)
			{
				if ((*it)->_index > max)
				{
					max = (*it)->_index;
					chess = (*it)->_select;
				}
			}
			return chess;
		}
	};
};

