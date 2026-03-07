#include "StateManager.h"

void StateManager::ChangeState(std::unique_ptr<GameState> state) {
    pendingOp_ = PendingOp::Change;
    pendingState_ = std::move(state); // transfer ownership
}

void StateManager::PushState(std::unique_ptr<GameState> state) {
    pendingOp_ = PendingOp::Push;
    pendingState_ = std::move(state);
}

void StateManager::PopState() {
    pendingOp_ = PendingOp::Pop;
}

void StateManager::Update(float dt) {
    if (!stack_.empty()) {
        stack_.top()->Update(dt); // only update the top state
    }
    ApplyPending(); // apply any state changes at end of frame
}

void StateManager::Draw() const {
    if (!stack_.empty()) {
        stack_.top()->Draw(); // only draw the top state
    }
}

void StateManager::ApplyPending() {
    if (pendingOp_ == PendingOp::None) return;

    switch (pendingOp_) {
    case PendingOp::Change:
        // Exit and destroy all existing states
        while (!stack_.empty()) {
            stack_.top()->Exit();
            stack_.pop();
        }
        if (pendingState_) {
            pendingState_->Enter();
            stack_.push(std::move(pendingState_));
        }
        break;

    case PendingOp::Push:
        // Current state "sleeps" (Exit), new state goes on top
        if (!stack_.empty()) stack_.top()->Exit();
        if (pendingState_) {
            pendingState_->Enter();
            stack_.push(std::move(pendingState_));
        }
        break;

    case PendingOp::Pop:
        if (!stack_.empty()) {
            stack_.top()->Exit();
            stack_.pop();
        }
        // The state below "wakes up" (Enter again)
        if (!stack_.empty()) stack_.top()->Enter();
        break;

    default: break;
    }

    pendingOp_ = PendingOp::None;
    pendingState_ = nullptr;
}