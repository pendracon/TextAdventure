#ifndef _ROOM_H
#define _ROOM_H

#include "character.h"
#include "config.h"
#include "item.h"
#include "main.h"
#include "zobject.h"

const string ACTION_ATTACK    = "Attack";
const string ACTION_EXIT      = "Exit";
const string ACTION_LOOK      = "Look";
const string ACTION_MOVE      = "Move";
const string ACTION_QUIT      = "Quit";
const string ACTION_RUN       = "Run";
const string ACTION_TAKE      = "Take";
const string ACTION_INVENTORY = "Inventory";
const string ACTION_USE       = "Use";

void appendText(string &text, string appendx);

class Room: public ZObject
{
public:
    Room(room_config, Config*);
    bool hasExit();
    int getPosition();
    bool hasConnections();
    vector<string> getConnections();
    bool hasItems();
    Item* getItem(string);
    vector<string> getItemNames();
    bool hasEnemies();
    Character* getEnemy(string);
    vector<string> getEnemyNames();
    void removeItem(string);
    void removeEnemy(string);
    void clearItems();
    void clearEnemies();
    string describe();
    string look();
    string listActions();
    void print(ostream *);

private:
    string getEnemiesText();
    string getItemsText(bool);
    void populateItems(Config*);
    void populateEnemies(Config*);

    room_config config;
    map<string, Character*> enemies;
    map<string, Item*> items;
};

#endif
