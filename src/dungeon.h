#ifndef DUNGEON_H
#define DUNGEON_H

#include <algorithm>
#include <map>
#include "main.h"
#include "player.h"
#include "room.h"
#include "zobject.h"

class Dungeon: public ZObject
{
public:
    Dungeon(string, int);
    ~Dungeon();
    bool run();
    bool addRoom(Room*);
    Room* getRoom(int);
    int getNumberOfRooms();
    int getCurrentRoom();
    void setPlayer(Player*);
    Player* getPlayer();
    Room* enterRoom(int);
    int handleEndGame();
    void print(ostream*);

private:
    bool doAction(string, string);
    bool isValidAction(string, Room*);
    string listActions(Room*);
    bool handleAction(string, Room*);
    void changeRooms(Room*);
    void displayText(string);

    Player *player = nullptr;
    map<int, Room*> rooms;
    int numRooms;
    int currentRoom;
    int previousRoom;
};

#endif
