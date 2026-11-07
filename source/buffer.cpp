#include "xgl/buffer.hpp"

namespace xgl {

    namespace {
        size_t IndexSizeOf(GLenum type) noexcept
        {
            switch (type)
            {
                case GL_UNSIGNED_BYTE:  return sizeof(uint8_t);
                case GL_UNSIGNED_SHORT: return sizeof(uint16_t);
                case GL_UNSIGNED_INT:   return sizeof(uint32_t);
                default:                return sizeof(uint32_t);
            }
        }
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

    IndexBuffer::IndexBuffer(
        Window::WeakContext ctx,
        const void* data,
        size_t bytes,
        BufferUsage usage,
        GLenum indexType
    ) : IGLObject { std::move(ctx) }, _indexType { indexType }
    {
        this->IAllocate();
        this->RequireContext();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(bytes), data, static_cast<GLenum>(usage));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        this->_count = bytes / IndexSizeOf(indexType);
    }

    void IndexBuffer::IAllocate()
    {
        this->RequireContext();
        GLuint handle { 0 };
        glGenBuffers(1, &handle);
        this->Adopt(handle);
    }

    void IndexBuffer::Bind() const noexcept { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_handle); }
} // namespace xgl
