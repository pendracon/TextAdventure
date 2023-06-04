#include "item.h"

Item::Item(): ZObject("") {}

Item::Item(stats_config config)
    : ZObject(config.name)
{
    this->config = config;
}

bool Item::hasDescription()
{
    return (config.description.length() > 0);
}

bool Item::hasLookText()
{
    return (config.lookText.length() > 0);
}

bool Item::isLootable()
{
    return config.lootable;
}

bool Item::isUsable()
{
    return config.usable;
}

string Item::getUsableOn()
{
    return config.usableOn;
}

string Item::getUseText()
{
    return config.useText;
}

string Item::getDescription()
{
    return config.description;
}

string Item::getLookText()
{
    return config.lookText;
}

int Item::getHealth()
{
    return config.health;
}

int Item::getAttack()
{
    return config.attack;
}

int Item::getDefense()
{
    return config.defense;
}

void Item::print(ostream *os)
{
    int hlen = sizeof(to_string(getHealth()));
    int alen = sizeof(to_string(getAttack()));
    int dlen = sizeof(to_string(getDefense()));

    int bsize = 50 + sizeof(getName().c_str()) + hlen + alen + dlen;
    char buff[bsize];
    sprintf(buff, "Item{ID: %d, Name: %s, Health: %d, Attack: %d, Defense: %d}", getId(), getName().c_str(), getHealth(), getAttack(), getDefense());
    *os << (string)buff;
    if (hasDescription()) {
        *os << endl << "Description: " << getDescription();
    }
    if (hasLookText()) {
        *os << endl << "Look text: " << getLookText();
    }
}
