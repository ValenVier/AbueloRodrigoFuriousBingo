#pragma once
#include <vector>
// PATTERN: Singleton (single source for all global game state)
// HP, score, time (everyone reads/writes the same instance)

enum class Difficulty {
    EASY,
    MEDIUM,
    HARD
};

class GameManager {
    public:
        // PATTERN: Singleton
        static GameManager& Instance() {
            static GameManager inst;
            return inst;
        }

        Difficulty difficulty = Difficulty::MEDIUM;  // default

        void Reset();
        void AddXP(float amount);

        // Health
        float health = 100.f;
        float maxHealth = 100.f;

        // Score
        int score = 0;
        int enemiesKilled = 0;
        void AddScore(int pts) { score += pts; }

        // Time
        float elapsedTime = 0.f;

        // XP and level
        int  level = 1;
        float xp = 0.f;
        float xpToNextLevel = 50.f;
        int upgradePoints = 0;

        // Multipliers modified by upgrades
        float dmgMult = 1.0f;
        float frMult = 1.0f;  // <1 = faster fire rate
        float speedMult = 1.0f;
        float orbPickupRadius = 40.f;

        // Weapon unlock and confiscation system
        std::vector<int> unlockedWeaponSlots; // indices into the full weapon list
        std::vector<int> confiscatedSlots; // confiscated weapon indices
        bool hpThreshold75Hit = false;
        bool hpThreshold50Hit = false;
        bool hpThreshold25Hit = false;

        // Returns true if a weapon should be confiscated this frame
        bool CheckConfiscation();
        void UnlockNextWeapon(); // called on bingo line

        bool IsGameOver() const { return health <= 0.f; }

    private:
        GameManager() = default;
        GameManager(const GameManager&) = delete;
        GameManager& operator=(const GameManager&) = delete;
};