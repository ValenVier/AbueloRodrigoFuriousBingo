#pragma once
// PATTERN: State (main gameplay state)
#include "GameState.h"
#include "raylib.h"
#include "Bullet.h"
#include "WeaponStrategy.h"
#include "Enemy.h"
#include "SpawnSystem.h"
#include "CollisionSystem.h" 
#include <memory>
#include <vector>
#include "Particle.h"
#include "Orb.h"
#include "XPSystem.h"


class PlayingState : public GameState {
    public:
        void Enter() override;
        void Exit() override;
        void Update(float dt) override;
        void Draw() const override;

    private:
        float pauseCooldown_ = 0.f;
        // Player position on screen
        Vector2 playerPos_ = { 400.f, 300.f };

        // Last non-zero movement direction 
        Vector2 lastMoveDir_ = { 1.f, 0.f };

        float speed_ = 180.f;
        float playerRadius_ = 16.f;

        ParticlePool particlePool_;
        OrbPool orbPool_;
        XPSystem xpSystem_;

        // Systems 
        BulletPool bulletPool_;
        EnemyPool enemyPool_;
        //SpawnSystem spawnSystem_;
        EnemyFactory enemyFactory_{ enemyPool_ };   // factory gets pool via injection
        SpawnSystem  spawnSystem_{ enemyFactory_ }; // spawnSystem gets factory via injection
        CollisionSystem collision_;
        float elapsedTime_ = 0.f; //total seconds since game started

        //std::unique_ptr<WeaponStrategy> currentWeapon_; //active weapon
        std::vector<std::unique_ptr<WeaponStrategy>> weapons_; // all unlocked weapons
        int currentWeaponIdx_ = 0; // active weapon index

        // PATTERN: Command (weapon swap actions decoupled form the keys that trigger them)
        void NextWeapon();
        void PrevWeapon();
        WeaponStrategy* CurrentWeapon() const;

        void UpdateBullets(float dt); // move bullets and deactivate old ones
        void UpdateEnemies(float dt); // move enemies toward player
        void DrawBullets() const; //render all active bullets
        void DrawEnemies() const; // render enemies + health bars
        void DrawHUD() const; // extracted to keep Draw() clean

        void UpdateParticles(float dt);
        void DrawParticles() const;
        void DrawOrbs() const;
        void DrawXPBar() const;
};