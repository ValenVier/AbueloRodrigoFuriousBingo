#pragma once
#include "Enemy.h"

// PATTERN: Factory + Dependency Injection
// Factory receives the pool via constructor instead of being a static utility.
// This allows swapping the pool without touching the Factory internals.

class EnemyFactory {
public:
    // Dependency injection, Factory holds a reference to the pool it writes into
    explicit EnemyFactory(EnemyPool& pool) : pool_(pool) {}

    // Spawn + configure in one call, caller only needs type and position
    Enemy* Spawn(EnemyType type, Vector2 position, bool isFinalBoss = false);

private:
    EnemyPool& pool_;  // injected, Factory does not own this
    static void Configure(Enemy& e, EnemyType type, bool isFinalBoss);
};

/*// PATTERN: Factory (one place that knows all enemy stats)
// Callers never set stats manually; they always go through Configure()
#include "Enemy.h"

class EnemyFactory {
public:
    // Fill an already spawned Enemy slot with the correct stats for "type"
    // The caller sets position before or after (Factory only handles stats)
    static void Configure(Enemy& e,
        EnemyType type,
        bool isFinalBoss = false);

private:
    EnemyFactory() = delete; // static only utility (never instantiated)
};*/