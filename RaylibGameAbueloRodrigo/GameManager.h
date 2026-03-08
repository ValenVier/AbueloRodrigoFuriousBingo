#pragma once
// PATTERN: Singleton (single source of truth for all global game state)
// HP, score, time (everyone reads/writes the same instance)

class GameManager {
    public:
        // PATTERN: Singleton
        static GameManager& Instance() {
            static GameManager inst;
            return inst;
        }

        void Reset();

        // Health
        float health = 100.f;
        float maxHealth = 100.f;

        // Score
        int score = 0;
        int enemiesKilled = 0;
        void AddScore(int pts) { score += pts; }

        // Time
        float elapsedTime = 0.f;

        bool IsGameOver() const { return health <= 0.f; }

    private:
        GameManager() = default;
        GameManager(const GameManager&) = delete;
        GameManager& operator=(const GameManager&) = delete;
};