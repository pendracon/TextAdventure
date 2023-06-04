#include "character.h"

Character::Character(): ZObject("") {}

Character::Character(stats_config config)
    : ZObject(config.name)
{
    maxHealth = currentHealth = config.health;
    this->config = config;
}

bool Character::hasDescription()
{
    return (config.description.length() > 0);
}

bool Character::hasLookText()
{
    return (config.lookText.length() > 0);
}

string Character::getDescription()
{
    return config.description;
}

string Character::getLookText()
{
    return config.lookText;
}

void Character::addItem(Item *item)
{
    inventory.push_back(item);
}

void Character::removeItem(Item *item)
{
    for (int i = 0; i < inventory.size(); i++) {
        if (inventory[i] == item) {
            inventory.erase(inventory.begin()+i);
            break;
        }
    }
}

vector<Item*> Character::getInventory()
{
    return inventory;
}

string Character::getInventoryNames()
{
    vector<string> itemNames;
    for (int i = 0; i < inventory.size(); i++) {
        itemNames.push_back(inventory[i]->getName());
    }
    string names = toCSV(itemNames);

    return names;
}

int Character::getMaxHealth()
{
    return maxHealth;
}

int Character::getCurrentHealth()
{
    return currentHealth;
}

int Character::getAttack()
{
    return config.attack;
}

int Character::getDefense()
{
    return config.defense;
}

int Character::takeDamage(int damage)
{
    int damageTaken = 0;

    if (damage > getDefense() && getCurrentHealth() > 0) {
        damageTaken = reduceHealth(damage);
    }

    return damageTaken;
}

bool Character::hasDied()
{
    return (getCurrentHealth() <= 0);
}

void Character::increaseHealth(int amount)
{
    currentHealth += amount;
    maxHealth += amount;
}

void Character::increaseAttack(int amount)
{
    config.attack += amount;
}

void Character::increaseDefense(int amount)
{
    config.defense += amount;
}

int Character::reduceHealth(int amount)
{
    int lostHealth = amount - getDefense();
    currentHealth -= lostHealth;

    return lostHealth;
}

void Character::print(ostream *os)
{
    int ilen = sizeof(to_string(getId()));
    int clen = sizeof(to_string(getCurrentHealth()));
    int mlen = sizeof(to_string(getMaxHealth()));
    int alen = sizeof(to_string(getAttack()));
    int dlen = sizeof(to_string(getDefense()));

    int bsize = 70 + sizeof(getName().c_str()) + ilen + clen + mlen + alen + dlen;
    char buff[bsize];
    sprintf(buff, "Character{ID: %d, Name: %s, Health (current/max): %d/%d, Attack: %d, Defense: %d}", getId(), getName().c_str(), getCurrentHealth(), getMaxHealth(), getAttack(), getDefense());
    *os << (string)buff;
    if (hasDescription()) {
        *os << endl << "Description: " << getDescription();
    }
    if (hasLookText()) {
        *os << endl << "Look text: " << getLookText();
    }
}
