#include "Application.hpp"
#include "Logger.hpp"

int Application::run() {
    auto engine = std::make_shared<Engine>();
    LOG_INFO("Starting engine...");
    if (!engine->init()) return -1;
    engine->run();
    return 0;
}

