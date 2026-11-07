#pragma once

#include "xgl/window.hpp"

namespace xgl::internal {

    template <class Derived, class Handle = GLuint> class GlObject {
    private:
        void _Destroy() noexcept
        {
            if (this->_handle == 0) return;

            if (const auto CTX {_ctx.lock()}; CTX && CTX->alive)
                Derived::Delete(this->_handle);

            this->_handle = 0;
        }

    protected:
        Handle              _handle { 0 };
        Window::WeakContext _ctx    { };

        static void Delete(Handle handle) noexcept; // Implement GL destruction logic
        void Allocate(); // Implement GL creation logic

        void RequireContext() const;
        void Adopt(Handle newHandle) noexcept { this->_handle = newHandle; }

    public:
        GlObject() = default;

        constexpr GlObject(GlObject&& other) noexcept
            : _handle { std::exchange(other._handle, 0) }
            , _ctx    { std::move(other._ctx) }
        {}

        constexpr GlObject &operator=(GlObject&& other) noexcept
        {
            if (this != &other)
            {
                this->_Destroy();
                this->_handle = std::exchange(other._handle, 0);
                this->_ctx = std::move(other._ctx);
            }

            return *this;
        }

        constexpr GlObject(const GlObject &)            noexcept = delete;
        constexpr GlObject &operator=(const GlObject &) noexcept = delete;

        ~GlObject() { this->_Destroy(); }

        [[nodiscard]] Handle NativeHandle() const noexcept { return this->_handle; }
        [[nodiscard]] bool Valid() const noexcept { return this->_handle != 0; }
    };

} // namespace xgl::internal
