#include "GameManager.h"
#include "GameEvents.h"


void GameManager::Reset() {
    health = 100.f;
    maxHealth = 100.f;
    score = 0;
    enemiesKilled = 0;
    elapsedTime = 0.f;
    level = 1;
    xp = 0.f;
    xpToNextLevel = 50.f;
    upgradePoints = 0;
    dmgMult = 1.0f;
    frMult = 1.0f;
    speedMult = 1.0f;
    orbPickupRadius = 40.f;

    // Start with Cane Laser (0) and Candy Rain (1)
    unlockedWeaponSlots = { 0, 1 };
    confiscatedSlots.clear();
    hpThreshold75Hit = false;
    hpThreshold50Hit = false;
    hpThreshold25Hit = false;
}

void GameManager::AddXP(float amount) {
    xp += amount;
    while (xp >= xpToNextLevel) {
        xp -= xpToNextLevel;
        xpToNextLevel = xpToNextLevel * 1.3f;  // 30% more XP needed each level
        level++;
        upgradePoints++;
        GameEvents::Instance().OnLevelUp.Fire({ level });
    }
}

bool GameManager::CheckConfiscation() {
    float hpPct = health / maxHealth;
    if (!hpThreshold75Hit && hpPct <= 0.75f) { hpThreshold75Hit = true; return true; }
    if (!hpThreshold50Hit && hpPct <= 0.50f) { hpThreshold50Hit = true; return true; }
    if (!hpThreshold25Hit && hpPct <= 0.25f) { hpThreshold25Hit = true; return true; }
    return false;
}

void GameManager::UnlockNextWeapon() {
    // Total weapon slots: 0-6
    // Find the next slot that is not yet unlocked
    for (int i = 0; i < 7; i++) {
        bool alreadyUnlocked = false;
        for (int u : unlockedWeaponSlots)
            if (u == i) { alreadyUnlocked = true; break; }
        if (!alreadyUnlocked) {
            unlockedWeaponSlots.push_back(i);
            return;
        }
    }
}