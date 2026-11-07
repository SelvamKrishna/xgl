#pragma once

#include "xgl/_gl_object.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace xgl {

    enum struct TextureFormat : GLenum { RGBA8 = GL_RGBA8, R8 = GL_R8, /* ... */ };
    enum struct PixelFormat   : GLenum { RGBA = GL_RGBA, RGB = GL_RGB, /* ... */ };
    enum struct PixelType     : GLenum { UByte = GL_UNSIGNED_BYTE, Float = GL_FLOAT };
    enum struct Filter        : GLint  { Nearest = GL_NEAREST, Linear = GL_LINEAR, LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR };
    enum struct Wrap          : GLint  { Repeat = GL_REPEAT, ClampToEdge = GL_CLAMP_TO_EDGE };

    struct Texture2D : public internal::IGLObject<Texture2D> {
    public:
        Texture2D(
            uint32_t width,
            uint32_t height,
            TextureFormat textureFormat,
            PixelFormat pixelFormat,
            PixelType pixelType,
            const void* data = nullptr
        );

        ~Texture2D() = default;

        void IAllocate();
        static void IDelete(GLuint handle) noexcept { glDeleteTextures(1, &handle); }

        void Bind(GLuint unit = 0) const noexcept;
        void SetFilter(Filter min, Filter mag);
        void SetWrap(Wrap s, Wrap t);
        void GenerateMipmaps();
        void Update(const void* data, uint32_t width, uint32_t height);
    };

} // namespace xgl
