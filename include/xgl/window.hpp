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

        constexpr Window(Window&&) noexcept;
        constexpr Window& operator=(Window&&) noexcept;

        void MakeCurrent();
        [[nodiscard]] WeakContext Context()      const noexcept { return this->_contextPtr; }
        [[nodiscard]] GLFWwindow* NativeHandle() const noexcept { return this->_windowPtr; }

        [[nodiscard]] bool ShouldClose() const noexcept;
        void PollEvents() const noexcept;
        void SwapBuffers() const noexcept;
        void RequestClose() noexcept;

        void SetVSync(bool enabled) noexcept;
        [[nodiscard]] std::pair<int32_t, int32_t> FramebufferSize() const noexcept;

        // void OnFramebufferSize(std::function<void(int,int)>);
        // void OnKey(std::function<void(int key,int scancode,int action,int mods)>);
    };

} // namespace xgl
