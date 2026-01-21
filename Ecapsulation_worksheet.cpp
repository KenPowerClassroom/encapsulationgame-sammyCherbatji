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
};

class Enemy : public Character 
{
public:
    Enemy(const std::string& name, int EnemyHealth, int characterStrength)
    :Character(name, EnemyHealth, characterStrength) {}
};


class GameManager 
{
private:
    Player& player;
    Enemy& enemy;
    std::vector<Weapon> weapons;

    void equipRandom(Character& character) 
    {
        if (weapons.empty())
        {
            return;
        }

        int index = std::rand() % weapons.size();
        character.equip(weapons[index]);
    }

    void maybeHealPlayer() 
    {
        if (std::rand() % 3 == 0) 
        {
            player.heal((std::rand() % 20) + 1);
        }
    }

public:
    GameManager( Player& p, Enemy& e)
        : player(p), enemy(e) 
    {
        std::srand(std::time(0));
    }

    void addWeapon(const Weapon& weapon) 
    {
        weapons.push_back(weapon);
    }

    void startGame() 
    {
        std::cout << "Game started: " << player.getName() << " vs " << enemy.getName() << "\n";

        equipRandomWeapon(player);
        equipRandomWeapon(enemy);

        while (player.isAlive() && enemy.isAlive()) 
        {
            player.attack(enemy);
            if (!enemy.isAlive())
            {
                break;
            }

            enemy.attack(player);
            maybeHealPlayer();

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


    Weapon* equipRandomWeapon(Character& character) 
    {

        if (weapons.empty()) 
        {
            return nullptr;
        }

        int randomIndex = std::rand() % weapons.size();
        Weapon* selectedWeapon = &weapons[randomIndex];
        character.equip(*selectedWeapon);

        return selectedWeapon;
    }

    void randomlyHealPlayer() 
    {
        int healAmount = std::rand() % 50 + 1; // heal between 1 and 50 point
        player.heal(healAmount);
    }
};

// Main Function
int main() 
{
    Player player("Hero", 300, 2);
    Enemy enemy("Goblin", 150, 4);

    Weapon sword("Sword", 15);
    Weapon axe("Axe", 20);
    Weapon dagger("Dagger", 10);
    Weapon bow("Bow", 25);

    GameManager game(player, enemy);

    game.addWeapon(sword);
    game.addWeapon(axe);
    game.addWeapon(dagger);
    game.addWeapon(bow);

    // Equip weapons
    game.equipRandomWeapon(player); // Equip sword to player
    game.equipRandomWeapon(enemy);  // Equip axe to enemy


    game.startGame();

    return 0;
}
