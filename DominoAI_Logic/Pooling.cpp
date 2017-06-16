#include "stdafx.h"
#include "Pooling.h"

using namespace std;

Pooling* Pooling::instance = nullptr;


Pooling* Pooling::getInstance()
{
	if (instance == nullptr){
		instance = new Pooling();
	}
	return instance;
}

void Pooling::store(std::string name, void* object)
{
	_objects[name] = object;
	//_objects.insert(std::pair<std::string, void*>(name, object));
}

void* Pooling::load(std::string name)
{
	return _objects[name];
}

Pooling::Pooling()
{
}


Pooling::~Pooling()
{
}
