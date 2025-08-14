// ==
// Standard Library
// ==

// ==
// Third Party
// ==

// ==
// Cosmos
// ==
#include <cosmos/core/Logger.hpp>
#include <cosmos/core/Application.hpp>
#include <cosmos/core/Engine.hpp>
#include <cosmos/core/AppBootstrap.hpp>

namespace cosmos::core {

int Application::run() {
    Logger::set_level(core::LogLevel::DEBUG);
    cosmos::core::Engine engine;
    AppBootstrap app;
    cosmos::core::EngineConfig cfg { 1600, 900, false, 4, "cosmos" };
    LOG_INFO("Starting engine...");
    if (!engine.init(cfg, app)) return -1;
    engine.run();
    return 0;
}
}

