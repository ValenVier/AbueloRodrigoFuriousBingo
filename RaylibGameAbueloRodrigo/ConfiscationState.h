#pragma once
// PATTERN: State, overlay shown when a weapon is confiscated
// Shows a "CONFISCATED" card for 5 seconds then resumes
#include "GameState.h"
#include <string>

class ConfiscationState : public GameState {
public:
    // confiscatedName: name of the weapon taken away
    // replacementName: name of the weapon now active
    explicit ConfiscationState(const char* confiscatedName, 
        const char* replacementName) : confiscatedName_(confiscatedName), 
        replacementName_(replacementName) {}

    void Enter() override;
    void Exit() override {}
    void Update(float dt) override;
    void Draw() const override;

private:
    const char* confiscatedName_;
    const char* replacementName_;
    float timer_ = 0.f;
};