#pragma once 
#include <memory>
#include <chrono>

namespace cosmos::core {
    class Engine; 
    class Window;
    class Profiler;
}
namespace cosmos::render{ class Renderer; }
namespace cosmos::scene { class SceneManager; class Camera; class CameraControls; class CameraInputAdapter; }
namespace cosmos::ui    { class UI; }
namespace cosmos::input { class InputManager; }
namespace cosmos::assets{ class SkyBoxManager; }


namespace cosmos::core {

// Visible to both Engine and App Bootstrap
struct EngineServices {
    core::Window& window;
    render::Renderer& renderer;
    core::Profiler& profiler;
    scene::SceneManager& scene_manager;
    ui::UI& ui;
    input::InputManager& input_manager;
};

// Contstructed by AppBootStrap and given to scenes/UI
struct AppContext {
    std::shared_ptr<scene::Camera> camera;
    std::shared_ptr<scene::CameraControls> controls;     // TODO: make this union for controls typing (eg. Orbital, Fly, ...)
    std::shared_ptr<scene::CameraInputAdapter> camera_adapter;
    std::shared_ptr<assets::SkyBoxManager> skybox_manager;
};

struct IApp {
    virtual ~IApp() = default;
    virtual void bootstrap(cosmos::core::Engine& engine, const EngineServices& svc, AppContext& app ) = 0;
};

struct UiContext {
    core::Window& window;
    render::Renderer& renderer;
    core::Profiler& profiler;
    scene::SceneManager& scene_manager;
    std::shared_ptr<scene::Camera> camera;
    std::shared_ptr<scene::CameraControls> controls;
    std::shared_ptr<scene::CameraInputAdapter> camera_adapter;
    std::shared_ptr<assets::SkyBoxManager> skybox_manager;
};


struct FrameStats {
    // Frame
    double frame_ms = 0, cpu_frame_ms = 0, gpu_frame_ms = 0;

    // Update
    double input_ms = 0, update_ms = 0;

    // render (CPU)
    double render_submit_ms = 0;
    int draw_calls = 0, shader_binds = 0, texture_binds = 0, fbo_binds = 0;

    // Render (GPU)
    double pass_main_gpu_ms = 0;
    uint64_t primitives = 0;

    // Scene
    int entities_total = 0, entities_visible = 0, lights = 0;

    // Memory
    size_t vram_textures = 0, vram_buffers = 0, vram_fbos = 0;
    size_t cpu_assets_bytes = 0;

    std::uint64_t frame_index = 0;
};

struct ScopeTimer {
    std::chrono::high_resolution_clock::time_point t0;
    double& out_ms;
    ScopeTimer(double& out) : t0(std::chrono::high_resolution_clock::now()), out_ms(out) {}
    ~ScopeTimer() {
        auto t1 = std::chrono::high_resolution_clock::now();
        out_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    }
};

}
