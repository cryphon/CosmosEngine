#pragma once

namespace cosmos::input {
class InputListener {
    public:
        virtual void on_mouse_move(double xpos, double ypos) {}
        virtual void on_mouse_button(int button, int action, int mods) {}
        virtual void on_key(int key, int action, int mods) {}
        virtual void on_scroll(double xoffset, double yoffset) {}
        virtual void update(float delta_t) {}

        virtual ~InputListener();
};
}
