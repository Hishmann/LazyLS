#pragma once

#include "pch.h"

#include <queue>

enum class EventType {
    NONE,
    MOUSE_MOVE,
    MOUSE_PRESS,
    MOUSE_RELEASE,
    KEY_PRESS,
    QUIT
};

enum class KeyType {
    CHARACTER,
    UP_ARROW,
    DOWN_ARROW,
    RIGHT_ARROW,
    LEFT_ARROW,
    HOME,
    END
};

enum class KeyModifiers {
    NONE = 0,
    SHIFT = 2,
    ALT = 3,
    SHIFT_ALT = 4,
    CTRL = 5,
    SHIFT_CTRL = 6,
    ALT_CTRL = 7,
    SHIFT_ALT_CTRL = 8
};

struct Event {
    EventType type;

    union {
        struct {
            int x, y, button;
            bool pressed;
        } mouse;

        struct {
            KeyType type;
            char key;
            KeyModifiers modifiers;
        } keyboard;
    };
};


class EventQueue {
private:
    std::deque<Event> queue;
    std::mutex mtx;

public:
    void push(const Event& e) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push_back(e);
    }

    std::vector<Event> pop_all() {
        std::lock_guard<std::mutex> lock(mtx);
        
        // Move contents to a vector and clear the queue
        std::vector<Event> batch(
            std::make_move_iterator(queue.begin()), 
            std::make_move_iterator(queue.end())
        );
        queue.clear();
        
        return batch;
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
};

void input_handling(bool& running, EventQueue& out_event_queue);