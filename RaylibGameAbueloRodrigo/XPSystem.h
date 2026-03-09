#pragma once
// Listens to OnEnemyKilled (spawns orbs and particles)
// PATTERN: Observer (reacts to events without coupling to CollisionSystem)
#include "Orb.h"
#include "Particle.h"
#include "GameEvents.h"

class XPSystem {
    public:
        // Subscribe to GameEvents,call once in PlayingState::Enter()
        void Init(OrbPool& orbPool, ParticlePool& particlePool);

        // Unsubscribe, call in PlayingState::Exit()
        void Shutdown();

        // Call every frame, handles orb magnet and collection
        void Update(float dt, Vector2 playerPos);

    private:
        OrbPool* orbPool_ = nullptr;
        ParticlePool* particlePool_ = nullptr;
        int killListenerId_ = -1;

        void OnEnemyKilled(const EnemyKilledData& data);
        void SpawnParticles(Vector2 pos, Color color, int count);
};