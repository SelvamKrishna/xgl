#include "xgl/render_state.hpp"

namespace xgl {

    ScopedDepthTest::ScopedDepthTest(bool isEnabled)
    {
        (isEnabled) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }

    ScopedBlend::ScopedBlend(bool isEnabled)
    {
        (isEnabled) ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    }

    ScopedBlend::ScopedBlend(GLint source, GLint destination)
    {
        glEnable(GL_BLEND);
        glBlendFunc(source, destination);
    }

    ScopedViewport::ScopedViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }

} // namespace xgl
