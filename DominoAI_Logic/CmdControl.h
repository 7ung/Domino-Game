#pragma once

#include <string>

#include <vector>


class CmdControl
{
private:
	CmdControl();
	
	static CmdControl* instance;

	void handleAction(std::vector<std::string> params);

public:

	static CmdControl* getInstance();

	void handleCmd(std::string cmd);

	~CmdControl();
};

