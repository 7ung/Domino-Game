#pragma once

#include <map>

class Pooling
{
private:
	Pooling();

	std::map<std::string, void*> _objects;

	static Pooling* instance;
public:
	static Pooling* getInstance();

	void store(std::string name, void* object);
	void* load(std::string name);

	~Pooling();
};

