#pragma once
#include "EnemyFactory.h"
#include "raylib.h"

class SpawnSystem {
public:
    // Dependency injection — SpawnSystem uses the factory, doesn't own it
    explicit SpawnSystem(EnemyFactory& factory) : factory_(factory) {}

    void Reset();
    void Update(float dt, float elapsedTime, Vector2 playerPos);

private:
    EnemyFactory& factory_;
    float spawnTimer_ = 0.f;

    Vector2  RandomEdgePosition(Vector2 playerPos) const;
    EnemyType PickEnemyType(float elapsedTime) const;
};


/*// It is decided when and where to spawn enemies based on elapsed game time
#include "Enemy.h"
#include "raylib.h"

class SpawnSystem {
public:
    void Reset();

    // Call every frame (spawns enemies into the pool when timers are ready)
    void Update(float dt,
        float elapsedTime,
        Vector2 playerPos,
        EnemyPool& enemyPool);

private:
    float spawnTimer_ = 0.f;

    // Returns a random position just outside one of the four screen edges
    Vector2 RandomEdgePosition(Vector2 playerPos) const;

    // Picks an enemy type based on how long the game has been running
    EnemyType PickEnemyType(float elapsedTime) const;
};*/