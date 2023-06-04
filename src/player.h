#ifndef _PLAYER_H
#define _PLAYER_H

#include "character.h"
#include "config.h"
#include "item.h"
#include "room.h"

const int BASE_HEALTH_LEVEL_UP = 10;
const int BASE_ATTACK_LEVEL_UP = 5;
const int BASE_DEFENSE_LEVEL_UP = 10;

class Player: public Character
{
public:
    Player(stats_config);
    int getCurrentLevel();
    void levelUp();
    bool attack(Room *);
    bool flee(Room *);
    void collectItems(Room *);
    string useItem(Room *);
    void print(ostream *);
    
private:
    int currentLevel;
};

#endif
