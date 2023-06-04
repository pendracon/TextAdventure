#include "character.h"
#include "config.h"
#include "dungeon.h"
#include "item.h"
#include "main.h"
#include "player.h"
#include "room.h"

// Main game functions
void welcome(Config*);
Dungeon initDungeon(Config*);

int main()
{
    Config cfg = Config("config/dungeon.cfg");

    welcome(&cfg);

    Dungeon game = initDungeon(&cfg);
    bool winner = game.run();

    if(winner) {
        cout << "Congratulations, you win!" << endl;
    }
    else {
        cout << "You lose, better luck next time." << endl;
    }

    exit(0);
}

void welcome(Config *cfg)
{
    cout << "Welcome to " << cfg->getDungeonTitle() << ", a text based mini-dungeon crawler!" << endl;
    cout << "To win the game you must find and defeat the wizard." << endl;
    cout << "If you exit the dungeon, quit the game, or are killed" << endl
         << "by the wizard then you lose." << endl;
    cout << "Ta-daa!" << endl << endl;
}

Dungeon initDungeon(Config *cfg)
{
    int numRooms = cfg->getNumberOfRooms();
    Dungeon dungeon = Dungeon(cfg->getDungeonTitle(), numRooms);

    room_config roomCfg;
    for (int i = 0; i < numRooms; i++) {
        roomCfg = cfg->getRoomConfig(i);
        Room* room = new Room(roomCfg, cfg);
        dungeon.addRoom(room);
    }
    dungeon.setPlayer(new Player((stats_config){"Indiana Jones", "", "", 100, 10, 20}));

    return dungeon;
}
