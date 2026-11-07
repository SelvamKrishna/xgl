#include <xgl/xgl.hpp>

#include <zen/log.hpp>

int main() {
    zen::logger::init();
    xgl::EngineGuard engine { {4, 6} };
    xgl::Window win { {.title = "Hello", .width = 1280, .height = 720} };

    xgl::ShaderProgram program {
        xgl::Shader {xgl::Shader::Kind::Vertex,   "ignore/shader.vert"},
        xgl::Shader {xgl::Shader::Kind::Fragment, "ignore/shader.frag"},
    };

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    xgl::VertexArray vao;
    vao.SetVertexBuffer(
        xgl::VertexBuffer { vertices, sizeof(vertices) },
        std::array { xgl::VertexAttrib{0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0} }
    );

    while (!win.ShouldClose()) {
        win.PollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.Bind();
        vao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        win.SwapBuffers();
    }
}
