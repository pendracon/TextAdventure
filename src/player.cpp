#include "player.h"

Player::Player(stats_config config)
    : Character(config)
{
    currentLevel = 1;
}

int Player::getCurrentLevel()
{
    return currentLevel;
}

void Player::levelUp()
{
    increaseHealth(BASE_HEALTH_LEVEL_UP * getCurrentLevel());
    increaseAttack(BASE_ATTACK_LEVEL_UP * getCurrentLevel());
    increaseDefense(BASE_DEFENSE_LEVEL_UP * getCurrentLevel());
    currentLevel++;
}

bool Player::attack(Room *room)
{
    bool died = false;

    //TODO
    cout << "TODO: player attack..." << endl;

    return died;
}

bool Player::flee(Room *room)
{
    bool died = false;

    //TODO
    cout << "TODO: player flee..." << endl;

    return died;
}

void Player::collectItems(Room *room)
{
    vector<string> itemsList = room->getItemNames();
    for(int i = 0; i < itemsList.size(); i++) {
        if (room->getItem(itemsList[i])->isLootable()) {
            addItem(room->getItem(itemsList[i]));
            room->removeItem(itemsList[i]);
        }
    }
}

string Player::useItem(Room *room)
{
    // get the list of usable items for this room...
    vector<Item*> usableList;
    string usableOn, useText = "Nothing to do here.";
    for (int i = 0; i < inventory.size(); i++) {
        if (inventory[i]->isUsable()) {
            usableOn = inventory[i]->getUsableOn();
            if (usableOn == "Player") {
                usableList.push_back(inventory[i]);
            }
            else if (room->hasEnemies()) {
                for (int j = 0; j < room->getEnemyNames().size(); j++) {
                    if (usableOn == room->getEnemyNames()[j]) {
                        usableList.push_back(inventory[i]);
                    }
                }
            }
        }
    }

    // prompt the user for item to use, if any...
    Item *item = nullptr;
    while (usableList.size() > 0) {
        for (int i = 0; i < usableList.size(); i++) {
            cout << i+1 << ". " << usableList[i]->getName() << endl;
        }
        int inum;
        cout << "Use which item (1-" << usableList.size() << ")? ";
        cin >> inum;

        if (inum > 0 && inum <= usableList.size()) {
            item = usableList[inum-1];
            break;
        }
        else {
            cout << inum << " is not a valid choice." << endl;
        }
    }

    // "use" the item...
    if (item) {
        if (item->getUsableOn() == "Player") {
            increaseHealth(item->getHealth());
            increaseAttack(item->getAttack());
            increaseDefense(item->getDefense());
        }
        else {
            room->getEnemy(item->getUsableOn())->addItem(item);
        }
        useText = item->getUseText();
        removeItem(item);
    }

    cout << useText << endl;

    return useText;
}

void Player::print(ostream *os)
{
    *os << "Player{";
    Character::print(os);
    *os << ", Current level: " << getCurrentLevel() << "}";
}
