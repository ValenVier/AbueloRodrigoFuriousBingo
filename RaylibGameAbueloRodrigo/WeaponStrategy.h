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