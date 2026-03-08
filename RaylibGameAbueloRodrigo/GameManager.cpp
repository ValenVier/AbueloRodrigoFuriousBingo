#include "GameManager.h"

void GameManager::Reset() {
    health = 100.f;
    maxHealth = 100.f;
    score = 0;
    enemiesKilled = 0;
    elapsedTime = 0.f;
}