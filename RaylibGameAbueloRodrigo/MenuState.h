#pragma once
// PATTERN: State (main menu screen)
#include "GameState.h"

class MenuState : public GameState {
    public:
        void Enter() override;
        void Exit() override {}
        void Update(float dt) override;
        void Draw() const override;

    private:
        float timer_ = 0.f; // used for animations
        int selectedDiff_ = 1;  // 0=Easy, 1=Medium, 2=Hard
        int selectedMode_ = 0;  // 0=Windowed, 1=Fullscreen
};