#include "BingoSystem.h"
#include <cstdlib>
#include <algorithm>

void BingoSystem::Init() {
    bingoAchieved_ = false;
    marked_.fill(false);
    GenerateCard();

    killListenerId_ = GameEvents::Instance().OnEnemyKilled.Subscribe(
        [this](const EnemyKilledData& d) { OnEnemyKilled(d); }
    );
}

void BingoSystem::Shutdown() {
    if (killListenerId_ >= 0) {
        GameEvents::Instance().OnEnemyKilled.Unsubscribe(killListenerId_);
        killListenerId_ = -1;
    }
}

void BingoSystem::GenerateCard() {
    // Fill numbers 1-75, shuffle, take first 25
    int pool[75];
    for (int i = 0; i < 75; i++) pool[i] = i + 1;
    for (int i = 74; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = pool[i];
        pool[i] = pool[j];
        pool[j] = tmp;
    }
    for (int i = 0; i < 25; i++) numbers_[i] = pool[i];

    // Center square is free
    marked_[12] = true;
}

int BingoSystem::MarkedCount() const {
    int count = 0;
    for (bool m : marked_) if (m) count++;
    return count;
}

void BingoSystem::OnEnemyKilled(const EnemyKilledData& data) {
    if (bingoAchieved_) return;

    // Each enemy type marks a different range of numbers
    // NURSE: 1-15, ORDERLY: 16-30, DOCTOR: 31-45, ADMIN: 46-60, CEOBOSS: 61-75
    int minVal = 1, maxVal = 15;
    // pointValue to identify type (10=nurse,25=orderly,20=doctor,35=admin,500=boss)
    if (data.pointValue == 25) { minVal = 16; maxVal = 30; }
    else if (data.pointValue == 20) { minVal = 31; maxVal = 45; }
    else if (data.pointValue == 35) { minVal = 46; maxVal = 60; }
    else if (data.pointValue == 500) { minVal = 61; maxVal = 75; }

    // Find an unmarked number in range and mark it
    for (int i = 0; i < 25; i++) {
        if (!marked_[i] && numbers_[i] >= minVal && numbers_[i] <= maxVal) {
            marked_[i] = true;
            bool isBingo = CheckBingo();
            if (isBingo) bingoAchieved_ = true;
            GameEvents::Instance().OnBingoNumber.Fire({ numbers_[i], isBingo });
            break;
        }
    }
}

bool BingoSystem::CheckBingo() const {
    // Check 5 rows
    for (int r = 0; r < 5; r++) {
        bool line = true;
        for (int c = 0; c < 5; c++)
            if (!marked_[r * 5 + c]) { line = false; break; }
        if (line) return true;
    }
    // Check 5 columns
    for (int c = 0; c < 5; c++) {
        bool line = true;
        for (int r = 0; r < 5; r++)
            if (!marked_[r * 5 + c]) { line = false; break; }
        if (line) return true;
    }
    // Check 2 diagonals
    if (marked_[0] && marked_[6] && marked_[12] && marked_[18] && marked_[24]) return true;
    if (marked_[4] && marked_[8] && marked_[12] && marked_[16] && marked_[20]) return true;
    return false;
}