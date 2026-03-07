#include "Bullet.h"

Bullet* BulletPool::Spawn() {
    for (auto& b : bullets) {
        if (!b.active) {
            b = Bullet{};  // reset all fields to defaults
            b.active = true;
            return &b;
        }
    }
    return nullptr; // pool exhausted (caller must handle this)
}

void BulletPool::DeactivateAll() {
    for (auto& b : bullets) b.active = false;
}