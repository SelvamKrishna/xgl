#include "xgl/buffer.hpp"

namespace xgl {

    VertexBuffer::VertexBuffer(const void* data, size_t bytes, BufferUsage usage)
    {
        this->IAllocate();
        this->SetData(data, bytes, usage);
    }

    void VertexBuffer::IAllocate()
    {
        this->RequireContext();
        GLuint handle { 0 };
        glGenBuffers(1, &handle);
        this->Adopt(handle);
    }

    void VertexBuffer::SetData(const void* data, size_t bytes, BufferUsage usage)
    {
        this->RequireContext();
        glBindBuffer(GL_ARRAY_BUFFER, this->_handle);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(bytes), data, static_cast<GLenum>(usage));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::Bind() const noexcept { glBindBuffer(GL_ARRAY_BUFFER, this->_handle); }

    IndexBuffer::IndexBuffer(const void* data, size_t bytes, BufferUsage usage)
    {
        this->IAllocate();
        this->RequireContext();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(bytes), data, static_cast<GLenum>(usage));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        this->_count = bytes / sizeof(uint32_t);
    }

    IndexBuffer::IndexBuffer(std::span<const uint32_t> indices, BufferUsage usage)
        : IndexBuffer(indices.data(), indices.size_bytes(), usage)
    {}

    void IndexBuffer::IAllocate()
    {
        this->RequireContext();
        GLuint handle { 0 };
        glGenBuffers(1, &handle);
        this->Adopt(handle);
    }

    void IndexBuffer::Bind() const noexcept
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_handle);
    }

} // namespace xgl
