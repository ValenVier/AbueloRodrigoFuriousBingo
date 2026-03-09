#pragma once
// PATTERN: Object Pool (particle is a plain data struct)
// Spawned on enemy death and orb collection for visual feedback
#include "raylib.h"
#include <array>

struct Particle {
    Vector2 position = { 0.f, 0.f };
    Vector2 velocity = { 0.f, 0.f };
    float lifetime = 0.f;
    float maxLifetime = 0.6f;
    float radius = 4.f;
    Color color = WHITE;
    bool active = false;
};

constexpr int MAX_PARTICLES = 600;

class ParticlePool {
public:
    Particle* Spawn();
    void DeactivateAll();
    std::array<Particle, MAX_PARTICLES> particles;
};