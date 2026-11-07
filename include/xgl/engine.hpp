#pragma once

#include "xgl/config.hpp"

#include <zen/log/core.hpp>

namespace xgl::internal {

    void AcquireGlfw(config::Version version);
    void ReleaseGlfw();
    void LoadGlad();

} // namespace xgl::internal

namespace xgl {

    struct EngineGuard {
    public:
        explicit EngineGuard(config::Version glVersion = {4, 6});
        ~EngineGuard();

        EngineGuard(const EngineGuard&)            = delete;
        EngineGuard& operator=(const EngineGuard&) = delete;
    };

} // namespace xgl
