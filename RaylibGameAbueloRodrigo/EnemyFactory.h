#pragma once
// PATTERN: Factory (one place that knows all enemy stats)
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
};