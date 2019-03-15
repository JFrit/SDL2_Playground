#ifndef SDL2_PLAYGROUND_SHADER_H
#define SDL2_PLAYGROUND_SHADER_H

#include <bits/shared_ptr.h>
#include <string>

namespace sp {
    class OGLShader;

    class Shader {
    public:

#if defined(_OPENGL)
        typedef OGLShader shaderType;
#endif

        static std::shared_ptr<Shader> UsingFiles(const std::string& vertexPath, const std::string& fragmentPath);

        Shader() = default;
        virtual ~Shader() = default;

        Shader(Shader& other) = delete;
        Shader& operator=(Shader& other) = delete;

        virtual void Use() = 0;

        virtual void SetBool(const std::string& name, bool value) = 0;
        virtual void SetInt(const std::string& name, int value) = 0;
        virtual void SetFloat(const std::string& name, int value) = 0;
    };
}

#endif //SDL2_PLAYGROUND_SHADER_H
