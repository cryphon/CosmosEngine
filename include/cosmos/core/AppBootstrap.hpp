#pragma once
#include "AppAPI.hpp"

namespace cosmos::core {


class AppBootstrap final : public IApp {
public:
  void bootstrap(cosmos::core::Engine& engine,
                 const EngineServices&  svc,
                 AppContext&            app) override;
};

}

