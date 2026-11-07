#include <xgl/xgl.hpp>

#include <zen/log.hpp>

#include <array>

int main()
{
    zen::logger::init();

    xgl::EngineGuard engine { {4, 6} };
    xgl::Window win { {.title = "Hello", } };

    win.MakeCurrent();
    xgl::internal::LoadGlad();

    xgl::Window::WeakContext ctx { win.Context() };

    xgl::ShaderProgram program {
        ctx,
        xgl::Shader { ctx, xgl::Shader::Kind::Vertex,   "ignore/shader.vert" },
        xgl::Shader { ctx, xgl::Shader::Kind::Fragment, "ignore/shader.frag" },
    };

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    xgl::VertexArray vao { ctx };
    vao.SetVertexBuffer(
        xgl::VertexBuffer { ctx, vertices, sizeof(vertices) },
        std::array { xgl::VertexAttrib { 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0 } }
    );

    while (!win.ShouldClose())
    {
        win.PollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.Bind();
        vao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        win.SwapBuffers();
    }
}
