#pragma once

#include "config.hpp"

#include <zen/log/core.hpp>

#include <cstdint>

namespace xgl::core {

    struct Engine final {
    private:
        enum class State : uint8_t { Awake, Initialized, Running, Shutdown };

        static const zen::log_tag _LOG;
        State _state { Engine::State::Awake };

        Engine() = default;
        ~Engine();

    public:
        Engine(Engine&&)                 = delete;
        Engine(const Engine&)            = delete;
        Engine& operator=(Engine&&)      = delete;
        Engine& operator=(const Engine&) = delete;

        [[nodiscard]] static Engine& GetInstance() noexcept;

        static void Init(config::Version openGLVersion = {3, 3}) noexcept;
        static void Quit() noexcept;
    };

} // namespace xgl::core

#define XGL_INST xgl::core::Engine::GetInstance()
