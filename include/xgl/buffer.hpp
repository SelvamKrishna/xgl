#pragma once

#include "xgl/_gl_object.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <span>
#include <cstdint>

namespace xgl {

    enum struct BufferUsage : GLenum {
        Static  = GL_STATIC_DRAW,
        Dynamic = GL_DYNAMIC_DRAW,
        Stream  = GL_STREAM_DRAW
    };

    struct VertexBuffer : public internal::IGLObject<VertexBuffer> {
    public:
        explicit VertexBuffer(Window::WeakContext ctx) : IGLObject { std::move(ctx) } { this->IAllocate(); }

        VertexBuffer(
            Window::WeakContext ctx,
            const void* data,
            size_t bytes,
            BufferUsage usage = BufferUsage::Static
        ) : VertexBuffer { std::move(ctx) } { this->SetData(data, bytes, usage); }

        void IAllocate();
        static void IDelete(Handle handle) noexcept { glDeleteBuffers(1, &handle); }

        void SetData(const void* data, size_t bytes, BufferUsage = BufferUsage::Static);
        void Bind() const noexcept;
    };

    struct IndexBuffer : public internal::IGLObject<IndexBuffer> {
    private:
        size_t _count     { 0 };
        GLenum _indexType { GL_UNSIGNED_INT };

    public:
        IndexBuffer(
            Window::WeakContext ctx,
            const void* data,
            size_t bytes,
            BufferUsage = BufferUsage::Static,
            GLenum indexType = GL_UNSIGNED_INT
        );

        IndexBuffer(
            Window::WeakContext ctx,
            std::span<const uint32_t> indices,
            BufferUsage usage = BufferUsage::Static
        ) : IndexBuffer { std::move(ctx), indices.data(), indices.size_bytes(), usage, GL_UNSIGNED_INT } {}

        void IAllocate();
        static void IDelete(Handle handle) noexcept { glDeleteBuffers(1, &handle); }

        [[nodiscard]] constexpr size_t Count()     const noexcept { return this->_count; }
        [[nodiscard]] constexpr GLenum IndexType() const noexcept { return this->_indexType; }

        void Bind() const noexcept;
    };

} // namespace xgl
