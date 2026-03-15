#pragma once
// PATTERN: Object Pool (Enemy is a plain data struct)
// EnemyPool owns a fixed array; no heap allocation during gameplay.
#include "raylib.h"
#include <array>

enum class EnemyType {
    NURSE, // Nurse; fast, low HP
    DOCTOR, // Doctor; medium, clipboard shield
    ADMIN,  // Admin; slow, calls reinforcements
    ORDERLY, // Orderly; charges and retreats
    CEOBOSS, // CEO Boss; 3 phases
};

// Substates for the Orderly charge/retreat behaviour
enum class OrderlyState { CHARGING, RETREATING, IDLE };

struct Enemy {
    Vector2 position = { 0.f, 0.f };
    float health = 50.f;
    float maxHealth = 50.f;
    float speed = 85.f;
    float baseSpeed = 85.f; // original speed before slow debuff
    float damage = 1.f; // damage per second on contact
    float size = 16.f;  // collision and draw radius
    int pointValue = 10;
    int xpValue = 12;
    bool active = false;
    Color color = WHITE;
    EnemyType type = EnemyType::NURSE;
    Vector2 retreatDir = { 0.f, 0.f };

    // Slow debuff timer — set by SLOWING bullets in stage 5
    float slowTimer = 0.f;

    // Ranged attack cooldown (nurse syringes, boss volleys)
    float attackTimer = 0.f;
    float attackCooldown = 3.f;

    // Admin reinforcement timer
    float reinforceTimer = 0.f;

    // Orderly charge/retreat
    OrderlyState orderlyState = OrderlyState::IDLE;
    float orderlyTimer = 0.f;
    bool orderlyHit = false;

    // Boss fields
    int bossPhase = 1;
    bool isFinalBoss = false;

    // Medico clipboard shield
    float clipboardAngle = 0.f;
    float clipboardCooldown = 0.f;
    bool clipboardActive = true;

    // Boss phase 2 puddle timer
    float puddleTimer = 0.f;

    // Brief invincibility flash on hit
    float invincTimer = 0.f;
};

// EnemyPool
constexpr int MAX_ENEMIES = 200;

class EnemyPool {
    public:
        // PATTERN: Object Pool — returns a free slot or nullptr if pool is full
        Enemy* Spawn();
        void DeactivateAll();
        int ActiveCount() const;

        std::array<Enemy, MAX_ENEMIES> enemies;
};