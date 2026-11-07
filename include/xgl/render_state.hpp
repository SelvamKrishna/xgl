// render_state.hpp
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace xgl {

    struct ScopedDepthTest {
    private:
        bool _prev;

    public:
        explicit ScopedDepthTest(bool isEnabled);
        ~ScopedDepthTest();

        ScopedDepthTest(const ScopedDepthTest&)            = delete;
        ScopedDepthTest& operator=(const ScopedDepthTest&) = delete;
    };

    struct ScopedBlend {
    private:
        bool   _prevEnabled;
        GLint  _prevSrc;
        GLint  _prevDst;

    public:
        explicit ScopedBlend(bool isEnabled);
        ScopedBlend(GLenum source, GLenum destination);
        ~ScopedBlend();

        ScopedBlend(const ScopedBlend&)            = delete;
        ScopedBlend& operator=(const ScopedBlend&) = delete;
    };

    struct ScopedViewport {
    private:
        GLint _prev[4];

    public:
        ScopedViewport(int x, int y, int width, int height);
        ~ScopedViewport();

        ScopedViewport(const ScopedViewport&)            = delete;
        ScopedViewport& operator=(const ScopedViewport&) = delete;
    };

} // namespace xgl
