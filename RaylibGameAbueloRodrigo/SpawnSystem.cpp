#include "SpawnSystem.h"
#include "EnemyFactory.h"
#include <cstdlib>  // rand
#include <cmath>
#include <algorithm> // std::max

static const int SW = 800;
static const int SH = 600;

void SpawnSystem::Reset() {
    spawnTimer_ = 0.f;
}

void SpawnSystem::Update(float dt,
    float elapsedTime,
    Vector2 playerPos,
    EnemyPool& enemyPool) {
    // Spawn interval shrinks over time (gets harder as the game goes on)
    // Formula: starts at 3.0s, minimum 0.4s
    float interval = std::max(0.4f, 3.0f - elapsedTime * 0.008f);

    spawnTimer_ += dt;
    if (spawnTimer_ < interval) return; // not time yet
    spawnTimer_ = 0.f;

    // Number of enemies per wave increases every 20 seconds
    int count = 2 + (int)(elapsedTime / 20.f);

    for (int i = 0; i < count; i++) {
        Enemy* e = enemyPool.Spawn();
        if (!e) break; // pool exhausted — skip remaining spawns

        EnemyType type = PickEnemyType(elapsedTime);
        EnemyFactory::Configure(*e, type);
        e->position = RandomEdgePosition(playerPos);
    }
}

EnemyType SpawnSystem::PickEnemyType(float t) const {
    // Enemy types unlock progressively as the game goes on
    EnemyType pool[4];
    int count = 0;

    pool[count++] = EnemyType::NURSE; // 0s+
    if (t >= 20.f)  pool[count++] = EnemyType::ORDERLY; // 20s+
    if (t >= 45.f)  pool[count++] = EnemyType::DOCTOR; // 45s+
    if (t >= 80.f)  pool[count++] = EnemyType::ADMIN; // 80s+

    return pool[rand() % count];
}

Vector2 SpawnSystem::RandomEdgePosition(Vector2 playerPos) const {
    // Try up to 10 times to find a spawn point far enough from the player
    for (int attempt = 0; attempt < 10; attempt++) {
        Vector2 pos;
        int edge = rand() % 4;
        switch (edge) {
        case 0: pos = { (float)(rand() % SW), -20.f }; break; // top
        case 1: pos = { (float)(rand() % SW), (float)SH + 20 }; break; // bottom
        case 2: pos = { -20.f, (float)(rand() % SH) }; break; // left
        default: pos = { (float)SW + 20.f, (float)(rand() % SH) }; break; // right
        }

        // Must be at least 80px from the player starting area
        float dx = pos.x - playerPos.x;
        float dy = pos.y - playerPos.y;
        if (dx * dx + dy * dy >= 80.f * 80.f) return pos;
    }

    return { -20.f, -20.f }; // fallback: top-left corner offscreen
}