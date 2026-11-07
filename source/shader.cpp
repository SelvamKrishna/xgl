#include "xgl/shader.hpp"

#include <zen/log/core.hpp>

#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

namespace xgl {

    namespace {

        std::string GetShaderInfoLog(GLuint handle)
        {
            GLint messageLen { 0 };
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &messageLen);
            if (messageLen <= 0) return {};

            std::vector<char> buf(static_cast<size_t>(messageLen));
            glGetShaderInfoLog(handle, messageLen, nullptr, buf.data());
            return std::string(buf.data());
        }

        std::string GetProgramInfoLog(GLuint handle)
        {
            GLint len { 0 };
            glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &len);
            if (len <= 0) return {};

            std::vector<char> buf(static_cast<size_t>(len));
            glGetProgramInfoLog(handle, len, nullptr, buf.data());
            return std::string(buf.data());
        }

    }

    std::string Shader::_ReadFile(const std::filesystem::path& path)
    {
        zen::log_process process {
            { "Reading Shader File: {}", path.string() },
            &config::XGL_TAG
        };

        std::ifstream in { path, std::ios::binary };
        if (!in) process.log_failure("Failed to open shader file: " + path.string());

        std::ostringstream ss;
        ss << in.rdbuf();

        process.log_success();
        return ss.str();
    }

    Shader::Shader(Window::WeakContext ctx, Kind type, std::filesystem::path sourceFile)
        : IGLObject { std::move(ctx) }
        , _type     { type }
    {
        zen::log_process process {
            {
                "Compiling {} Shader",
                (type == Kind::Vertex)   ? "Vertex"   :
                (type == Kind::Fragment) ? "Fragment" :
                (type == Kind::Geometry) ? "Geometry" :
                (type == Kind::Compute)  ? "Compute"  : "Unknown"
            }, &config::XGL_TAG
        };

        this->RequireContext();
        this->IAllocate();

        const std::string SOURCE { Shader::_ReadFile(sourceFile) };
        const char* SOURCE_CSTR { SOURCE.c_str() };
        const GLint SOURCE_LEN  { static_cast<GLint>(SOURCE.size()) };

        glShaderSource(this->_handle, 1, &SOURCE_CSTR, &SOURCE_LEN);
        glCompileShader(this->_handle);

        GLint ok { GL_FALSE };
        glGetShaderiv(this->_handle, GL_COMPILE_STATUS, &ok);

        if (ok != GL_TRUE) process.log_failure(GetShaderInfoLog(this->_handle));
        else               process.log_success();
    }

    void Shader::IAllocate()
    {
        this->RequireContext();
        this->Adopt(glCreateShader(static_cast<GLenum>(this->_type)));
    }

    void Shader::IDelete(GLuint handle) noexcept { glDeleteShader(handle); }

    void ShaderProgram::_Link(std::span<Shader* const> shaders)
    {
        zen::log_process process { "Linking Shader Program", &config::XGL_TAG };

        this->RequireContext();
        this->IAllocate();

        for (const Shader* SHADER : shaders)
        {
            if (!SHADER->Valid())
            {
                process.log_failure("Attempted to link an invalid shader");
                continue;
            }

            glAttachShader(this->_handle, SHADER->NativeHandle());
        }

        glLinkProgram(this->_handle);

        for (const Shader* SHADER : shaders)
            glDetachShader(this->_handle, SHADER->NativeHandle());

        GLint ok { GL_FALSE };
        glGetProgramiv(this->_handle, GL_LINK_STATUS, &ok);

        if (ok != GL_TRUE)
        {
            process.log_failure(GetProgramInfoLog(this->_handle));

            GLuint dead { this->_handle };
            glDeleteProgram(dead);
            this->_handle = 0;
        }

        process.log_success();
    }

    ShaderProgram::ShaderProgram(
        Window::WeakContext ctx,
        Shader&& vertexShader,
        Shader&& fragmentShader
    ) : IGLObject { std::move(ctx) }
    {
        Shader* shaders[] { &vertexShader, &fragmentShader };
        this->_Link(shaders);
    }

    ShaderProgram::ShaderProgram(
        Window::WeakContext ctx,
        Shader&& vertexShader,
        Shader&& fragmentShader,
        Shader&& geometryShader
    ) : IGLObject { std::move(ctx) }
    {
        Shader* shaders[] { &vertexShader, &fragmentShader, &geometryShader };
        this->_Link(shaders);
    }

    void ShaderProgram::IAllocate()
    {
        this->RequireContext();
        this->Adopt(glCreateProgram());
    }

    void ShaderProgram::IDelete(GLuint handle) noexcept { glDeleteProgram(handle); }

    void ShaderProgram::Bind() const noexcept { glUseProgram(this->_handle); }

    GLint ShaderProgram::UniformLocation(std::string_view name)
    {
        std::string key { name };

        if (auto it = this->_uniformCache.find(key); it != this->_uniformCache.end())
            return it->second;

        const GLint LOC { glGetUniformLocation(this->_handle, key.c_str()) };
        this->_uniformCache.emplace(std::move(key), LOC);
        return LOC;
    }

} // namespace xgl
