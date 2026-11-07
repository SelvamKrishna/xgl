#include "xgl/engine.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <zen/log/core.hpp>

#include <atomic>

namespace xgl::internal {

    namespace {
        std::atomic<int> gGLFWRefCount { 0 };
    }

    void AcquireGLFW(config::Version version)
    {
        if (gGLFWRefCount.fetch_add(1) != 0)
            return;

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
        if (gGLFWRefCount.fetch_sub(1) != 1)
            return;

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

        config::XGL_TAG.info()
            << reinterpret_cast<const char*>(glGetString(GL_VERSION))
            << " ("
            << reinterpret_cast<const char*>(glGetString(GL_RENDERER))
            << ")";

        process.log_success();
    }

} // namespace xgl::internal
