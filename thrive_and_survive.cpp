#include <iostream>
#include <string>
#include <format>
#include <vector>
#include <cctype>
#include <limits>
#include <thread>
#include <chrono>
#include <streambuf>

namespace ansi {

    // Reset
    constexpr const char* reset = "\033[0m\033[48;2;25;15;45m";

    // Text styles
    constexpr const char* bold      = "\033[1m";
    constexpr const char* dim       = "\033[2m";
    constexpr const char* italic    = "\033[3m";
    constexpr const char* underline = "\033[4m";
    constexpr const char* blink     = "\033[5m";
    constexpr const char* reverse   = "\033[7m";
    constexpr const char* hidden    = "\033[8m";

    // Standard foreground colors
    constexpr const char* black   = "\033[48;2;25;15;45m\033[30m";
    constexpr const char* red     = "\033[48;2;25;15;45m\033[31m";
    constexpr const char* green   = "\033[48;2;25;15;45m\033[32m";
    constexpr const char* yellow  = "\033[48;2;25;15;45m\033[33m";
    constexpr const char* blue    = "\033[48;2;25;15;45m\033[34m";
    constexpr const char* magenta = "\033[48;2;25;15;45m\033[35m";
    constexpr const char* cyan    = "\033[48;2;25;15;45m\033[36m";
    constexpr const char* white   = "\033[48;2;25;15;45m\033[37m";

    // Bright foreground colors
    constexpr const char* gray          = "\033[48;2;25;15;45m\033[90m";
    constexpr const char* brightRed     = "\033[48;2;25;15;45m\033[91m";
    constexpr const char* brightGreen   = "\033[48;2;25;15;45m\033[92m";
    constexpr const char* brightYellow  = "\033[48;2;25;15;45m\033[93m";
    constexpr const char* brightBlue    = "\033[48;2;25;15;45m\033[94m";
    constexpr const char* brightMagenta = "\033[48;2;25;15;45m\033[95m";
    constexpr const char* brightCyan    = "\033[48;2;25;15;45m\033[96m";
    constexpr const char* brightWhite   = "\033[48;2;25;15;45m\033[97m";

    // Cursor / screen controls
    constexpr const char* clearScreen =
    "\033[2J"
    "\033[3J"
    "\033[48;2;25;15;45m"
    "\033[H";
    constexpr const char* clearLine   = "\033[2K";
    constexpr const char* hideCursor  = "\033[?25l";
    constexpr const char* showCursor  = "\033[?25h";
}


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
    void ResetHealth();

    bool IsDead() const;

    std::string GetStats() const;

    int GetHP() const;
    int GetXP() const;
    int GetEP() const;
    int GetDamage() const;
    int GetLevel() const;
    int GetXPForLevel() const;
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

void Player::ResetHealth() {
    hp = maxHp;
}


bool Player::IsDead() const {
    return hp == 0;
}


std::string Player::GetStats() const {
    return std::format(R"(==============================
||           STATS          ||
==============================

HP: {}
XP: {} / {}
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
        GetXPForLevel(),
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

int Player::GetXPForLevel() const {
    return XPForLevel(level + 1);
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
            "Pirate",
            50,
            10,
            5,
            5,
            1
        );

        enemies.emplace_back(
            "Skeleton",
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
    std::cout << ansi::clearScreen;
    std::cout << std::format(R"(

{}Welcome,{} {}{}!{}

{}[fight] <- Fight{}
{}[stats] <- View Stats{}
{}[quit] <- Quit{}
{}[menu] <- View Menu{}

> )",   ansi::brightCyan, ansi::reset,

        ansi::brightYellow,
            playerName,
        ansi::reset,

        ansi::brightCyan, ansi::reset,

        ansi::brightYellow, ansi::reset,

        ansi::brightRed, ansi::reset,

        ansi::brightCyan, ansi::reset
    );
}


Enemy& Game::ShowFightMenu() {
    std::cout << ansi::clearScreen;

    std::cout << ansi::brightRed << std::format(R"(
╔══════════════════════════════════════════════════════════════════════╗
║                ███████╗██╗ ██████╗ ██╗  ██╗████████╗                 ║
║                ██╔════╝██║██╔════╝ ██║  ██║╚══██╔══╝                 ║
║                █████╗  ██║██║  ███╗███████║   ██║                    ║
║                ██╔══╝  ██║██║   ██║██╔══██║   ██║                    ║
║                ██║     ██║╚██████╔╝██║  ██║   ██║                    ║
║                ╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝                    ║
╚══════════════════════════════════════════════════════════════════════╝
    )") << ansi:reset;

    Wait(1000);

    std::cout << std::format(R"(
{}
Choose your enemy:{}

)", ansi::brightMagenta, ansi::reset
    );

    Wait(250);

    int index = 1;

    for (const Enemy& enemy : enemies) {

        std::cout
            << index
            << ". "
            << enemy.GetName();

        if (player.GetLevel() < enemy.GetRequiredLevel())
            std::cout << ansi::gray << " [LOCKED]" << ansi::reset;

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

            std::cout << ansi::brightRed << "Please enter a number.\n" << ansi::reset;

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
                << ansi::brightYellow << "That enemy is locked.\n" << ansi::reset;

        } else {

            std::cout
                << ansi::brightRed << "Invalid choice.\n" << ansi::reset;
        }
    }
}


void Game::Fight(Enemy& enemy) {

    enemy.ResetHealth();
    player.ResetHealth();

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    std::string action;
    std::cout << ansi::clearScreen;

    while (
        !player.IsDead() &&
        !enemy.IsDead()
    ) {
std::cout << ansi::clearScreen;

std::cout << std::format(R"(

{}========================
||{}  {}{}{} {}vs{} {}{}{}  {}||
========================{}

{}Your HP  {}: {}{}{}
{}Enemy HP {}: {}{}{}

{}{}[a] <- Attack{}
{}{}[esc] <- Escape{}

> )",
    ansi::brightYellow,
    ansi::reset,

    ansi::brightCyan,
    playerName,
    ansi::reset,

    ansi::brightWhite,
    ansi::reset,

    ansi::brightYellow,
    enemy.GetName(),
    ansi::reset,

    ansi::brightYellow,
    ansi::reset,

    ansi::brightCyan,
    ansi::brightCyan,
    player.GetHP(),
    ansi::reset,
    ansi::brightMagenta,
    ansi::reset,

    ansi::brightCyan,
    ansi::brightCyan,
    enemy.GetHP(),
    ansi::reset,
    ansi::brightMagenta,
    ansi::reset,

    ansi::brightGreen,
    ansi::reset,

    ansi::brightRed,
    ansi::reset
);
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

            std::cout << ansi::clearScreen;
            player.LoseXP(xpLoss);
            player.LoseEP(epLoss);


            std::cout << std::format(
    R"(
{}You fled the battle!{}

{}-{} XP{}
{}-{} EP{}

)",
    ansi::brightRed,
    ansi::reset,

    ansi::brightMagenta,
    xpLoss,
    ansi::reset,

    ansi::brightMagenta,
    epLoss,
    ansi::reset
);
            Wait(1500);
            std::cout << ansi::clearScreen;
            ShowMainMenu();

            return;
        }


        // ==============================
        // ATTACK VALIDATION
        // ==============================

        if (action != "a") {

            std::cout
                << ansi::brightRed << "Unknown command.\n" << ansi::reset;
            Wait(500);

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

        std::cout << ansi::clearScreen;
        std::cout << ansi::brightRed << "\nYou were defeated.\n" << ansi::reset;
        Wait(1500);

        ShowMainMenu();

    } else {

        player.GainXP(
            enemy.GetXPReward()
        );

        player.GainEP(
            enemy.GetEPReward()
        );

        std::cout << ansi::clearScreen;
        std::cout << ansi::brightGreen << std::format(
            R"(
Victory!

+{} XP
+{} EP

)",
            enemy.GetXPReward(),
            enemy.GetEPReward()
        ) << ansi::reset;
        Wait(1500);

        ShowMainMenu();
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

            std::cout << ansi::clearScreen;
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
                << ansi::brightMagenta << "\nThanks for playing!\n" << ansi::reset;

            break;
        }


        // ==============================
        // UNKNOWN COMMAND
        // ==============================

        else {

            std::cout
                << ansi::brightRed << "Unknown command. " << ansi::reset
                << ansi::brightCyan << "Type [menu] to see the commands.\n" << ansi::reset;
            Wait(500);
        }
    }
}


// ==============================
// MAIN
// ==============================

int main() {
    std::cout << ansi::clearScreen << ansi::clearScreen;

    std::cout << ansi::brightRed << R"(
╔═══════════════════════════════════════════════════════════════════════════════╗
║                                                                               ║
║  █████╗ ███████╗ ██████╗███████╗███╗   ██╗██████╗  █████╗ ███╗   ██╗████████╗ ║
║ ██╔══██╗██╔════╝██╔════╝██╔════╝████╗  ██║██╔══██╗██╔══██╗████╗  ██║╚══██╔══╝ ║
║ ███████║███████╗██║     █████╗  ██╔██╗ ██║██║  ██║███████║██╔██╗ ██║   ██║    ║
║ ██╔══██║╚════██║██║     ██╔══╝  ██║╚██╗██║██║  ██║██╔══██║██║╚██╗██║   ██║    ║
║ ██║  ██║███████║╚██████╗███████╗██║ ╚████║██████╔╝██║  ██║██║ ╚████║   ██║    ║
║ ╚═╝  ╚═╝╚══════╝ ╚═════╝╚══════╝╚═╝  ╚═══╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝    ║
║                                                                               ║
╚═══════════════════════════════════════════════════════════════════════════════╝

)" << ansi::reset;
    
    Wait(2000);

    std::cout << ansi::brightRed << "\n\nFight. " << std::flush;
    Wait(1500);
    std::cout << "Buy. " << std::flush;
    Wait(1500);
    std::cout << "Survive. " << ansi::reset << std::flush;  
    Wait(1500);

    std::cout << ansi::clearScreen;

    std::string playerName;


    std::cout
        << ansi::brightMagenta << "\nSalute, Captain! "
        << "What should we call you?\n" << ansi::reset << ansi::brightCyan << "> \a" << ansi::reset;


    std::getline(
        std::cin,
        playerName
    );


    if (playerName.empty())
        playerName = "Captain";


    Game game(playerName);

    game.Run();
}