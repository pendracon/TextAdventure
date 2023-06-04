#include "zobject.h"

int _nextId = 1;

ZObject::ZObject(string name)
{
    this->name = name;
    id = _nextId++;
}

int ZObject::getId()
{
    return id;
}

string ZObject::getName()
{
    return name;
}
