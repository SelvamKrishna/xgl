#pragma once

#include "xgl/_gl_object.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <span>
#include <cstdint>

namespace xgl {

    enum struct BufferUsage : GLenum {
        Static = GL_STATIC_DRAW,
        Dynamic = GL_DYNAMIC_DRAW,
        Stream = GL_STREAM_DRAW
    };

    struct VertexBuffer : public internal::IGLObject<VertexBuffer> {
    public:
        explicit VertexBuffer() = default;
        explicit VertexBuffer(const void* data, size_t bytes, BufferUsage = BufferUsage::Static);

        void IAllocate();
        static void IDelete(Handle handle) noexcept { glDeleteBuffers(1, &handle); }

        void SetData(const void* data, size_t bytes, BufferUsage = BufferUsage::Static);

        void Bind() const noexcept;
    };

    struct IndexBuffer : public internal::IGLObject<IndexBuffer> {
    private:
        size_t _count { 0 };

    public:
        IndexBuffer(const void* data, size_t bytes, BufferUsage = BufferUsage::Static);
        IndexBuffer(std::span<const uint32_t> indices, BufferUsage = BufferUsage::Static);

        void IAllocate();
        static void IDelete(Handle handle) noexcept { glDeleteBuffers(1, &handle); }

        [[nodiscard]] size_t Count() const noexcept { return this->_count; }

        void Bind() const noexcept;
    };

} // namespace xgl
