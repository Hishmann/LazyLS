#pragma once

#include "pch.h"

#include <queue>

enum class EventType {
    MOUSE_MOVE,
    MOUSE_PRESS,
    MOUSE_RELEASE,
    KEY_PRESS,
    QUIT
};


struct Event {
    EventType type;

    union {
        struct {
            int x, y, button;
            bool pressed;
        } mouse;

        struct {
            char key;
            int modifiers;
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


struct PixelCoordinates {
    int x;
    int y;
};


class BaseRenderElement {  
    protected:
        std::string rep = "";
        virtual void make() = 0;
    public:
        PixelCoordinates coord;
        virtual ~BaseRenderElement() = default; // Compiler will generate the destructor
        virtual std::string representation() = 0;
        virtual void update(const Event&) = 0;
};


template <typename Derived>
class RenderElement : public BaseRenderElement {
    protected:
        std::function<bool(Derived&, const Event&)> update_func;

        RenderElement(
            PixelCoordinates coordinates, std::function<bool(Derived&, const Event&)> func
        ) : 
            update_func(std::move(func)) {
                this -> coord = coordinates;
            }
        
    public:
        std::string representation() { 
            if (rep.empty()) {
                make();
            }
            return rep; 
        }
        
        void update(const Event& e) override {
            if (update_func && update_func(*static_cast<Derived*>(this), e)) {
                make();
            }
        };
}; 


class BoxRenderElement : public RenderElement<BoxRenderElement> {
    protected:
        void make() override;
    public:
        int w, h;
        std::string colour;

        BoxRenderElement(
            int width, int height, PixelCoordinates coordinates, const std::string& box_colour, std::function<bool(BoxRenderElement&, const Event&)> func
        ) : 
            RenderElement<BoxRenderElement>(coordinates, std::move(func)), 
            w {width}, h {height}, colour {box_colour} {}
};