#pragma once
// PATTERN: State (manages the currently active game state)
// PATTERN: Singleton (one global instance accessible from anywhere)
#include "GameState.h"
#include <memory>
#include <stack>

class StateManager {
    public:
        // PATTERN: Singleton — global access point
        static StateManager& Instance() {
            static StateManager inst; // created once, reused every call
            return inst;
        }

        // Replace the entire stack with a new state (example: go to menu)
        void ChangeState(std::unique_ptr<GameState> state);

        // Push a state on top without destroying the previous one (example: pause over game)
        void PushState(std::unique_ptr<GameState> state);

        // Remove the top state and return to the previous one
        void PopState();

        void Update(float dt);
        void Draw() const;

        bool IsEmpty() const { return stack_.empty(); }

    private:
        // Private constructor: nobody can create instances from outside
        StateManager() = default;
        StateManager(const StateManager&) = delete;
        StateManager& operator=(const StateManager&) = delete;

        std::stack<std::unique_ptr<GameState>> stack_;

        // Pending operations are applied at the end of the frame to avoid
        // modifying the stack while iterating over it
        enum class PendingOp { None, Change, Push, Pop };
        PendingOp pendingOp_ = PendingOp::None;
        std::unique_ptr<GameState> pendingState_;

        void ApplyPending();
};