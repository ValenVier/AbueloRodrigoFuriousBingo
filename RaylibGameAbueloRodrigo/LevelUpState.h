#pragma once
// PATTERN: State; overlay shown when Rodrigo levels up
// Player picks one of three upgrade cards
// PlayingState stays on the stack underneath
#include "GameState.h"
#include "UpgradeCard.h"
#include <array>

class LevelUpState : public GameState {
public:
    void Enter() override;
    void Exit() override {}
    void Update(float dt) override;
    void Draw() const override;

private:
    std::array<UpgradeCard, 3> cards_;
    int hoveredCard_ = -1;
    float timer_ = 0.f;

    void BuildCards();
    void ApplyCard(int idx);
};