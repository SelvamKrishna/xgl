#include "config.hpp"
#include "core/window.hpp"

namespace xgl::core {

    Window::Window(
        std::string_view title,
        uint32_t width,
        uint32_t height,
        GLFWmonitor* monitor,
        GLFWwindow* share
    ) noexcept
        : _LOG { std::format("WINDOW:{}", title), zen::ansi_color::MAGENTA, &config::XGL_TAG}
        , _ptrWindow { glfwCreateWindow(width, height, title.data(), monitor, share) }
    {
        zen::log_process process { "Creating Window", &this->_LOG };
        process.log_panic_if(this->_ptrWindow == NULL, "Failed to create GLFWwindow");
        process.log_success();
    }

    Window::~Window() { glfwDestroyWindow(this->_ptrWindow); }

    Window WindowBuilder::Build() const noexcept
    {
        Window window {this->_title, this->_width, this->_height, this->_monitor, this->_share};
        zen::log_process process {"Build Window", &window._LOG};

        process.log_panic_if(this->_title.empty(), "Window title can't be empty");
        process.log_panic_if(this->_width == 0, "Window width can't be 0");
        process.log_panic_if(this->_height == 0, "Window height can't be 0");

        return window;
    }

} // namespace xgl::core
