#pragma once
// PATTERN: State (main gameplay state)
#include "GameState.h"
#include "raylib.h"

class PlayingState : public GameState {
public:
    void Enter() override;
    void Exit() override {}
    void Update(float dt) override;
    void Draw() const override;

private:
    // Player position on screen
    Vector2 playerPos_ = { 400.f, 300.f };

    // Last non-zero movement direction 
    Vector2 lastMoveDir_ = { 1.f, 0.f };

    float speed_ = 180.f;
};