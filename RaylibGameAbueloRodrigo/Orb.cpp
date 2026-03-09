#include "Orb.h"

Orb* OrbPool::Spawn() {
    for (auto& o : orbs) {
        if (!o.active) {
            o = Orb{};
            o.active = true;
            return &o;
        }
    }
    return nullptr;
}

void OrbPool::DeactivateAll() {
    for (auto& o : orbs) o.active = false;
}