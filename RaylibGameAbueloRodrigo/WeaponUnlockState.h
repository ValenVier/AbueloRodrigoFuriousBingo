#pragma once
// PATTERN: State, shown when a bingo line is completed
// Player chooses between 2 weapon options
#include "GameState.h"
#include <array>
#include "raylib.h"

class WeaponUnlockState : public GameState {
    public:
        // option1Idx, option2Idx: indices into the full weapon list
        WeaponUnlockState(int option1Idx, int option2Idx)
            : option1Idx_(option1Idx), option2Idx_(option2Idx) {
        }

        void Enter() override;
        void Exit() override {}
        void Update(float dt) override;
        void Draw() const override;

    private:
        int option1Idx_;
        int option2Idx_;
        int hovered_ = -1;
        float timer_ = 0.f;

        const char* WeaponName(int idx) const;
        const char* WeaponDesc(int idx) const;
        Color WeaponColor(int idx) const;
        void PickWeapon(int idx);
};