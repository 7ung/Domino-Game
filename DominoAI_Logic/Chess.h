// shit

#pragma once
class Chess
{
private: 
	int numbers[2];
	Chess(int head, int tail);	//

	bool _isSwitch;


public:
	int getHead();

	int getTail();

	bool hasNumber(const int& number);

	void show();
	void switchNumber();
	bool isSameNumber();
	static Chess* create(int head, int tail);
	Chess* clone();
	~Chess();
};

