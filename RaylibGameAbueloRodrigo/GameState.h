#pragma once
// PATTERN: State (abstract interface that every game state must implement)
// Each state (menu, playing, pause...) is a subclass

class GameState {
public:
    virtual ~GameState() = default;

    // Called once when entering this state
    virtual void Enter() {}

    // Called once when leaving this state
    virtual void Exit() {}

    // Per frame logic; dt = time since last frame in seconds
    virtual void Update(float dt) = 0;

    // Per frame rendering; const because drawing does not modify game logic
    virtual void Draw() const = 0;
};