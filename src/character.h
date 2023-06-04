#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "config.h"
#include "item.h"
#include "main.h"
#include "zobject.h"

class Character: public ZObject
{
public:
    Character();
    Character(stats_config config);
    bool hasDescription();
    string getDescription();
    bool hasLookText();
    string getLookText();
    void addItem(Item *);
    void removeItem(Item *);
    vector<Item*> getInventory();
    string getInventoryNames();
    int getMaxHealth();
    int getCurrentHealth();
    int getAttack();
    int getDefense();
    int takeDamage(int);
    void increaseHealth(int);
    void increaseAttack(int);
    void increaseDefense(int);    
    bool hasDied();
    void print(ostream *);

protected:
    vector<Item*> inventory;

private:
    int reduceHealth(int);

    stats_config config;
    int maxHealth;
    int currentHealth;
};

#endif
