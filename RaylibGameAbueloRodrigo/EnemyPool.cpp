#include "Enemy.h"

Enemy* EnemyPool::Spawn() {
    for (auto& e : enemies) {
        if (!e.active) {
            e = Enemy{}; // reset all fields to defaults
            e.active = true;
            return &e;
        }
    }
    return nullptr; // pool exhausted
}

void EnemyPool::DeactivateAll() {
    for (auto& e : enemies) e.active = false;
}

int EnemyPool::ActiveCount() const {
    int count = 0;
    for (const auto& e : enemies)
        if (e.active) count++;
    return count;
}