#include "xgl/shader.hpp"

#include <zen/log/core.hpp>

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

    Shader::Shader(Kind type, std::filesystem::path sourceFile) : _type { type }
    {
        zen::log_process process {
            {
                "Compiling {} Shader",
                (type == Kind::Vertex)   ? "Vertex" :
                (type == Kind::Fragment) ? "Fragment" :
                (type == Kind::Geometry) ? "Geometry" :
                (type == Kind::Compute)  ? "Compute" : "Unknown"
            }, &config::XGL_TAG
        };

        this->RequireContext();
        this->IAllocate();


        const std::string SOURCE { Shader::_ReadFile(sourceFile) };
        const char* SOURCE_CSTR { SOURCE.c_str() };
        const GLint SOURCE_LEN { static_cast<GLint>(strlen(SOURCE.c_str())) };

        glShaderSource(this->_handle, 1, &SOURCE_CSTR, &SOURCE_LEN);
        glCompileShader(this->_handle);

        GLint ok { GL_FALSE };
        glGetShaderiv(this->_handle, GL_COMPILE_STATUS, &ok);

        ok != GL_TRUE ? process.log_failure(GetShaderInfoLog(this->_handle)) : process.log_success();
    }

    void Shader::IAllocate()
    {
        this->RequireContext();
        this->Adopt(glCreateShader(static_cast<GLenum>(this->_type)));
    }

    void Shader::IDelete(GLuint handle) noexcept { glDeleteShader(handle); }

    ShaderProgram::ShaderProgram(Shader&& vertexShader, Shader&& fragmentShader)
    {
        zen::log_process process { "Linking Shader Program", &config::XGL_TAG };

        this->RequireContext();
        this->IAllocate();

        glAttachShader(this->_handle, vertexShader.NativeHandle());
        glAttachShader(this->_handle, fragmentShader.NativeHandle());
        glLinkProgram(this->_handle);

        GLint ok { GL_FALSE };
        glGetProgramiv(this->_handle, GL_LINK_STATUS, &ok);

        if (ok != GL_TRUE) process.log_failure(GetProgramInfoLog(this->_handle));
        process.log_success();
    }

    ShaderProgram::ShaderProgram(Shader&& vertexShader, Shader&& fragmentShader, Shader&& geometryShader)
    {
        zen::log_process process { "Linking Shader Program", &config::XGL_TAG };

        this->RequireContext();
        this->IAllocate();

        glAttachShader(this->_handle, vertexShader.NativeHandle());
        glAttachShader(this->_handle, fragmentShader.NativeHandle());
        glAttachShader(this->_handle, geometryShader.NativeHandle());
        glLinkProgram(this->_handle);

        GLint ok { GL_FALSE };
        glGetProgramiv(this->_handle, GL_LINK_STATUS, &ok);

        if (ok != GL_TRUE) process.log_failure(GetProgramInfoLog(this->_handle));
        process.log_success();
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
        std::string key { name.data(), name.size() };

        if (auto it = this->_uniformCache.find(key); it != this->_uniformCache.end())
            return it->second;

        const GLint LOC { glGetUniformLocation(this->_handle, key.c_str()) };
        this->_uniformCache.emplace(std::move(key), LOC);

        return LOC;
    }

} // namespace xgl
