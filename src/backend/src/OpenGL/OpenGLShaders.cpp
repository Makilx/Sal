#include <SalBackend/Rendering/OpenGL/OpenGLShaders.hpp>
#include <glad/glad.h>

namespace sal::backend {
    bool OpenGLShader::Create(ShaderType type, const String &source) {
        if (_created) {
            Destroy();
        }

        // create shader
        _shader = glCreateShader(type == ShaderType::VertexShader
                                     ? GL_VERTEX_SHADER
                                     : GL_FRAGMENT_SHADER);

        _created = true;
        _type = type;

        // set shader source
        const char *sourcePtr = source.c_str();
        glShaderSource(_shader, 1, &sourcePtr, NULL);

        // compile shader
        glCompileShader(_shader);

        int success;
        glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            int logLength = 0;
            glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &logLength);

            if (logLength > 0) {
                std::vector<char> infoLog(logLength);
                glGetShaderInfoLog(_shader, logLength, NULL, &infoLog[0]);

                // Using your custom Errorf console logger
                Console::Error("Failed to compile", ToString(type), "shader:\n",
                               &infoLog[0]);
            }

            Destroy();
            return false;
        }

        return true;
    }

    void OpenGLShader::Destroy() {
        if (!_created) {
            return;
        }

        glDeleteShader(_shader);

        _created = false;
    }

    //

    bool OpenGLShaderProgram::Create() {
        if (_created) {
            Destroy();
        }

        _program = glCreateProgram();

        _created = true;
        _uniformCache.clear();

        return true;
    }

    void OpenGLShaderProgram::Destroy() {
        if (!_created) {
            return;
        }

        glDeleteProgram(_program);

        _created = false;
        _uniformCache.clear();
    }

    void OpenGLShaderProgram::Attach(const interface::IShader &shader) {
        if (!_created) {
            return;
        }

        if (shader.GetRendererAPI() != GetRendererAPI()) {
            return;
        }

        const OpenGLShader &glShader =
            static_cast<const OpenGLShader &>(shader);

        glAttachShader(_program, glShader.GetHandle());
    }

    void OpenGLShaderProgram::Link() {
        if (!_created) {
            return;
        }

        glLinkProgram(_program);
    }

    void OpenGLShaderProgram::Set(const String &name, int value) {
        if (!_created) {
            return;
        }

        int location = GetUniformLocation(name);
        if (location == -1) {
            return;
        }

        glProgramUniform1i(_program, location, value);
    }

    void OpenGLShaderProgram::Set(const String &name, float value) {
        if (!_created) {
            return;
        }

        int location = GetUniformLocation(name);
        if (location == -1) {
            return;
        }

        glProgramUniform1f(_program, location, value);
    }

    void OpenGLShaderProgram::Set(const String &name, const Vector2 &value) {
        if (!_created) {
            return;
        }

        int location = GetUniformLocation(name);
        if (location == -1) {
            return;
        }

        glProgramUniform2f(_program, location, value.X, value.Y);
    }

    void OpenGLShaderProgram::Set(const String &name, const Vector3 &value) {
        if (!_created) {
            return;
        }

        int location = GetUniformLocation(name);
        if (location == -1) {
            return;
        }

        glProgramUniform3f(_program, location, value.X, value.Y, value.Z);
    }

    void OpenGLShaderProgram::Set(const String &name, const Vector4 &value) {
        if (!_created) {
            return;
        }

        int location = GetUniformLocation(name);
        if (location == -1) {
            return;
        }

        glProgramUniform4f(_program, location, value.X, value.Y, value.Z,
                           value.W);
    }

    void OpenGLShaderProgram::Set(const String &name, const Color &value) {
        if (!_created) {
            return;
        }

        int location = GetUniformLocation(name);
        if (location == -1) {
            return;
        }

        glProgramUniform4f(_program, location, value.R, value.G, value.B,
                           value.A);
    }

    void OpenGLShaderProgram::Set(const String &name, const CFrame &value) {
        Set(name, value.ToMatrix4());
    }

    void OpenGLShaderProgram::Set(const String &name, const Matrix4 &value) {
        if (!_created) {
            return;
        }

        int location = GetUniformLocation(name);
        if (location == -1) {
            return;
        }

        glProgramUniformMatrix4fv(_program, location, 1, GL_FALSE,
                                  value.Data());
    }

    int OpenGLShaderProgram::GetUniformLocation(const String &uniform) {
        auto it = _uniformCache.find(uniform);
        if (it != _uniformCache.end()) {
            return it->second;
        }

        int location = glGetUniformLocation(_program, uniform.c_str());

        if (location == -1) {
            return -1;
        }

        _uniformCache.insert({uniform, location});
        return location;
    }
}
