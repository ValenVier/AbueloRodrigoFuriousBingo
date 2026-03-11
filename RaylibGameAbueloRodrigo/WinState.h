#pragma once
// PATTERN: State;shown when Rodrigo completes a bingo line
#include "GameState.h"

class WinState : public GameState {
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
    int finalLevel_ = 1;
};