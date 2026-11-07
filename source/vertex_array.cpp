#include "xgl/vertex_array.hpp"

namespace xgl {

    void VertexArray::IAllocate()
    {
        this->RequireContext();
        GLuint handle { 0 };
        glGenVertexArrays(1, &handle);
        this->Adopt(handle);
    }

    void VertexArray::SetVertexBuffer(const VertexBuffer& vbo, std::span<const VertexAttrib> attribs)
    {
        this->RequireContext();
        glBindVertexArray(this->_handle);

        vbo.Bind();

        for (const auto& ATTRIB : attribs)
        {
            glEnableVertexAttribArray(ATTRIB.location);
            glVertexAttribPointer(
                ATTRIB.location,
                ATTRIB.components,
                ATTRIB.type,
                ATTRIB.normalized,
                ATTRIB.stride,
                reinterpret_cast<const void*>(ATTRIB.offset)
            );
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexArray::SetIndexBuffer(const IndexBuffer& ibo)
    {
        this->RequireContext();
        glBindVertexArray(this->_handle);
            ibo.Bind();
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void VertexArray::Bind() const noexcept { glBindVertexArray(this->_handle); }

} // namespace xgl
