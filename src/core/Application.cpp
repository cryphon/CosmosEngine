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

namespace cosmos::core {

int Application::run() {
    Logger::set_level(core::LogLevel::DEBUG);
    auto engine = std::make_shared<Engine>();
    LOG_INFO("Starting engine...");
    if (!engine->init()) return -1;
    engine->run();
    return 0;
}
}

