#pragma once

#include <zen/log/core.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string_view>

namespace xgl::core {

    struct Window final {
        friend struct WindowBuilder;

    private:
        const zen::log_tag _LOG;
        GLFWwindow* _ptrWindow {nullptr};

        Window(
            std::string_view title,
            uint32_t width,
            uint32_t height,
            GLFWmonitor* monitor,
            GLFWwindow* share
        ) noexcept;

    public:
        ~Window();

        [[nodiscard]] constexpr GLFWwindow* GetWindowPtr() const noexcept { return this->_ptrWindow; }
    };

    struct WindowBuilder final {
    private:
        std::string_view _title {"Application"};
        uint32_t _width  {800};
        uint32_t _height {600};

        GLFWmonitor* _monitor {nullptr};
        GLFWwindow*  _share   {nullptr};

    public:
        WindowBuilder() = default;

        [[nodiscard]] WindowBuilder& WithDimension(uint32_t width, uint32_t height) noexcept
        {
            this->_width  = width;
            this->_height = height;
            return *this;
        }

        [[nodiscard]] WindowBuilder& WithTitle(std::string_view title)  noexcept { this->_title = title;      return *this; }
        [[nodiscard]] WindowBuilder& WithMonitor(GLFWmonitor* monitior) noexcept { this->_monitor = monitior; return *this; }
        [[nodiscard]] WindowBuilder& WithShareWindow(GLFWwindow* share) noexcept { this->_share = share;      return *this; }

        [[nodiscard]] Window Build() const noexcept;
    };

} // namespace xgl::core
