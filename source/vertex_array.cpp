#include "xgl/vertex_array.hpp"

namespace xgl {

    void VertexArray::IAllocate()
    {
        this->RequireContext();
        GLuint handle { 0 };
        glGenVertexArrays(1, &handle);
        this->Adopt(handle);
    }

    void VertexArray::SetVertexBuffer(VertexBuffer vbo, std::span<const VertexAttrib> attribs)
    {
        this->RequireContext();
        this->_vbo = std::move(vbo);

        glBindVertexArray(this->_handle);
            this->_vbo->Bind();

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

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void VertexArray::SetIndexBuffer(IndexBuffer ibo)
    {
        this->RequireContext();
        this->_ibo = std::move(ibo);

        glBindVertexArray(this->_handle);
            this->_ibo->Bind();
        glBindVertexArray(0);
    }

    void VertexArray::Bind() const noexcept { glBindVertexArray(this->_handle); }

} // namespace xgl
