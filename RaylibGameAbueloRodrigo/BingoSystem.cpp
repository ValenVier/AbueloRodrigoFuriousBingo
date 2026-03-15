#include "BingoSystem.h"
#include <cstdlib>
#include <algorithm>

void BingoSystem::Init() {
    bingoAchieved_ = false;
    linesCompleted_ = 0;
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
    // Pick exactly one boss number (61-75)
    int bossNumber = 61 + rand() % 15;

    // Fill numbers 1-60 only, shuffle, take 24
    int pool[60];
    for (int i = 0; i < 60; i++) pool[i] = i + 1;
    for (int i = 59; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = pool[i];
        pool[i] = pool[j];
        pool[j] = tmp;
    }

    // Place boss number at a random position (not center — center is FREE)
    int bossPos = rand() % 24;
    if (bossPos >= 12) bossPos++;  // skip center

    for (int i = 0, src = 0; i < 25; i++) {
        if (i == 12) numbers_[i] = 0; //free center
        else if (i == bossPos) numbers_[i] = bossNumber;
        else numbers_[i] = pool[src++];
    }

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

            int newLines = CountLines();
            if (newLines > linesCompleted_) {
                linesCompleted_ = newLines;
                if (OnLineCompleted) OnLineCompleted();  // notify PlayingState
            }

            //bool isBingo = CheckBingo();
            //bool isBingo = (linesCompleted_ >= 5);
            bool isBingo = true;
            for (bool m : marked_)
                if (!m) { isBingo = false; break; }

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

int BingoSystem::CountLines() const {
    int count = 0;
    for (int r = 0; r < 5; r++) {
        bool line = true;
        for (int c = 0; c < 5; c++)
            if (!marked_[r * 5 + c]) { line = false; break; }
        if (line) count++;
    }
    for (int c = 0; c < 5; c++) {
        bool line = true;
        for (int r = 0; r < 5; r++)
            if (!marked_[r * 5 + c]) { line = false; break; }
        if (line) count++;
    }
    if (marked_[0] && marked_[6] && marked_[12] && marked_[18] && marked_[24]) count++;
    if (marked_[4] && marked_[8] && marked_[12] && marked_[16] && marked_[20]) count++;
    return count;
}