#pragma once

class Scene {
    public:
        virtual ~Scene() = default;

        virtual void initialize() = 0;
        virtual void cleanup() = 0;
        virtual void on_enter() = 0;
        virtual void on_exit() = 0;
        virtual void update(float delta_t) = 0;
        virtual void render() = 0;
        virtual void render_ui() = 0;
};
