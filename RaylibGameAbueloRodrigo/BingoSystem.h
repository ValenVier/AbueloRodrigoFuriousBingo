#pragma once
// Manages the 5x5 bingo card.
// PATTERN: Observer (subscribes to OnEnemyKilled, marks numbers automatically)
// PATTERN: Singleton (one global bingo card for the whole game)
#include "GameEvents.h"
#include <array>
#include <functional> 

class BingoSystem {
    public:
        static BingoSystem& Instance() {
            static BingoSystem inst;
            return inst;
        }

        void Init(); // subscribe to events, reset card
        void Shutdown(); // unsubscribe

        bool HasBingo() const { return bingoAchieved_; }
        int MarkedCount() const;

        // For drawing the card
        const std::array<int, 25>& Numbers() const { return numbers_; }
        const std::array<bool, 25>& Marked() const { return marked_; }
        std::function<void()> OnLineCompleted;  // set by PlayingState::Enter()

    private:
        BingoSystem() = default;
        BingoSystem(const BingoSystem&) = delete;
        BingoSystem& operator=(const BingoSystem&) = delete;

        std::array<int, 25> numbers_ = {}; // the 25 numbers on the card
        std::array<bool, 25> marked_ = {}; // which are marked
        bool bingoAchieved_ = false;
        int killListenerId_ = -1;

        void GenerateCard();
        void OnEnemyKilled(const EnemyKilledData& data);
        bool CheckBingo() const; // returns true if any row/col/diagonal complete
        int linesCompleted_ = 0;
        int CountLines() const;
};