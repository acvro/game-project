#include <iostream>
#include <string>
#include <format>
#include <vector>
#include <cctype>
#include <limits>

class Player {
private:
    int hp;
    int ep;
    int speed;
    int defense;

    int xp;
    int level;

    int damage;
    int maxHp;

    int baseDamage;
    int baseMaxHp;

    int XPForLevel(int targetLevel) const;

public:
    Player()
        : hp(100),
          ep(100),
          speed(5),
          defense(0),
          xp(0),
          level(1),
          damage(10),
          maxHp(100),
          baseDamage(10),
          baseMaxHp(100) {}

    void TakeDamage(int amount);
    void Heal(int amount);
    void Die();

    void GainXP(int amount);
    void GainEP(int amount);

    void LevelUp();
    void LevelDown();

    void LoseXP(int amount);
    void LoseEP(int amount);

    void UpgradeDamage(int amount);
    void UpgradeSpeed(int amount);
    void UpgradeDefense(int amount);

    bool IsDead() const;

    std::string GetStats() const;

    int GetHP() const;
    int GetXP() const;
    int GetEP() const;
    int GetDamage() const;
    int GetLevel() const;
};


// ==============================
// PLAYER IMPLEMENTATION
// ==============================

int Player::XPForLevel(int targetLevel) const {
    if (targetLevel <= 1)
        return 0;

    int requiredXP = 0;

    for (int currentLevel = 1;
         currentLevel < targetLevel;
         currentLevel++) {

        requiredXP += currentLevel * 10;
    }

    return requiredXP;
}


void Player::TakeDamage(int amount) {
    if (amount < 0)
        amount = 0;

    int finalDamage =
        (amount - defense < 0)
        ? 0
        : amount - defense;

    hp =
        (hp - finalDamage < 0)
        ? 0
        : hp - finalDamage;
}


void Player::Heal(int amount) {
    if (amount < 0)
        amount = 0;

    hp =
        (hp + amount > maxHp)
        ? maxHp
        : hp + amount;
}


void Player::Die() {
    hp = 0;
}


void Player::GainXP(int amount) {
    if (amount <= 0)
        return;

    xp += amount;

    while (level < 100 &&
           xp >= XPForLevel(level + 1)) {

        LevelUp();
    }
}


void Player::GainEP(int amount) {
    if (amount <= 0)
        return;

    ep += amount;
}


void Player::LevelUp() {
    if (level >= 100)
        return;

    level++;

    maxHp = baseMaxHp + (level - 1) * 15;
    damage = baseDamage + (level - 1) * 2;

    hp = maxHp;
}


void Player::LevelDown() {
    if (level <= 1)
        return;

    level--;

    maxHp = baseMaxHp + (level - 1) * 15;
    damage = baseDamage + (level - 1) * 2;

    if (hp > maxHp)
        hp = maxHp;
}


void Player::LoseXP(int amount) {
    if (amount <= 0)
        return;

    xp =
        (xp - amount < 0)
        ? 0
        : xp - amount;

    while (level > 1 &&
           xp < XPForLevel(level)) {

        LevelDown();
    }
}


void Player::LoseEP(int amount) {
    if (amount <= 0)
        return;

    ep =
        (ep - amount < 0)
        ? 0
        : ep - amount;
}


void Player::UpgradeDamage(int amount) {
    if (amount <= 0)
        return;

    baseDamage += amount;
    damage += amount;
}


void Player::UpgradeSpeed(int amount) {
    if (amount <= 0)
        return;

    speed += amount;
}


void Player::UpgradeDefense(int amount) {
    if (amount <= 0)
        return;

    defense += amount;
}


bool Player::IsDead() const {
    return hp == 0;
}


std::string Player::GetStats() const {
    return std::format(R"(==============================
||           STATS          ||
==============================

HP: {}
XP: {}
Evo Points: {}
Level: {}

==============================
||         UPGRADES         ||
==============================

Defense: {}
Speed: {}
Damage: {})",
        hp,
        xp,
        ep,
        level,
        defense,
        speed,
        damage);
}


int Player::GetHP() const {
    return hp;
}


int Player::GetXP() const {
    return xp;
}


int Player::GetEP() const {
    return ep;
}


int Player::GetDamage() const {
    return damage;
}


int Player::GetLevel() const {
    return level;
}


// ==============================
// ENEMY CLASS
// ==============================

class Enemy {
private:
    std::string name;

    int hp;
    int maxHp;

    int damage;
    int xpReward;
    int epReward;
    int requiredLevel;

public:
    Enemy(
        std::string name,
        int hp,
        int damage,
        int xpReward,
        int epReward,
        int requiredLevel
    )
        : name(name),
          hp(hp),
          maxHp(hp),
          damage(damage),
          xpReward(xpReward),
          epReward(epReward),
          requiredLevel(requiredLevel) {}

    void TakeDamage(int amount);
    void ResetHealth();

    bool IsDead() const;

    int GetDamage() const;
    int GetXPReward() const;
    int GetEPReward() const;
    std::string GetName() const;
    int GetHP() const;
    int GetRequiredLevel() const;
};


// ==============================
// ENEMY IMPLEMENTATION
// ==============================

void Enemy::TakeDamage(int amount) {
    if (amount < 0)
        amount = 0;

    hp =
        (hp - amount < 0)
        ? 0
        : hp - amount;
}


void Enemy::ResetHealth() {
    hp = maxHp;
}


bool Enemy::IsDead() const {
    return hp == 0;
}


int Enemy::GetDamage() const {
    return damage;
}


int Enemy::GetXPReward() const {
    return xpReward;
}


int Enemy::GetEPReward() const {
    return epReward;
}


std::string Enemy::GetName() const {
    return name;
}


int Enemy::GetHP() const {
    return hp;
}


int Enemy::GetRequiredLevel() const {
    return requiredLevel;
}


// ==============================
// GAME CLASS
// ==============================

class Game {
private:
    Player player;
    std::string playerName;
    std::vector<Enemy> enemies;

    void ShowMainMenu();

public:
    Game(std::string playerName)
        : playerName(playerName) {

        enemies.emplace_back(
            "Slime",
            50,
            10,
            5,
            5,
            1
        );

        enemies.emplace_back(
            "Goblin",
            75,
            20,
            15,
            10,
            5
        );

        enemies.emplace_back(
            "Dire Wolf",
            95,
            24,
            20,
            15,
            8
        );

        enemies.emplace_back(
            "Skeleton Knight",
            140,
            30,
            30,
            20,
            12
        );

        enemies.emplace_back(
            "Stone Golem",
            220,
            42,
            50,
            35,
            18
        );

        enemies.emplace_back(
            "Crimson Knight",
            350,
            55,
            120,
            75,
            25
        );
    }

    void Run();

    Enemy& ShowFightMenu();
    void Fight(Enemy& enemy);
};


// ==============================
// GAME IMPLEMENTATION
// ==============================

void Game::ShowMainMenu() {
    std::cout << std::format(R"(

=================================
||         EVOLUTION          ||
=================================

Welcome, {}!

[fight]  Fight
[stats]  View Stats
[quit]   Quit
[menu]   View Menu

> )", playerName);
}


Enemy& Game::ShowFightMenu() {

    std::cout << R"(

===========================
||        FIGHT!         ||
===========================

Choose your enemy:

)";

    int index = 1;

    for (const Enemy& enemy : enemies) {

        std::cout
            << index
            << ". "
            << enemy.GetName();

        if (player.GetLevel() < enemy.GetRequiredLevel())
            std::cout << " [LOCKED]";

        std::cout << '\n';

        index++;
    }


    int choice;

    while (true) {

        std::cout << "\n> ";

        if (!(std::cin >> choice)) {

            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout << "Please enter a number.\n";

            continue;
        }


        if (
            choice >= 1 &&
            choice <= static_cast<int>(enemies.size())
        ) {

            Enemy& selected =
                enemies[choice - 1];


            if (
                player.GetLevel() >=
                selected.GetRequiredLevel()
            ) {
                return selected;
            }


            std::cout
                << "That enemy is locked.\n";

        } else {

            std::cout
                << "Invalid choice.\n";
        }
    }
}


void Game::Fight(Enemy& enemy) {

    enemy.ResetHealth();

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    std::string action;


    while (
        !player.IsDead() &&
        !enemy.IsDead()
    ) {

        std::cout << std::format(R"(

========================
{} vs {}
========================

Your HP : {}
Enemy HP: {}

[att] <- Attack
[esc] <- Escape from fight

> )",
            playerName,
            enemy.GetName(),
            player.GetHP(),
            enemy.GetHP());


        std::getline(
            std::cin,
            action
        );


        for (char& a : action) {

            a = static_cast<char>(
                std::tolower(
                    static_cast<unsigned char>(a)
                )
            );
        }


        // ==============================
        // ESCAPE
        // ==============================

        if (action == "esc") {

            int xpLoss =
                player.GetXP() / 5;

            int epLoss =
                player.GetEP() / 10;


            player.LoseXP(xpLoss);
            player.LoseEP(epLoss);


            std::cout << std::format(
                R"(
You fled the battle!

-{} XP
-{} EP

)",
                xpLoss,
                epLoss
            );


            return;
        }


        // ==============================
        // ATTACK VALIDATION
        // ==============================

        if (action != "att") {

            std::cout
                << "Unknown command.\n";

            continue;
        }


        // ==============================
        // PLAYER ATTACKS
        // ==============================

        enemy.TakeDamage(
            player.GetDamage()
        );


        if (enemy.IsDead())
            break;


        // ==============================
        // ENEMY ATTACKS
        // ==============================

        player.TakeDamage(
            enemy.GetDamage()
        );
    }


    // ==============================
    // BATTLE RESULT
    // ==============================

    if (player.IsDead()) {

        std::cout
            << "\nYou were defeated.\n";

    } else {

        player.GainXP(
            enemy.GetXPReward()
        );

        player.GainEP(
            enemy.GetEPReward()
        );


        std::cout << std::format(
            R"(
Victory!

+{} XP
+{} EP

)",
            enemy.GetXPReward(),
            enemy.GetEPReward()
        );
    }
}


// ==============================
// MAIN GAME LOOP
// ==============================

void Game::Run() {

    ShowMainMenu();

    std::string choice;


    while (true) {

        std::cin >> choice;


        // Convert command to lowercase

        for (char& c : choice) {

            c = static_cast<char>(
                std::tolower(
                    static_cast<unsigned char>(c)
                )
            );
        }


        // ==============================
        // FIGHT
        // ==============================

        if (choice == "fight") {

            Enemy& enemy =
                ShowFightMenu();

            Fight(enemy);
        }


        // ==============================
        // STATS
        // ==============================

        else if (choice == "stats") {

            std::cout
                << player.GetStats()
                << '\n';
        }


        // ==============================
        // MENU
        // ==============================

        else if (choice == "menu") {

            ShowMainMenu();
        }


        // ==============================
        // QUIT
        // ==============================

        else if (choice == "quit") {

            std::cout
                << "\nThanks for playing!\n";

            break;
        }


        // ==============================
        // UNKNOWN COMMAND
        // ==============================

        else {

            std::cout
                << "Unknown command. "
                << "Type [menu] to see the commands.\n";
        }
    }
}


// ==============================
// MAIN
// ==============================

int main() {

    std::string playerName;


    std::cout
        << "Salute, Captain! "
        << "What should we call you?\n> ";


    std::getline(
        std::cin,
        playerName
    );


    if (playerName.empty())
        playerName = "Captain";


    Game game(playerName);

    game.Run();
}