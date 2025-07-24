#include "Application.hpp"
#include "Logger.hpp"
#include <iostream>

int Application::run() {
    auto engine = std::make_shared<Engine>();
    Logger::log("Starting engine...", LogLevel::INFO);
    if (!engine->init()) return -1;
    engine->run();
    return 0;
}

