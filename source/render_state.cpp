// render_state.cpp
#include "xgl/render_state.hpp"

namespace xgl {

    ScopedDepthTest::ScopedDepthTest(bool isEnabled) : _prev { glIsEnabled(GL_DEPTH_TEST) == GL_TRUE }
    {
        isEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }

    ScopedDepthTest::~ScopedDepthTest() { this->_prev ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST); }

    ScopedBlend::ScopedBlend(bool isEnabled)
        : _prevEnabled { glIsEnabled(GL_BLEND) == GL_TRUE }
        , _prevSrc { GL_ONE }
        , _prevDst { GL_ZERO }
    {
        glGetIntegerv(GL_BLEND_SRC_RGB, &_prevSrc);
        glGetIntegerv(GL_BLEND_DST_RGB, &_prevDst);
        isEnabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    }

    ScopedBlend::ScopedBlend(GLenum source, GLenum destination)
        : _prevEnabled { glIsEnabled(GL_BLEND) == GL_TRUE }
        , _prevSrc { GL_ONE  }
        , _prevDst { GL_ZERO }
    {
        glGetIntegerv(GL_BLEND_SRC_RGB, &_prevSrc);
        glGetIntegerv(GL_BLEND_DST_RGB, &_prevDst);
        glEnable(GL_BLEND);
        glBlendFunc(source, destination);
    }

    ScopedBlend::~ScopedBlend()
    {
        glBlendFunc(static_cast<GLenum>(_prevSrc), static_cast<GLenum>(_prevDst));
        this->_prevEnabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    }

    ScopedViewport::ScopedViewport(int x, int y, int width, int height)
    {
        glGetIntegerv(GL_VIEWPORT, _prev);
        glViewport(x, y, width, height);
    }

    ScopedViewport::~ScopedViewport() { glViewport(_prev[0], _prev[1], _prev[2], _prev[3]); }

} // namespace xgl
