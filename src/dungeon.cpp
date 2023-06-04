#include "dungeon.h"

Dungeon::Dungeon(string title, int numRooms)
    : ZObject(title)
{
    this->numRooms = numRooms;
    currentRoom = 1;
    previousRoom = 0;
}

Dungeon::~Dungeon()
{
    player = nullptr;
}

bool Dungeon::run()
{
    bool playerWins = false;
    bool isRunning = true;

    string action;
    while (isRunning) {
        Room *room = enterRoom(getCurrentRoom());
        if (room) {
            cout << "? ";
            cin >> action;

            if (action.length() > 0) {
                if (!isValidAction(action, room)) {
                    cout << action << " is not a valid action for this room." << endl;
                }
                else if (doAction(action, ACTION_EXIT) || doAction(action, ACTION_QUIT)) {
                    cout << (doAction(action, ACTION_QUIT) ? "You quit the game." : "You leave the dungeon.") << endl;
                    isRunning = false;
                }
                else {
                    // handleAction returns true if the action results in the game
                    // being over
                    isRunning = !handleAction(action, room);
                }
                cout << endl;
            }
        }
        else {
            isRunning = false;
        }
    }

    return playerWins;
}

bool Dungeon::addRoom(Room *room)
{
    bool added = false;

    if (room->getPosition() <= numRooms) {
        rooms[room->getPosition()] = room;
        added = true;
    }

    return added;
}

Room* Dungeon::getRoom(int num)
{
    Room *room = nullptr;

    if (num > 0 && num <= rooms.size()) {
        room = rooms[num];
    }

    return room;
}

int Dungeon::getNumberOfRooms()
{
    return numRooms;
}

int Dungeon::getCurrentRoom()
{
    return currentRoom;
}

void Dungeon::setPlayer(Player *player)
{
    this->player = player;
}

Player* Dungeon::getPlayer()
{
    return player;
}

Room* Dungeon::enterRoom(int num)
{
    Room *room = getRoom(num);
    if (room != nullptr) {
        if (previousRoom != num) {
            cout << "Entering the " << room->getName() << "..." << endl;
            previousRoom = currentRoom;
            currentRoom = num;
        }
        displayText(room->describe());
        displayText(listActions(room));
    }
    else {
        cout << "That room doesn't exist!\n";
    }

    return room;
}

int Dungeon::handleEndGame()
{
    // TODO
    cout << "(TODO) Handle end game logic...\n";

    return 0;
}

void Dungeon::print(ostream* os)
{
    int ilen = sizeof(to_string(getId()));
    int rlen = sizeof(to_string(getNumberOfRooms()));
    int alen = sizeof(to_string(this->rooms.size()));
    int clen = sizeof(to_string(getCurrentRoom()));
    int plen = (getPlayer() != NULL ? 3 : 2);

    int bsize = 85 + sizeof(getName().c_str()) + ilen + rlen + alen + clen + plen;
    char buff[bsize];
    sprintf(buff, "Dungeon{ID: %d, Title: %s, Num rooms (actual/max): %d/%d, Current room: %d, Has player: %s}",
            getId(), getName().c_str(), (int)this->rooms.size(), getNumberOfRooms(), getCurrentRoom(), (plen == 3 ? "yes" : "no"));
    *os << (string)buff;
}

bool Dungeon::doAction(string action, string actionType)
{
    return (action == actionType || action == actionType.substr(0, 1));
}

bool Dungeon::isValidAction(string action, Room* room)
{
    bool isValid = true;

    string validActions = listActions(room);
    if (validActions.find(action) == string::npos) {
        size_t pos = validActions.find(":");
        if (pos != string::npos) {
            validActions = trim(validActions.erase(0, pos));
        }

        string shortActions;
        do {
            pos = validActions.find(" ");
            if (pos == string::npos) {
                shortActions += validActions.substr(0, 1);
            }
            else {
                shortActions += validActions.substr(0, pos).substr(0, 1);
                validActions.erase(0, pos+1);
            }
        }
        while (pos != string::npos);
        isValid = shortActions.find(action) != string::npos;
    }

    return isValid;
}

string Dungeon::listActions(Room* room)
{
    string actionList = room->listActions();
    int invCount = player->getInventory().size();
    if (invCount > 0) {
        for (int i = 0; i < invCount; i++) {
            if (player->getInventory()[i]->isUsable()) {
                appendText(actionList, ACTION_USE);
                break;
            }
        }
        appendText(actionList, ACTION_INVENTORY);
    }
    appendText(actionList, ACTION_QUIT);

    return actionList;
}

bool Dungeon::handleAction(string action, Room* room)
{
    bool gameOver = false;

    if (doAction(action, ACTION_ATTACK)) {
        gameOver = player->attack(room);
    }
    else if (doAction(action, ACTION_LOOK)) {
        displayText(room->look());
    }
    else if (doAction(action, ACTION_MOVE)) {
        changeRooms(room);
    }
    else if (doAction(action, ACTION_RUN)) {
        gameOver = player->flee(room);
    }
    else if (doAction(action, ACTION_TAKE)) {
        player->collectItems(room);
        displayText("Inventory: " + player->getInventoryNames());
    }
    else if (doAction(action, ACTION_USE)) {
        player->useItem(room);
    }
    else if (doAction(action, ACTION_INVENTORY)) {
        displayText("Inventory: " + player->getInventoryNames());
    }

    return gameOver;
}

void Dungeon::changeRooms(Room *room)
{
    vector<string> positions = room->getConnections();
    if (positions.size() == 1) {
        currentRoom = stoi(positions[0]);
    }
    else {
        string roomPos;
        while (true) {
            bool hasChoice = false;
            for (int i = 0; i < positions.size(); i++) {
                Room *room = getRoom(stoi(positions[i]));
                if (room) {
                    cout << room->getPosition() << ". " << room->getName() << endl;
                    hasChoice = true;
                }
            }

            if (hasChoice) {
                cout << "Move to which room number? ";
                cin >> roomPos;

                for (int i = 0; i < positions.size(); i++) {
                    if (positions[i] == roomPos) {
                        currentRoom = stoi(roomPos);
                        goto end_input_loop;
                    }
                }
                cout << roomPos << " is not a valid room number." << endl;
            }
        }
        end_input_loop: cout << endl;
    }
}

void Dungeon::displayText(string text)
{
    string line = "";

    size_t pos;
    do {
        pos = text.find(" ");
        if (pos == string::npos) {
            appendText(line, text);
        }
        else {
            appendText(line, text.substr(0, pos));
            text.erase(0, pos+1);
        }

        if (line.length() >= 60 || pos == string::npos) {
            cout << line << endl;
            line = "";
        }
    }
    while (pos != string::npos);
}
