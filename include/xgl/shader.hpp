#pragma once

#include "xgl/_gl_object.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace xgl {

    struct ShaderCompileError : public std::runtime_error {
    private:
        std::string _log;

    public:
        ShaderCompileError(GLuint shaderHandle, std::string log);
        [[nodiscard]] const std::string& Log() const noexcept;
    };

    struct ShaderLinkError : public std::runtime_error {
    private:
        std::string _log;

    public:
        ShaderLinkError(GLuint shaderHandle, std::string log);
        [[nodiscard]] const std::string& Log() const noexcept;
    };

    enum struct ShaderType : GLenum {
        Vertex   = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        Compute  = GL_COMPUTE_SHADER,
    };

    struct Shader : public internal::GlObject<Shader> {
    private:
        ShaderType _type;

    public:
        Shader(ShaderType type, std::string_view source);

        [[nodiscard]] ShaderType Type() const noexcept;
        static void Delete(GLuint handle) noexcept;
    };

    struct ShaderProgram : public internal::GlObject<ShaderProgram> {
    private:
        std::unordered_map<std::string, GLint> _uniformCache; // populated lazily

    public:
        ShaderProgram(Shader&& vertexShader, Shader&& fragmentShader);
        ShaderProgram(Shader&& vertexShader, Shader&& fragmentShader, Shader&& geometryShader);

        void Bind() const noexcept;

        void SetUniform(std::string_view name, float) noexcept;
        void SetUniform(std::string_view name, int)   noexcept;

        [[nodiscard]] GLint UniformLocation(std::string_view name) noexcept;

        static void Delete(GLuint handle) noexcept;
    };

} // namespace xgl
