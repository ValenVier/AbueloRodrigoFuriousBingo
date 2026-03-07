#include "WeaponStrategy.h"
#include <cmath>

// Base timer logic
void WeaponStrategy::Update(float dt,
    Vector2 playerPos,
    Vector2 moveDir,
    Vector2 mousePos,
    BulletPool& pool,
    float dmgMult,
    float frMult) {
    // frMult < 1 means faster fire rate
    float effectiveRate = GetFireRate() * frMult;

    fireTimer += dt;
    if (fireTimer >= effectiveRate) {
        fireTimer -= effectiveRate; // subtract instead of reset to avoid drift
        Fire(playerPos, moveDir, mousePos, pool, dmgMult, frMult);
    }
}

// Helper: normalise a 2D vector 
static Vector2 Normalize(Vector2 v) {
    float len = sqrtf(v.x * v.x + v.y * v.y);
    if (len < 0.0001f) return { 1.f, 0.f }; // fallback: shoot right
    return { v.x / len, v.y / len };
}

// CaneLaser::Fire
void CaneLaser::Fire(Vector2 playerPos,
    Vector2 moveDir,
    Vector2 mousePos,
    BulletPool& pool,
    float dmgMult,
    float frMult) {
    Bullet* b = pool.Spawn();
    if (!b) return; // pool exhausted (skip this shot silently)

    b->position = playerPos;
    b->origin = playerPos;
    b->velocity = { moveDir.x * 550.f, moveDir.y * 550.f };
    b->damage = 12.f * dmgMult;
    b->radius = 5.f;
    b->color = SKYBLUE;
    b->bulletType = BulletType::STANDARD;
    b->maxLifetime = 2.f;
}