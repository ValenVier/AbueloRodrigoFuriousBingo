#pragma once
// Handles all collision detection between bullets, enemies and player.
#include "Bullet.h"
#include "Enemy.h"
#include "raylib.h"

class Player; 

class CollisionSystem {
    public:
        // Run all collision checks for one frame.
        // Returns true if Rodrigo's HP reached zero this frame.
        bool Update(BulletPool& bullets,
            EnemyPool& enemies,
            Vector2 playerPos,
            float playerRadius,
            float dt);

    private:
        void CheckBulletsVsEnemies(BulletPool& bullets, EnemyPool& enemies);
        void CheckEnemiesVsPlayer(EnemyPool& enemies,
            Vector2 playerPos,
            float playerRadius,
            float dt);

        // Squared distance between two points (avoids sqrt — faster)
        static float Dist2(Vector2 a, Vector2 b) {
            float dx = a.x - b.x;
            float dy = a.y - b.y;
            return dx * dx + dy * dy;
        }
};