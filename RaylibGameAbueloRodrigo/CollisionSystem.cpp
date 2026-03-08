#include "CollisionSystem.h"
#include "GameManager.h"
#include "GameEvents.h"

bool CollisionSystem::Update(BulletPool& bullets,
    EnemyPool& enemies,
    Vector2 playerPos,
    float playerRadius,
    float dt) {
    CheckBulletsVsEnemies(bullets, enemies);
    CheckEnemiesVsPlayer(enemies, playerPos, playerRadius, dt);

    return GameManager::Instance().IsGameOver();
}

void CollisionSystem::CheckBulletsVsEnemies(BulletPool& bullets,
    EnemyPool& enemies) {
    for (auto& b : bullets.bullets) {
        if (!b.active || b.hitPlayer) continue;

        for (auto& e : enemies.enemies) {
            if (!e.active) continue;

            // Circle vs circle collision
            float minDist = b.radius + e.size;
            if (Dist2(b.position, e.position) > minDist * minDist) continue;

            // Hit!
            e.health -= b.damage;
            b.active = false;  // bullet consumed on hit

            if (e.health <= 0.f) {
                // PATTERN: Observer (fire event; score/xp systems will react)
                GameEvents::Instance().OnEnemyKilled.Fire({
                    e.position,
                    e.pointValue,
                    e.xpValue,
                    e.color
                    });

                GameManager::Instance().AddScore(e.pointValue);
                GameManager::Instance().enemiesKilled++;

                e.active = false;
            }

            break; // one bullet hits one enemy per frame
        }
    }
}

void CollisionSystem::CheckEnemiesVsPlayer(EnemyPool& enemies,
    Vector2 playerPos,
    float playerRadius,
    float dt) {
    auto& gm = GameManager::Instance();

    for (auto& e : enemies.enemies) {
        if (!e.active) continue;

        float minDist = e.size + playerRadius;
        if (Dist2(e.position, playerPos) > minDist * minDist) continue;

        // Deal damage per second (damage * dt so it's frame rate independent)
        float dmg = e.damage * dt;
        gm.health -= dmg;
        if (gm.health < 0.f) gm.health = 0.f;

        // PATTERN: Observer (fire event)
        GameEvents::Instance().OnPlayerDamaged.Fire({ dmg, playerPos });
    }
}