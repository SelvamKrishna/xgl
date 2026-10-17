#include "core/engine.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace xgl::core {

    const zen::log_tag Engine::_LOG {"ENGINE", zen::ansi_color::MAGENTA, &config::XGL_TAG};

    Engine::~Engine() { Engine::Quit(); }

    Engine& Engine::GetInstance() noexcept
    {
        static Engine instance {};
        return instance;
    }

    void Engine::Init(config::Version openGLVersion) noexcept
    {
        Engine& inst {XGL_INST};
        zen::log_process process { {"Engine Initialization OPENGL {}", openGLVersion}, &Engine::_LOG };

        process.log_panic_if(inst._state != Engine::State::Awake, "Engine Already Initialized");
        process.log_panic_if(!glfwInit(), "GLFW Initialization Failed");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLVersion.major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLVersion.minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        inst._state = Engine::State::Initialized;
        process.log_success();
    }

    void Engine::Quit() noexcept
    {
        Engine& inst {XGL_INST};

        switch (inst._state)
        {
            case State::Awake:       break;
            case State::Initialized: glfwTerminate();
            case State::Running:     glfwTerminate();
            case State::Shutdown:    break;
        };

        inst._state = Engine::State::Shutdown;
    }

} // namespace xgl::core
