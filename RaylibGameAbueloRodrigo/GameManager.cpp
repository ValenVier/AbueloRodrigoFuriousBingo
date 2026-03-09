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