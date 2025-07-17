#pragma once

class Scene {
    public:
        virtual ~Scene() = default;

        virtual void initialize() = 0;
        virtual void update(float delta_t) = 0;
        virtual void render() = 0;
        virtual void render_ui() = 0;
        virtual void cleanup() = 0;
};
