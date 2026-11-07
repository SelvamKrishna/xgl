#pragma once

#include "xgl/window.hpp"
#include "xgl/config.hpp"

#include <zen/log/tools.hpp>

#include <utility>

namespace xgl::internal {

    template <class Derived, class GLHandle = GLuint>
    class IGLObject {
    private:
        void _Destroy() noexcept
        {
            if (this->_handle == 0) return;

            const std::shared_ptr<Window::ContextState> CTX { this->_ctx.lock() };
            if (CTX && CTX->alive)
                Derived::IDelete(this->_handle);

            this->_handle = 0;
        }

    protected:
        using Handle = GLHandle;

        Handle              _handle { 0 };
        Window::WeakContext _ctx    { };

        explicit IGLObject(Window::WeakContext ctx) noexcept : _ctx { std::move(ctx) } {}

        static void IDelete(Handle handle) noexcept; // Implement GL destruction logic
        void IAllocate();                            // Implement GL creation logic

        void RequireContext() const
        {
            const std::shared_ptr<Window::ContextState> CTX { this->_ctx.lock() };

            zen::panic_if(
                !CTX || !CTX->alive,
                {
                    "Attempted to use an OpenGL object `{}` without a valid context.",
                    zen::_get_type<Derived>()
                },
                &config::GLFW_TAG
            );
        }

        void Adopt(Handle newHandle) noexcept
        {
            zen::panic_if(this->_handle != 0, "Adopt() called on a live handle", &config::GLFW_TAG);
            this->_handle = newHandle;
        }

    public:
        IGLObject(IGLObject&& other) noexcept
            : _handle { std::exchange(other._handle, 0) }
            , _ctx    { std::move(other._ctx) }
        {}

        IGLObject& operator=(IGLObject&& other) noexcept
        {
            if (this != &other)
            {
                this->_Destroy();
                this->_handle = std::exchange(other._handle, 0);
                this->_ctx    = std::move(other._ctx);
            }
            return *this;
        }

        IGLObject(const IGLObject&)            = delete;
        IGLObject& operator=(const IGLObject&) = delete;

        ~IGLObject() { this->_Destroy(); }

        [[nodiscard]] constexpr Handle NativeHandle() const noexcept { return this->_handle; }
        [[nodiscard]] constexpr bool   Valid()        const noexcept { return this->_handle != 0; }
    };

} // namespace xgl::internal
