#include "xgl/texture.hpp"

namespace xgl {

    Texture2D::Texture2D(
        Window::WeakContext ctx,
        uint32_t width, uint32_t height,
        TextureFormat textureFormat,
        PixelFormat pixelFormat,
        PixelType pixelType,
        const void* data
    )
        : IGLObject    { std::move(ctx) }
        , _pixelFormat { pixelFormat }
        , _pixelType   { pixelType }
    {
        this->RequireContext();
        this->IAllocate();

        glBindTexture(GL_TEXTURE_2D, this->_handle);
            glTexImage2D(
                GL_TEXTURE_2D, 0,
                static_cast<GLint>(textureFormat),
                static_cast<GLsizei>(width),
                static_cast<GLsizei>(height),
                0,
                static_cast<GLenum>(pixelFormat),
                static_cast<GLenum>(pixelType),
                data
            );
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::IAllocate()
    {
        this->RequireContext();
        GLuint handle { 0 };
        glGenTextures(1, &handle);
        this->Adopt(handle);
    }

    void Texture2D::Bind(GLuint unit) const noexcept
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, this->_handle);
    }

    void Texture2D::SetFilter(Filter min, Filter mag)
    {
        this->RequireContext();
        glBindTexture(GL_TEXTURE_2D, this->_handle);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::SetWrap(Wrap s, Wrap t)
    {
        this->RequireContext();
        glBindTexture(GL_TEXTURE_2D, this->_handle);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::GenerateMipmaps()
    {
        this->RequireContext();
        glBindTexture(GL_TEXTURE_2D, this->_handle);
            glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::Update(const void* data, uint32_t width, uint32_t height)
    {
        this->RequireContext();
        glBindTexture(GL_TEXTURE_2D, this->_handle);
            glTexSubImage2D(
                GL_TEXTURE_2D, 0, 0, 0,
                static_cast<GLsizei>(width),
                static_cast<GLsizei>(height),
                static_cast<GLenum>(this->_pixelFormat),
                static_cast<GLenum>(this->_pixelType),
                data
            );
        glBindTexture(GL_TEXTURE_2D, 0);
    }

} // namespace xgl
