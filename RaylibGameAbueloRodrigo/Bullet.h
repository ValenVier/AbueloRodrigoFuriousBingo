#pragma once
// PATTERN: Object Pool (Bullet is a plain data struct)
// The pool owns a fixed array of these; no heap allocation during gameplay
#include "raylib.h"
#include <array>

// All possible bullet behaviours
enum class BulletType {
    STANDARD,   // straight line, no special behaviour
    BOUNCING,   // reflects off screen edges
    EXPLOSIVE,  // explodes on deactivation
    SLOWING,    // slows enemy on hit
    SHOCKWAVE,  // expands outward from player
    KNOCKBACK,  // pushes enemy back on hit
    GRAVITY,    // affected by gravity, arc trajectori
};

// Bullet data
struct Bullet {
    Vector2 position = { 0.f, 0.f };
    Vector2 velocity = { 0.f, 0.f };
    Vector2 origin = { 0.f, 0.f };  // spawn point (used for knockback direction)
    float damage = 10.f;
    float radius = 5.f;
    Color color = WHITE;
    bool active = false;
    BulletType bulletType = BulletType::STANDARD;

    // BOUNCING
    int bounceCount = 0;

    // SHOCKWAVE
    float expansionSpeed = 120.f;

    // EXPLOSIVE
    float explosionRadius = 55.f;

    // SLOWING
    float slowDuration = 2.f;

    // KNOCKBACK
    float knockbackForce = 80.f;

    // Lifetime (bullet deactivates when this exceeds maxLifetime)
    float lifetime = 0.f;
    float maxLifetime = 4.f;

    // If true, this bullet targets the player not enemies
    bool hitPlayer = false;

    // Per bullet hit flags for shockwave 
    bool shockwaveHit[200] = {};
};

// BulletPool
constexpr int MAX_BULLETS = 500;

class BulletPool {
    public:
        // PATTERN: Object Pool (returns a pointer to a free (inactive) slot)
        // Returns nullptr if all slots are in use (pool exhausted).
        Bullet* Spawn();

        void DeactivateAll();

        std::array<Bullet, MAX_BULLETS> bullets;
};