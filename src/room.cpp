#include "room.h"

void appendText(string &text, string appendx)
{
    if (appendx != "" && text != "" && !endsWith(text, "\n")) text.append(" ");
    text.append(appendx);
}

Room::Room(room_config config, Config *cfg)
    : ZObject(config.name)
{
    this->config = config;
    populateItems(cfg);
    populateEnemies(cfg);
}

bool Room::hasExit()
{
    return config.isExit;
}

bool Room::hasConnections()
{
    bool isConnected = false;
    for (int i = 0; i < config.connections.size(); i++) {
        int roomPos = stoi(config.connections[i]);
        if (roomPos > 0) {
            isConnected = true;
            break;
        }
    }

    return isConnected;
}

bool Room::hasEnemies()
{
    return enemies.size() > 0;
}

bool Room::hasItems()
{
    return items.size() > 0;
}

int Room::getPosition()
{
    return config.position;
}

vector<string> Room::getConnections()
{
    return config.connections;
}

Item* Room::getItem(string name)
{
    return items[name];
}

vector<string> Room::getItemNames()
{
    return config.items;
}

Character* Room::getEnemy(string name)
{
    return enemies[name];
}

vector<string> Room::getEnemyNames()
{
    return config.enemies;
}

void Room::removeItem(string name)
{
    items.erase(name);
    for (int i = 0; i < config.items.size(); i++) {
        if (config.items[i] == name) {
            config.items.erase(config.items.begin()+i);
            break;
        }
    }
}

void Room::removeEnemy(string name)
{
    enemies.erase(name);
    for (int i = 0; i < config.enemies.size(); i++) {
        if (config.enemies[i] == name) {
            config.enemies.erase(config.enemies.begin()+i);
            break;
        }
    }
}

void Room::clearItems()
{
    items.clear();
}

void Room::clearEnemies()
{
    enemies.clear();
}

string Room::describe()
{
    string text = "";
    if (config.description != "") {
        appendText(text, config.description);
    }
    if (hasItems()) {
        appendText(text, getItemsText(false));
    }
    if (hasEnemies()) {
        appendText(text, getEnemiesText());
    }

    return text;
}

string Room::look()
{
    string text = "";
    if (config.lookText != "") {
        text.append(config.lookText);
    }
    if (hasItems()) {
        appendText(text, "\n");
        appendText(text, getItemsText(true));
    }

    return text;
}

string Room::listActions()
{
    string actionList = "Actions:";
    if (hasExit()) {
        appendText(actionList, ACTION_EXIT);
    }
    if (look().length() > 0) {
        appendText(actionList, ACTION_LOOK);
    }
    if (hasConnections()) {
        appendText(actionList, ACTION_MOVE);
    }
    if (hasEnemies()) {
        appendText(actionList, ACTION_ATTACK);
        appendText(actionList, ACTION_RUN);
    }
    if (hasItems()) {
        for (int i = 0; i < getItemNames().size(); i++) {
            if (getItem(getItemNames()[i])->isLootable()) {
                appendText(actionList, ACTION_TAKE);
                break;
            }
        }
    }

    return actionList;
}

void Room::print(ostream *os)
{
    int ilen = sizeof(to_string(getId()));
    int plen = sizeof(to_string(getPosition()));
    int elen = (hasExit() ? 3 : 2);
    int tlen = sizeof(to_string((int)getItemNames().size()));
    int nlen = sizeof(to_string((int)getEnemyNames().size()));

    int bsize = 75 + sizeof(getName().c_str()) + ilen + plen + elen + tlen + nlen;
    char buff[bsize];
    sprintf(buff, "Room{ID: %d, Name: %s, Position: %d, Is exit: %s, Item count: %d, Enemy count: %d}", getId(), getName().c_str(), getPosition(),
            (hasExit() ? "yes" : "no"), (int)getItemNames().size(), (int)getEnemyNames().size());
    *os << (string)buff;
}

string Room::getEnemiesText()
{
    string text = "";

    for (int i = 0; i < config.enemies.size(); i++) {
        appendText(text, getEnemy(config.enemies[i])->getDescription());
    }

    return text;
}

string Room::getItemsText(bool isLooking)
{
    string text = "";

    for (int i = 0; i < config.items.size(); i++) {
        appendText(text, (isLooking ? getItem(config.items[i])->getLookText() : getItem(config.items[i])->getDescription()));
    }

    return text;
}

void Room::populateEnemies(Config *cfg)
{
    stats_config enemy_config;
    for (int i = 0; i < getEnemyNames().size(); i++) {
        enemy_config = cfg->getEnemyStats(getEnemyNames()[i]);
        if (enemy_config.name.length() > 0) {
            Character *enemy = new Character(enemy_config);
            this->enemies[enemy_config.name] = enemy;
        }
    }
}

void Room::populateItems(Config *cfg)
{
    stats_config item_config;
    for (int i = 0; i < getItemNames().size(); i++) {
        item_config = cfg->getItemStats(getItemNames()[i]);
        if (item_config.name.length() > 0) {
            Item *item = new Item(item_config);
            this->items[item_config.name] = item;
        }
    }
}
