#pragma once

#include "xgl/_gl_object.hpp"
#include "xgl/buffer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <span>

namespace xgl {

    struct VertexAttrib {
        GLuint    location;
        GLint     components;
        GLenum    type;
        GLboolean normalized;
        GLsizei   stride;
        size_t    offset;
    };

    struct VertexArray : public internal::IGLObject<VertexArray> {
    private:
        std::optional<VertexBuffer> _vbo;
        std::optional<IndexBuffer>  _ibo;

    public:
        explicit VertexArray(Window::WeakContext ctx) : IGLObject { std::move(ctx) } { this->IAllocate(); }

        void IAllocate();
        static void IDelete(GLuint handle) noexcept { glDeleteVertexArrays(1, &handle); }

        void SetVertexBuffer(VertexBuffer vbo, std::span<const VertexAttrib> attribs);
        void SetIndexBuffer(IndexBuffer ibo);
        void Bind() const noexcept;
    };

} // namespace xgl
