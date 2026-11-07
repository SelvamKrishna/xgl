#pragma once

#include "xgl/window.hpp"
#include "xgl/config.hpp"

#include <zen/log/tools.hpp>

#include <utility>

namespace xgl::internal {

    template <class Derived, class GLHandle = GLuint> class IGLObject {
    private:
        void _Destroy() noexcept
        {
            if (this->_handle == 0) return;
            const std::shared_ptr<Window::ContextState> CTX {this->_ctx.lock()};

            if (CTX && CTX->alive)
                Derived::IDelete(this->_handle);

            this->_handle = { 0 };
        }

    protected:
        using Handle = GLHandle;

        Handle              _handle { 0 };
        Window::WeakContext _ctx    { };

        static void IDelete(Handle handle) noexcept; // Implement GL destruction logic
        void IAllocate();                            // Implement GL creation logic

        void RequireContext() const
        {
            const std::shared_ptr<Window::ContextState> CTX { this->_ctx.lock() };

            zen::panic_if(
                !CTX || !CTX->alive,
                {
                    "Attempted to use an OpenGL object `{}` without a valid context.",
                    typeid(Derived).name()
                },
                &config::GLFW_TAG
            );
        }

        void Adopt(Handle newHandle) noexcept { this->_handle = newHandle; }

    public:
        IGLObject() = default;

        constexpr IGLObject(IGLObject&& other) noexcept
            : _handle { std::exchange(other._handle, 0) }
            , _ctx    { std::move(other._ctx) }
        {}

        constexpr IGLObject &operator=(IGLObject&& other) noexcept
        {
            if (this != &other)
            {
                this->_Destroy();
                this->_handle = std::exchange(other._handle, 0);
                this->_ctx = std::move(other._ctx);
            }

            return *this;
        }

        constexpr IGLObject(const IGLObject &)            noexcept = delete;
        constexpr IGLObject &operator=(const IGLObject &) noexcept = delete;

        ~IGLObject() { this->_Destroy(); }

        [[nodiscard]] Handle NativeHandle() const noexcept { return this->_handle; }
        [[nodiscard]] bool Valid() const noexcept { return this->_handle != 0; }
    };

} // namespace xgl::internal
