#include "WeaponStrategy.h"
#include <cmath>

static const float M_PI = 3.14159265f;

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

// FlyingDentures::Fire
void FlyingDentures::Fire(Vector2 playerPos, Vector2 moveDir, Vector2 mousePos,
    BulletPool& pool, float dmgMult, float frMult) {
    Bullet* b = pool.Spawn();
    if (!b) return;

    Vector2 dir = Normalize({ mousePos.x - playerPos.x, mousePos.y - playerPos.y });
    b->position = playerPos;
    b->origin = playerPos;
    b->velocity = { dir.x * 380.f, dir.y * 380.f };
    b->damage = 18.f * dmgMult;
    b->radius = 8.f;
    b->color = WHITE;
    b->bulletType = BulletType::BOUNCING;
    b->bounceCount = 0;
    b->maxLifetime = 5.f;
}

// ExplosivePills::Fire
void ExplosivePills::Fire(Vector2 playerPos, Vector2 moveDir, Vector2 mousePos,
    BulletPool& pool, float dmgMult, float frMult) {
    Bullet* b = pool.Spawn();
    if (!b) return;

    Vector2 dir = Normalize({ mousePos.x - playerPos.x, mousePos.y - playerPos.y });
    b->position = playerPos;
    b->origin = playerPos;
    // Arc: horizontal speed toward mouse, upward initial velocity
    b->velocity = { dir.x * 300.f, dir.y * 300.f - 150.f };
    b->damage = 45.f * dmgMult;
    b->radius = 8.f;
    b->color = PINK;
    b->bulletType = BulletType::GRAVITY;
    b->explosionRadius = 55.f;
    b->maxLifetime = 3.f;
}

// PureeCatapult::Fire
void PureeCatapult::Fire(Vector2 playerPos, Vector2 moveDir, Vector2 mousePos,
    BulletPool& pool, float dmgMult, float frMult) {
    Vector2 dir = Normalize({ mousePos.x - playerPos.x, mousePos.y - playerPos.y });
    float   baseAngle = atan2f(dir.y, dir.x);

    // 6 blobs spread across 80 degrees centered on mouse direction
    for (int i = 0; i < 6; i++) {
        Bullet* b = pool.Spawn();
        if (!b) continue;

        float spread = (-40.f + (80.f / 5.f) * i) * (M_PI / 180.f);
        float angle = baseAngle + spread;

        b->position = playerPos;
        b->origin = playerPos;
        b->velocity = { cosf(angle) * 260.f, sinf(angle) * 260.f };
        b->damage = 8.f * dmgMult;
        b->radius = 7.f;
        b->color = { 255, 253, 208, 255 }; // cream
        b->bulletType = BulletType::SLOWING;
        b->slowDuration = 2.f;
        b->maxLifetime = 2.5f;
    }
}

// BingoSoundWave::Fire
void BingoSoundWave::Fire(Vector2 playerPos, Vector2 moveDir, Vector2 mousePos,
    BulletPool& pool, float dmgMult, float frMult) {
    Bullet* b = pool.Spawn();
    if (!b) return;

    b->position = playerPos;
    b->origin = playerPos;
    b->velocity = { 0.f, 0.f };   // shockwave doesn't move, only expands
    b->damage = 15.f * dmgMult;
    b->radius = 10.f;          // starts small, grows each frame
    b->color = GOLD;
    b->bulletType = BulletType::SHOCKWAVE;
    b->expansionSpeed = 120.f;
    b->maxLifetime = 4.f;
    for (int i = 0; i < 200; i++) b->shockwaveHit[i] = false;
}

// TurboCharge::Fire
void TurboCharge::Fire(Vector2 playerPos, Vector2 moveDir, Vector2 mousePos,
    BulletPool& pool, float dmgMult, float frMult) {
    Bullet* b = pool.Spawn();
    if (!b) return;

    b->position = playerPos;
    b->origin = playerPos;
    b->velocity = { moveDir.x * 600.f, moveDir.y * 600.f };
    b->damage = 30.f * dmgMult;
    b->radius = 12.f;
    b->color = ORANGE;
    b->bulletType = BulletType::KNOCKBACK;
    b->knockbackForce = 80.f;
    b->maxLifetime = 2.f;
}

// CandyRain::Fire
void CandyRain::Fire(Vector2 playerPos, Vector2 moveDir, Vector2 mousePos,
    BulletPool& pool, float dmgMult, float frMult) {
    static const Color candyColors[] = {
        RED, YELLOW, GREEN, BLUE, PURPLE, PINK, ORANGE
    };

    for (int i = 0; i < 12; i++) {
        Bullet* b = pool.Spawn();
        if (!b) continue;

        float angle = ((float)rand() / RAND_MAX) * 2.f * M_PI;
        b->position = playerPos;
        b->origin = playerPos;
        b->velocity = { cosf(angle) * 300.f, sinf(angle) * 300.f };
        b->damage = 6.f * dmgMult;
        b->radius = 5.f;
        b->color = candyColors[rand() % 7];
        b->bulletType = BulletType::STANDARD;
        b->maxLifetime = 2.f;
    }
}