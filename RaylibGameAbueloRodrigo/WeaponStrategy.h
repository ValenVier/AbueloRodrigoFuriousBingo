#pragma once
// PATTERN: Strategy (WeaponStrategy is the abstract base)
// Each concrete weapon is a subclass that implements Fire()
// Player only ever calls Fire() (it never knows which weapon it holds)
#include "Bullet.h"
#include "raylib.h"
#include <string>

// Abstract base
class WeaponStrategy {
    public:
        virtual ~WeaponStrategy() = default;

        // Fire one shot cycle into the bullet pool.
        // playerPos : world position of the player center
        // moveDir   : normalised movement direction
        // mousePos  : cursor position (used by some weapons)
        // pool      : shared bullet pool — weapon writes into it
        // dmgMult   : global damage multiplier from upgrades
        // frMult    : fire-rate multiplier (<1 = faster)
        virtual void Fire(Vector2 playerPos,
            Vector2 moveDir,
            Vector2 mousePos,
            BulletPool& pool,
            float dmgMult,
            float frMult) = 0;

        virtual const char* GetName() const = 0;
        virtual const char* GetDescription() const = 0;
        virtual float GetFireRate() const = 0; // base seconds between shots

        // Called every frame (accumulates dt and calls Fire() when ready)
        // Subclasses do NOT override this; they only override Fire()
        void Update(float dt,
            Vector2 playerPos,
            Vector2 moveDir,
            Vector2 mousePos,
            BulletPool& pool,
            float dmgMult,
            float frMult);

        float fireTimer = 0.f; // time since last shot
};

// Weapon 1: CaneLaser
// Single fast cyan beam in the movement direction.
class CaneLaser : public WeaponStrategy {
    public:
        void Fire(Vector2 playerPos,
            Vector2 moveDir,
            Vector2 mousePos,
            BulletPool& pool,
            float dmgMult,
            float frMult) override;

        const char* GetName() const override { return "Cane Laser"; }
        const char* GetDescription() const override { return "Fast energy beam"; }
        float GetFireRate()  const override { return 1.0f; } //0.25 too fast
};

// Weapon 2: FlyingDentures
// Fires chattering teeth toward mouse (bounces off screen edges up to 3 times)
class FlyingDentures : public WeaponStrategy {
public:
    void Fire(Vector2 playerPos, Vector2 moveDir, Vector2 mousePos,
        BulletPool& pool, float dmgMult, float frMult) override;
    const char* GetName()        const override { return "Flying Dentures"; }
    const char* GetDescription() const override { return "Bounces off walls x3"; }
    float       GetFireRate()    const override { return 0.7f; }
};

// Weapon 3: ExplosivePills
// Lobs a pill in an arc (explodes on contact with ground or enemy)
class ExplosivePills : public WeaponStrategy {
public:
    void Fire(Vector2 playerPos, Vector2 moveDir, Vector2 mousePos,
        BulletPool& pool, float dmgMult, float frMult) override;
    const char* GetName()        const override { return "Explosive Pills"; }
    const char* GetDescription() const override { return "Arc shot, explodes on impact"; }
    float       GetFireRate()    const override { return 1.8f; }
};

// Weapon 4: PureeCatapult
// Fires 6 mashed potato blobs in a spread (slows enemies on hit)
class PureeCatapult : public WeaponStrategy {
public:
    void Fire(Vector2 playerPos, Vector2 moveDir, Vector2 mousePos,
        BulletPool& pool, float dmgMult, float frMult) override;
    const char* GetName()        const override { return "Puree Catapult"; }
    const char* GetDescription() const override { return "6 blobs, slows 50%"; }
    float       GetFireRate()    const override { return 0.9f; }
};

// Weapon 5: BingoSoundWave
// Emits an expanding gold shockwave that hits all enemies it touches
class BingoSoundWave : public WeaponStrategy {
public:
    void Fire(Vector2 playerPos, Vector2 moveDir, Vector2 mousePos,
        BulletPool& pool, float dmgMult, float frMult) override;
    const char* GetName()        const override { return "Bingo Sound Wave"; }
    const char* GetDescription() const override { return "Expanding ring, hits all"; }
    float       GetFireRate()    const override { return 1.2f; }
};

// Weapon 6: TurboCharge
// High speed force projectile (knocks enemies back 80px on hit)
class TurboCharge : public WeaponStrategy {
public:
    void Fire(Vector2 playerPos, Vector2 moveDir, Vector2 mousePos,
        BulletPool& pool, float dmgMult, float frMult) override;
    const char* GetName()        const override { return "Turbo Charge"; }
    const char* GetDescription() const override { return "Knockback 80px on hit"; }
    float       GetFireRate()    const override { return 2.0f; }
};

// Weapon 7: CandyRain
// Fires 12 candy bullets in random 360 degree directions
class CandyRain : public WeaponStrategy {
public:
    void Fire(Vector2 playerPos, Vector2 moveDir, Vector2 mousePos,
        BulletPool& pool, float dmgMult, float frMult) override;
    const char* GetName()        const override { return "Candy Rain"; }
    const char* GetDescription() const override { return "12 candies in 360 degrees"; }
    float       GetFireRate()    const override { return 0.6f; }
};