#include "Particle.h"

Particle* ParticlePool::Spawn() {
    for (auto& p : particles) {
        if (!p.active) {
            p = Particle{};
            p.active = true;
            return &p;
        }
    }
    return nullptr;
}

void ParticlePool::DeactivateAll() {
    for (auto& p : particles) p.active = false;
}