#include "xgl/engine.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <zen/log/core.hpp>
#include <zen/log/tools.hpp>

#include <atomic>
#include <stdexcept>

namespace xgl::internal {

    namespace { std::atomic<int> gGLFWRefCount { 0 }; }

    void AcquireGLFW(config::Version version)
    {
        if (gGLFWRefCount.fetch_add(1) != 0) return;

        zen::log_process process { {"Acquiring GLFW {}", version}, &config::XGL_TAG };

        glfwSetErrorCallback(
            [](int code, const char* desc)
            {
                config::GLFW_TAG.err()
                    << zen::log_tag { std::to_string(code), zen::ansi_color::RED }
                    << desc;
            }
        );

        if (!glfwInit())
        {
            gGLFWRefCount.fetch_sub(1);
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        process.log_success();
    }

    void ReleaseGLFW()
    {
        if (gGLFWRefCount.fetch_sub(1) != 1) return;

        zen::log_process process { "Releasing GLFW", &config::XGL_TAG };
        glfwTerminate();
        process.log_success();
    }

    void LoadGlad()
    {
        zen::log_process process { "Loading GLAD", &config::XGL_TAG };

        process.log_panic_if(
            !gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)),
            "Failed to load GLAD"
        );

        const GLubyte* VERSION_STR  { glGetString(GL_VERSION)  };
        const GLubyte* RENDERER_STR { glGetString(GL_RENDERER) };

        config::XGL_TAG.info() << zen::fmt_str {
            "OpenGL {} ({})",
            VERSION_STR  ? reinterpret_cast<const char*>(VERSION_STR)  : "<unknown>",
            RENDERER_STR ? reinterpret_cast<const char*>(RENDERER_STR) : "<unknown>"
        };

        process.log_success();
    }

} // namespace xgl::internal
