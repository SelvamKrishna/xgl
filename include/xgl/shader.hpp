#pragma once

#include "xgl/_gl_object.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>
#include <filesystem>

namespace xgl {

    struct Shader : public internal::IGLObject<Shader> {
    public:
        enum Kind : GLenum {
            Vertex   = GL_VERTEX_SHADER,
            Fragment = GL_FRAGMENT_SHADER,
            Geometry = GL_GEOMETRY_SHADER,
            Compute  = GL_COMPUTE_SHADER,
        };

    private:
        Kind _type;

        static std::string _ReadFile(const std::filesystem::path& path);

    public:
        Shader(Shader::Kind type, std::filesystem::path sourceFile);

        void IAllocate();
        static void IDelete(Handle handle) noexcept;

        [[nodiscard]] constexpr Kind Type() const noexcept { return this->_type; }
    };

    struct ShaderProgram : public internal::IGLObject<ShaderProgram> {
    private:
        std::unordered_map<std::string, GLint> _uniformCache; // populated lazily

    public:
        ShaderProgram(Shader&& vertexShader, Shader&& fragmentShader);
        ShaderProgram(Shader&& vertexShader, Shader&& fragmentShader, Shader&& geometryShader);

        void IAllocate();
        static void IDelete(Handle handle) noexcept;

        void Bind() const noexcept;
        [[nodiscard]] GLint UniformLocation(std::string_view name);

        void SetUniform(std::string_view name, float value) noexcept
        {
            glUniform1f(this->UniformLocation(name), value);
        }

        void SetUniform(std::string_view name, int value) noexcept
        {
            glUniform1i(this->UniformLocation(name), value);
        }
    };

} // namespace xgl
