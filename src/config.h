#ifndef _CONFIG_H
#define _CONFIG_H

#include <fstream>
#include <map>
#include "main.h"
#include "zobject.h"

const string NONE = "\0";
const string CONFIG_NAME = "Crawler Config";

// Configuration keys
// - Meta config
//
const string KEY_TITLE = "title";
const string KEY_NUMBER_OF_ROOMS = "num.rooms";
const string KEY_NUMBER_OF_ENEMIES = "num.enemies";
const string KEY_NUMBER_OF_ITEMS = "num.items";

// - Objects config: object configuration entries follow pattern <KEY>.n.<ATTR>
//   where '<KEY>' is one the following, '<ATTR>' is one of the below
//   attributes, and 'n' is the item's configuration index in range 1 - 'num.'
//   and, in the case of rooms, also represents the room's position number.
//
const string KEY_ENEMY = "enemy";
const string KEY_ITEM = "item";
const string KEY_ROOM = "room";
//
// - Enemy and Item attributes
//
const string ATTR_HEALTH = "health";
const string ATTR_ATTACK = "attack";
const string ATTR_DEFENSE = "defense";
const string ATTR_LOOTABLE = "lootable";
const string ATTR_USABLE = "usable";
const string ATTR_USABLE_ON = "usableOn";
const string ATTR_REQUIRES_ITEM = "requiresItem";
const string ATTR_USE_TEXT = "useText";

//
// - Room attributes
//
const string ATTR_IS_EXIT = "isExit";
const string ATTR_ENEMIES = "enemies";          // csv list of enemy names
const string ATTR_ITEMS = "items";              // csv list of item names
const string ATTR_CONNECTIONS = "connections";  // csv list of connecting room positions
//
// - Common attributes
//
const string ATTR_NAME = "name";
const string ATTR_DESCRIPTION = "description";
const string ATTR_LOOK_TEXT = "lookText";


struct key_value {
    string key;
    string value;
};

struct stats_config {
    string name;
    string description;
    string lookText;
    int health;
    int attack;
    int defense;
    bool lootable;
    bool usable;
    string usableOn;
    string requiresItem;
    string useText;
};

struct room_config {
    int position;
    string name;
    string description;
    string lookText;
    bool isExit;
    vector<string> connections;
    vector<string> items;
    vector<string> enemies;
};

bool endsWith(string, string);
bool startsWith(string, string);
string trim(string);
string toCSV(vector<string>);

class Config: public ZObject
{
public:
    Config(string);
    string getDungeonTitle();
    int getNumberOfEnemies();
    int getNumberOfItems();
    int getNumberOfRooms();
    stats_config getEnemyStats(string);
    stats_config getItemStats(string);
    room_config getRoomConfig(int);
    void print(ostream*);

private:
    void parseConfig(string);
    key_value split(string);
    void setConfigValue(key_value);
    void setObjectConfigValue(key_value, const string);
    void setRoomConfigValue(key_value);

    string title;
    int numEnemies;
    int numItems;
    int numRooms;
    map<string, stats_config> enemies;
    map<string, stats_config> items;
    vector<room_config> rooms;
};

#endif
