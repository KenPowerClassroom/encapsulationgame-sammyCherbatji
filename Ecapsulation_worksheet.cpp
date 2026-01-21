#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

class Weapon 
{
private:
    std::string name;
    int damage;

public:
    Weapon(const std::string& name, int weaponDamage)
    : name(name), damage(weaponDamage) {}

    int getDamage() const { return damage; }

    const std::string& getName() const 
    {
        return name;
    }

    int damageValue() const 
    {
        return damage;
    }

};


class Character 
{
public:
    std::string name;
    int health;
    int strength; // multiplier for weapon damage
    const Weapon* currentWeapon;

    void applyDamage(int amount) 
    {
        health -= amount;
        if (health < 0) 
        {
            health = 0;
        }
        std::cout << name << "take damage " << amount << "\n";
    }
    
	Character(const std::string& name, int characterHealth, int characterStrength)
        :  health(characterHealth), 
          strength(characterStrength), currentWeapon(nullptr) {}

    int getHealth() const { return health; }

    const std::string& getName() const 
    {
        return name;
    }

    virtual ~Character() = default;

    bool isAlive() const 
    {
        return health > 0;
    }

    void equip(const Weapon& w) 
    {
        currentWeapon = &w;
    }

    void attack(Character& target) const 
    {
        if (!currentWeapon)
        {
            return;
        }
        int damage = currentWeapon->damageValue() * strength;
        target.takeDamage(damage);
        std::cout << name << " attacks " << target.getName() << " with " << currentWeapon->getName() << " for " << damage << " damage.\n";
    }

    void takeDamage(int amount) 
    {
        applyDamage(amount);
    }

    void heal(int amount) 
    {
        if (isAlive()) 
        {
            health += amount;
        }
    }
};

class Player : public Character 
{
public:
    Player(const std::string& name, int playerHealth, int characterStrength)
    :Character(name,playerHealth, characterStrength) {}

public:
    void maybeHeal() 
    {
        if (shouldHeal()) 
        {
            int healAmount = randomHealAmount();
            heal(healAmount);
        }
    }

private:
    bool shouldHeal() const
    { 
        return std::rand() % 3 == 0;
    }
    int randomHealAmount() const 
    { 
        return (std::rand() % 20) + 1;
    }
};

class Enemy : public Character 
{
public:
    Enemy(const std::string& name, int EnemyHealth, int characterStrength)
    :Character(name, EnemyHealth, characterStrength) {}
};

class Armory
{
private:
    std::vector<Weapon> weapons;

public:
    void addWeapon(const Weapon& weapon)
    {
        weapons.push_back(weapon);
    }

    Weapon& getRandomWeapon()
    {
        int index = std::rand() % weapons.size();
        return weapons[index];
    }

    bool isEmpty() const
    {
        return weapons.empty();
    }
};

class BattleManager
{
private:
    Player& player;
    Enemy& enemy;
    Armory& armory;

public:
    BattleManager( Player& p, Enemy& e, Armory& a)
        : player(p), enemy(e), armory(a) {}

    void startBattle()
    {

        player.equip(armory.getRandomWeapon());
        enemy.equip(armory.getRandomWeapon());

        std::cout << "Battle  started: " << player.getName() << " vs " << enemy.getName() << "\n";

        while (player.isAlive() && enemy.isAlive()) 
        {
            player.attack(enemy);
            enemy.attack(player);
            player.maybeHeal();


            std::cout << "-----------------------\n";
        }

        if (!player.isAlive()) 
        {
            std::cout << player.getName() << " has been defeated.\n";
        }
        else if (!enemy.isAlive()) 
        {
            std::cout << enemy.getName() << " has been defeated.\n";
        }
    }
};

// Main Function
int main() 
{
    std::srand(std::time(0));

    Player player("Hero", 300, 2);
    Enemy enemy("Goblin", 150, 4);

    Armory armory;
    armory.addWeapon(Weapon("Sword", 15));
    armory.addWeapon(Weapon("Axe", 20));
    armory.addWeapon(Weapon("Dagger", 10));
    armory.addWeapon(Weapon("Bow", 25));

    BattleManager  game(player, enemy, armory);
    game.startBattle();

    return 0;
}
