#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace xgl {

    struct ScopedDepthTest {
    public:
        explicit ScopedDepthTest(bool isEnabled);
    };

    struct ScopedBlend {
    public:
        explicit ScopedBlend(bool isEnabled);
        ScopedBlend(GLint source, GLint destination);
    };

    struct ScopedViewport {
    public:
        ScopedViewport(int x, int y, int width, int height);
    };

} // namespace xgl
