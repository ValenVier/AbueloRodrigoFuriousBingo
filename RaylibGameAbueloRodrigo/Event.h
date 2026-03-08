#pragma once
// PATTERN: Observer (typed Event<T> template)
// Systems subscribe with a callback; anyone can fire the event
// Subscribers never need to know who fired it
#include <functional>
#include <vector>

template<typename T>
class Event {
public:
    using Callback = std::function<void(const T&)>;

    // Register a listener (returns an ID for unsubscribing later)
    int Subscribe(Callback cb) {
        int id = nextId_++;
        listeners_.push_back({ id, cb });
        return id;
    }

    // Unsubscribe by ID (call in destructor or Exit() to avoid dangling refs)
    void Unsubscribe(int id) {
        listeners_.erase(
            std::remove_if(listeners_.begin(), listeners_.end(),
                [id](const Entry& e) { return e.id == id; }),
            listeners_.end()
        );
    }

    // Notify all subscribers
    void Fire(const T& data) const {
        for (auto& entry : listeners_)
            entry.cb(data);
    }

private:
    struct Entry { int id; Callback cb; };
    std::vector<Entry> listeners_;
    int nextId_ = 0;
};