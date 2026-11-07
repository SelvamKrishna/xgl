#include <xgl/xgl.hpp>

#include <zen/log.hpp>

const char* VS_SRC {
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n"
};

const char* FS_SRC {
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
    "}\n"
};

int main() {
    zen::logger::init();
    xgl::EngineGuard engine { {4, 6} };
    xgl::Window win { {.title = "Hello", .width = 1280, .height = 720} };

    xgl::ShaderProgram program {
        xgl::Shader{xgl::ShaderType::Vertex,   VS_SRC},
        xgl::Shader{xgl::ShaderType::Fragment, FS_SRC},
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
