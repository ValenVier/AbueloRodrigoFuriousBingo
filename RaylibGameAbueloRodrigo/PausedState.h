#pragma once
// PATTERN: State (pause overlay pushed on top of PlayingState)
// PlayingState is NOT destroyed (it resumes exactly where it left off)
#include "GameState.h"

class PausedState : public GameState {
public:
    void Enter() override {}
    void Exit() override {}
    void Update(float dt) override;
    void Draw() const override;
};