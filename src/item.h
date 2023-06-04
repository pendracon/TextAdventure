#ifndef _ITEM_H
#define _ITEM_H

#include "config.h"
#include "main.h"
#include "zobject.h"

class Item: public ZObject
{
public:
    Item();
    Item(stats_config);
    bool hasDescription();
    string getDescription();
    bool hasLookText();
    string getLookText();
    bool isLootable();
    bool isUsable();
    string getUsableOn();
    string getUseText();
    int getHealth();
    int getAttack();
    int getDefense();
    void print(ostream *);

private:
    stats_config config;
};

#endif
