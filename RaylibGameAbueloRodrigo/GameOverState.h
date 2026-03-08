#pragma once
// PATTERN: State (shown when Rodrigo's HP reaches zero)
#include "GameState.h"

class GameOverState : public GameState {
    public:
        void Enter() override;
        void Exit() override {}
        void Update(float dt) override;
        void Draw() const override;

    private:
        float timer_ = 0.f;
        int finalScore_ = 0;
        int finalKills_ = 0;
        float finalTime_ = 0.f;
};