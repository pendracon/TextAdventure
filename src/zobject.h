#ifndef _ZOBJECT_H
#define _ZOBJECT_H

#include <iostream>

using namespace std;

class ZObject
{
public:
    ZObject(string);
    int getId();
    string getName();
    virtual void print(ostream *) = 0;

private:
    int id;
    string name;
};

#endif
