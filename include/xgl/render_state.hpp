#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace xgl {

    struct ScopedDepthTest {
    public:
        explicit ScopedDepthTest(bool enabled);

        ~ScopedDepthTest();
    };

    struct ScopedBlend {
    public:
        explicit ScopedBlend(bool enabled);
        ScopedBlend(GLint src, GLint dst);

        ~ScopedBlend();
    };

    struct ScopedViewport {
    public:
        ScopedViewport(int x, int y, int w, int h);

        ~ScopedViewport();
    };

} // namespace xgl
