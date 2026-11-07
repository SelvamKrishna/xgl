#pragma once

#include "xgl/_gl_object.hpp"
#include "xgl/buffer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
    public:
        VertexArray() { this->IAllocate(); }

        void IAllocate();
        static void IDelete(GLuint handle) noexcept { glDeleteVertexArrays(1, &handle); }

        void SetVertexBuffer(const VertexBuffer& vbo, std::span<const VertexAttrib> attribs);
        void SetIndexBuffer(const IndexBuffer& ibo);
        void Bind() const noexcept;
    };

} // namespace xgl
