#pragma once
// PATTERN: Singleton (single source for all global game state)
// HP, score, time (everyone reads/writes the same instance)

class GameManager {
    public:
        // PATTERN: Singleton
        static GameManager& Instance() {
            static GameManager inst;
            return inst;
        }

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

        bool IsGameOver() const { return health <= 0.f; }

    private:
        GameManager() = default;
        GameManager(const GameManager&) = delete;
        GameManager& operator=(const GameManager&) = delete;
};