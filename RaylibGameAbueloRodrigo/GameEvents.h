#pragma once
// PATTERN: Observer (central event bus)
// PATTERN: Singleton (one global instance)
// All game events are here. Systems subscribe without knowing each other
#include "Event.h"
#include "raylib.h"

// Event payload structs 

struct EnemyKilledData {
    Vector2 position;
    int pointValue;
    int xpValue;
    Color color;
};

struct PlayerDamagedData {
    float damage;
    Vector2 position;
};

// GameEvents singleton

class GameEvents {
public:
    // PATTERN: Singleton
    static GameEvents& Instance() {
        static GameEvents inst;
        return inst;
    }

    Event<EnemyKilledData> OnEnemyKilled;
    Event<PlayerDamagedData> OnPlayerDamaged;

private:
    GameEvents() = default;
    GameEvents(const GameEvents&) = delete;
    GameEvents& operator=(const GameEvents&) = delete;
};