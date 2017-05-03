
#ifndef __CHESS_H__

#define __CHESS_H__


#include "cocos2d.h"

USING_NS_CC;

class Chess;
typedef void(*onClick)(Chess* sender);

class Chess : public Node
{
public:
	~Chess();

	virtual bool init() override;		//

	int getHead();
	int getTail();

	bool hasNumber(const int& number);

	void show();
	void switchNumber();
	bool isSameNumber();

	static Chess* create(int head, int tail); //
	Chess* clone();

	inline Sprite* getSprite()
	{
		return _sprite;
	}

	onClick onClickEvent;

	void setFaceUp(bool isFaceUp);
	Chess(int head, int tail); // 

private:
	Sprite* _sprite;
	Sprite* _faceDown;
	int numbers[2];

	bool _isSwitch;


};



#endif // !__CHESS_H__
