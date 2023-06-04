#include <algorithm>
#include <sstream>
#include "config.h"

bool startsWith(string text, string token)
{
    if (text.length() >= token.length() && text.substr(0, token.length()) == token) {
        return true;
    }

    return false;
}

bool endsWith(string text, string token)
{
    if (text.length() >= token.length() && text.substr(text.length()-token.length(), text.length()) == token) {
        return true;
    }

    return false;
}

string trim(string str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return NONE;
    }

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

vector<string> fromCSV(string csv)
{
    vector<string> list;

    size_t pos;
    do {
        pos = csv.find(",");
        if (pos == string::npos) {
            list.push_back(trim(csv));
        }
        else {
            list.push_back(trim(csv.substr(0, pos)));
            csv.erase(0, pos+1);
        }
    }
    while (pos != string::npos);

    return list;
}

string toCSV(vector<string> svec)
{
    string csv = "";

    for (int i = 0; i < svec.size(); i++) {
        if (i > 0) csv += ", ";
        csv += svec[i];
    }

    return csv;
}

bool toBool(string value)
{
    bool rval;

    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    istringstream is(value);
    is >> std::boolalpha >> rval;

    return rval;
}

Config::Config(string filePath)
    : ZObject(CONFIG_NAME)
{
    parseConfig(filePath);
}

string Config::getDungeonTitle()
{
    return title;
}

int Config::getNumberOfEnemies()
{
    return numEnemies;
}

int Config::getNumberOfItems()
{
    return numItems;
}

int Config::getNumberOfRooms()
{
    return numRooms;
}

stats_config Config::getEnemyStats(string name)
{
    return enemies[name];
}

stats_config Config::getItemStats(string name)
{
    return items[name];
}

room_config Config::getRoomConfig(int roomPos)
{
    return rooms[roomPos];
}

void Config::parseConfig(string filePath)
{
    auto cfg = ifstream(filePath.c_str());

    if (cfg.bad()) {
        cerr << "Could not open configuration file '" << filePath << "'!";
        return;
    }

    string line;
    while (getline(cfg, line)) {
        setConfigValue(split(line));
    }

    cfg.close();
}

void Config::print(ostream *os)
{
    int rlen = sizeof(to_string(getNumberOfRooms()));
    int elen = sizeof(to_string(getNumberOfEnemies()));
    int ilen = sizeof(to_string(getNumberOfItems()));

    int bsize = 80 + sizeof(getName().c_str()) + sizeof(getDungeonTitle().c_str()) + rlen + elen + ilen;
    char buff[bsize];
    sprintf(buff, "Config{%s - Title: %s, Number of Rooms: %d, Number of Enemies: %d, Number of Items: %d}", getName().c_str(), getDungeonTitle().c_str(), getNumberOfRooms(), getNumberOfEnemies(), getNumberOfItems());
    *os << (string)buff << "\n\n";

    *os << "Enemies:\n";
    for (int i = 1; i <= getNumberOfEnemies(); i++) {
        *os << i << ".\tName: " << enemies[to_string(i)].name << ", Health: " << enemies[to_string(i)].health
            << ", Attack: " << enemies[to_string(i)].attack << ", Defense: " << enemies[to_string(i)].defense << "\n";
    }
    cout << "\n";

    *os << "Items:\n";
    for (int i = 1; i <= getNumberOfItems(); i++) {
        *os << i << ".\tName: " << items[to_string(i)].name << ", Health: " << items[to_string(i)].health
            << ", Attack: " << items[to_string(i)].attack << ", Defense: " << items[to_string(i)].defense << "\n";
    }
    cout << "\n";

    *os << "Rooms:\n";
    room_config room;
    for (int i = 0; i < getNumberOfRooms(); i++) {
        room = rooms[i];
        *os << room.position << ".\tTitle: " << room.name << ", Is Exit: " << room.isExit << "\n";
        *os << "\tConnecting rooms: " << toCSV(room.connections) << "\n";
        *os << "\tEnemies: " << toCSV(room.enemies) << "\n";
        *os << "\tItems: " << toCSV(room.items) << "\n";
        *os << "\tRoom description: " << room.description << "\n";
        *os << "\tLook description: " << room.lookText << "\n";
    }
}

void Config::setConfigValue(key_value kvp)
{
    if (kvp.key == NONE) return;

    if (kvp.key == KEY_TITLE) {
        title = kvp.value;
    }
    else if (kvp.key == KEY_NUMBER_OF_ENEMIES) {
        numEnemies = atoi(kvp.value.c_str());
    }
    else if (kvp.key == KEY_NUMBER_OF_ITEMS) {
        numItems = atoi(kvp.value.c_str());
    }
    else if (kvp.key == KEY_NUMBER_OF_ROOMS) {
        numRooms = atoi(kvp.value.c_str());
        rooms.resize(numRooms);
    }
    else if (startsWith(kvp.key, KEY_ENEMY) && kvp.key.length() > KEY_ENEMY.length() + 2) {
        setObjectConfigValue(kvp, KEY_ENEMY);
    }
    else if (startsWith(kvp.key, KEY_ITEM) && kvp.key.length() > KEY_ITEM.length() + 2) {
        setObjectConfigValue(kvp, KEY_ITEM);
    }
    else if (startsWith(kvp.key, KEY_ROOM) && kvp.key.length() > KEY_ROOM.length() + 2) {
        setRoomConfigValue(kvp);
    }
}

void Config::setObjectConfigValue(key_value kvp, const string objectKey)
{
    // TODO: throw an exception
    if (objectKey != KEY_ENEMY && objectKey != KEY_ITEM) return;

    string objectNum = kvp.key.substr(objectKey.length()+1, 1);
    map<string, stats_config> *objectMap = (objectKey == KEY_ENEMY ? &enemies : &items);

    stats_config *config = &(*objectMap)[objectNum];
    bool updated = true;
    if (endsWith(kvp.key, ATTR_NAME)) {
        config->name = kvp.value;
    }
    else if (endsWith(kvp.key, ATTR_DESCRIPTION)) {
        config->description = kvp.value;
    }
    else if (endsWith(kvp.key, ATTR_LOOK_TEXT)) {
        config->lookText = kvp.value;
    }
    else if (endsWith(kvp.key, ATTR_HEALTH)) {
        config->health = atoi(kvp.value.c_str());
    }
    else if (endsWith(kvp.key, ATTR_ATTACK)) {
        config->attack = atoi(kvp.value.c_str());
    }
    else if (endsWith(kvp.key, ATTR_DEFENSE)) {
        config->defense = atoi(kvp.value.c_str());
    }
    else if (endsWith(kvp.key, ATTR_LOOTABLE)) {
        config->lootable = toBool(kvp.value);
    }
    else if (endsWith(kvp.key, ATTR_USABLE)) {
        config->usable = toBool(kvp.value);
    }
    else if (endsWith(kvp.key, ATTR_USABLE_ON)) {
        config->usableOn = kvp.value;
    }
    else if (endsWith(kvp.key, ATTR_USE_TEXT)) {
        config->useText = kvp.value;
    }
    else if (endsWith(kvp.key, ATTR_REQUIRES_ITEM)) {
        config->requiresItem = kvp.value;
    }
    else {
        updated = false;
    }

    if (updated && config->name.length() > 0) {
        (*objectMap)[config->name] = *config;
    }
}

void Config::setRoomConfigValue(key_value kvp)
{
    string roomNum = kvp.key.substr(KEY_ROOM.length()+1, 1);
    int roomPos = atoi(roomNum.c_str());

    // TODO: throw an exception
    if (roomPos < 1 || roomPos > getNumberOfRooms()) return;

    room_config *room = &rooms[roomPos-1];
    room->position = roomPos;

    if (endsWith(kvp.key, ATTR_NAME)) {
        room->name = kvp.value;
    }
    else if (endsWith(kvp.key, ATTR_DESCRIPTION)) {
        room->description = kvp.value;
    }
    else if (endsWith(kvp.key, ATTR_LOOK_TEXT)) {
        room->lookText = kvp.value;
    }
    else if (endsWith(kvp.key, ATTR_IS_EXIT)) {
        room->isExit = toBool(kvp.value);
    }
    else if (endsWith(kvp.key, ATTR_ENEMIES)) {
        vector<string> elist = fromCSV(kvp.value);
        for (int i = 0; i < elist.size(); i++) {
            room->enemies.push_back(elist[i]);
        }
    }
    else if (endsWith(kvp.key, ATTR_ITEMS)) {
        vector<string> ilist = fromCSV(kvp.value);
        for (int i = 0; i < ilist.size(); i++) {
            room->items.push_back(ilist[i]);
        }
    }
    else if (endsWith(kvp.key, ATTR_CONNECTIONS)) {
        vector<string> clist = fromCSV(kvp.value);
        for (int i = 0; i < clist.size(); i++) {
            room->connections.push_back(clist[i]);
        }
    }
}

key_value Config::split(string str)
{
    key_value kvp = {NONE, NONE};

    size_t pos = str.find("=");
    if (pos != string::npos) {
        kvp.key = trim(str.substr(0, pos));
        kvp.value = trim(str.substr(pos + 1, str.length()));
    }

    return kvp;
}
