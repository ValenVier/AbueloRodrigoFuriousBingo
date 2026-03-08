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
};