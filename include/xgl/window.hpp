#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <cstdint>

namespace xgl {

    struct Window {
    public:
        struct Config {
            std::string  title     { "Application" };
            uint32_t     width     { 800 };
            uint32_t     height    { 600 };
            bool         resizable { true };
            bool         vsync     { true };
            GLFWmonitor* monitor   { nullptr };
            GLFWwindow*  share     { nullptr };
        };

        struct ContextState {
            GLFWwindow* window { nullptr };
            bool        alive  { true };
        };

        using WeakContext = std::weak_ptr<ContextState>;

    private:
        GLFWwindow*                   _windowPtr { nullptr };
        std::shared_ptr<ContextState> _contextPtr;

    public:
        explicit Window(const Config& config);
        ~Window();

        constexpr Window(const Window&)            noexcept = delete;
        constexpr Window& operator=(const Window&) noexcept = delete;

        Window(Window&&) noexcept;
        Window& operator=(Window&&) noexcept;

        void MakeCurrent();
        [[nodiscard]] WeakContext Context()      const noexcept { return this->_contextPtr; }
        [[nodiscard]] GLFWwindow* NativeHandle() const noexcept { return this->_windowPtr; }

        [[nodiscard]] bool ShouldClose() const noexcept
        {
            return this->_windowPtr ? glfwWindowShouldClose(this->_windowPtr) : true;
        }

        void PollEvents() const noexcept { glfwPollEvents(); }
        void SwapBuffers() const noexcept { if (this->_windowPtr) glfwSwapBuffers(this->_windowPtr); }
        void RequestClose() noexcept
        {
            if (this->_windowPtr) glfwSetWindowShouldClose(this->_windowPtr, GLFW_TRUE);
        }

        void SetVSync(bool enabled) noexcept { glfwSwapInterval(enabled ? 1 : 0); }

        [[nodiscard]] std::pair<int32_t, int32_t> FramebufferSize() const noexcept;

        // void OnFramebufferSize(std::function<void(int,int)>);
        // void OnKey(std::function<void(int key,int scancode,int action,int mods)>);
    };

} // namespace xgl
