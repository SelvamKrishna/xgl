#include "xgl/window.hpp"
#include "xgl/config.hpp"

#include <zen/log/core.hpp>
#include <zen/log/tools.hpp>

#include <utility>

namespace xgl {

    namespace {

        GLFWwindow* CreateNativeWindow(const Window::Config& config)
        {
            zen::log_process process { "Create Native Window", &config::XGL_TAG };

            GLFWwindow* windowPtr {
                glfwCreateWindow(
                    static_cast<int>(config.width),
                    static_cast<int>(config.height),
                    config.title.c_str(),
                    config.monitor,
                    config.share
                )
            };

            if (!windowPtr) process.log_failure("Failed to create GLFW window");
            return windowPtr;
        }

    }

    Window::Window(const Config& config)
        : _windowPtr    { CreateNativeWindow(config) }
        , _contextPtr   { std::make_shared<ContextState>() }
        , _vsyncEnabled { config.vsync }
    {
        this->_contextPtr->window = this->_windowPtr;
        this->_contextPtr->alive  = true;
    }

    Window::~Window()
    {
        if (this->_contextPtr)
            this->_contextPtr->alive = false;

        if (this->_windowPtr)
            glfwDestroyWindow(this->_windowPtr);

        this->_windowPtr = nullptr;
        this->_contextPtr.reset();
    }

    Window::Window(Window&& other) noexcept
        : _windowPtr  { std::exchange(other._windowPtr, nullptr) }
        , _contextPtr { std::move(other._contextPtr) }
    {}

    Window& Window::operator=(Window&& other) noexcept
    {
        if (this != &other)
        {
            if (this->_contextPtr)
                this->_contextPtr->alive = false;

            if (this->_windowPtr)
                glfwDestroyWindow(this->_windowPtr);

            this->_windowPtr  = std::exchange(other._windowPtr, nullptr);
            this->_contextPtr = std::move(other._contextPtr);
        }
        return *this;
    }

    void Window::MakeCurrent()
    {
        zen::log_process process { "Make Current Context", &config::XGL_TAG };
        process.log_panic_if(this->_windowPtr == nullptr, "Window is not valid");

        glfwMakeContextCurrent(this->_windowPtr);
        this->SetVSync(this->_vsyncEnabled);
    }

    std::pair<int32_t, int32_t> Window::FramebufferSize() const noexcept
    {
        int w = 0, h = 0;
        if (this->_windowPtr)
            glfwGetFramebufferSize(this->_windowPtr, &w, &h);
        return { static_cast<int32_t>(w), static_cast<int32_t>(h) };
    }

} // namespace xgl
