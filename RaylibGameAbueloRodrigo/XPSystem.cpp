#include "XPSystem.h"
#include "GameManager.h"
#include <cmath>
#include <cstdlib>

void XPSystem::Init(OrbPool& orbPool, ParticlePool& particlePool) {
    orbPool_ = &orbPool;
    particlePool_ = &particlePool;

    // PATTERN: Observer, subscribe to enemy killed event
    killListenerId_ = GameEvents::Instance().OnEnemyKilled.Subscribe(
        [this](const EnemyKilledData& d) { OnEnemyKilled(d); }
    );
}

void XPSystem::Shutdown() {
    if (killListenerId_ >= 0) {
        GameEvents::Instance().OnEnemyKilled.Unsubscribe(killListenerId_);
        killListenerId_ = -1;
    }
}

void XPSystem::Update(float dt, Vector2 playerPos) {
    auto& gm = GameManager::Instance();

    for (auto& o : orbPool_->orbs) {
        if (!o.active) continue;

        o.lifetime += dt;
        if (o.lifetime >= o.maxLifetime) { o.active = false; continue; }

        float dx = playerPos.x - o.position.x;
        float dy = playerPos.y - o.position.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < gm.orbPickupRadius) {
            gm.AddXP(o.xpValue);
            GameEvents::Instance().OnOrbCollected.Fire({ o.position, o.xpValue });
            SpawnParticles(o.position, o.color, 5);
            o.active = false;
        }
    }
}

void XPSystem::OnEnemyKilled(const EnemyKilledData& data) {
    Orb* o = orbPool_->Spawn();
    if (o) {
        o->position = data.position;
        o->xpValue = (float)data.xpValue;
    }
    SpawnParticles(data.position, data.color, 12);
}

void XPSystem::SpawnParticles(Vector2 pos, Color color, int count) {
    for (int i = 0; i < count; i++) {
        Particle* p = particlePool_->Spawn();
        if (!p) continue;

        float angle = ((float)rand() / RAND_MAX) * 6.2831f;
        float speed = 40.f + ((float)rand() / RAND_MAX) * 80.f;

        p->position = pos;
        p->velocity = { cosf(angle) * speed, sinf(angle) * speed };
        p->color = color;
        p->radius = 2.f + ((float)rand() / RAND_MAX) * 3.f;
        p->maxLifetime = 0.3f + ((float)rand() / RAND_MAX) * 0.4f;
    }
}