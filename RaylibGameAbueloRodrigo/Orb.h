#pragma once
// PATTERN: Object Pool (XP orb dropped by enemies on death to collect XP)
#include "raylib.h"
#include <array>

struct Orb {
    Vector2 position = { 0.f, 0.f };
    float xpValue = 10.f;
    float radius = 6.f;
    float lifetime = 0.f;
    float maxLifetime = 12.f;  // disappears after 12s if not collected
    bool active = false;
    Color color = { 80, 220, 255, 255 };
};

constexpr int MAX_ORBS = 80;

class OrbPool {
public:
    Orb* Spawn();
    void DeactivateAll();
    std::array<Orb, MAX_ORBS> orbs;
};