#ifndef SDL2_PLAYGROUND_SHADERPROGRAM_H
#define SDL2_PLAYGROUND_SHADERPROGRAM_H

#include <glad/glad.h>
#include <memory>
#include <string>

namespace sp {
    class OGLShaderProgram;

    class ShaderProgram {
    public:

#if defined(_OPENGL)
        typedef OGLShaderProgram shaderType;
#endif

        static std::shared_ptr<ShaderProgram> UsingFiles(const std::string& vertexPath, const std::string& fragmentPath);

        ShaderProgram() = default;
        virtual ~ShaderProgram() = default;

        ShaderProgram(ShaderProgram& other) = delete;
        ShaderProgram& operator=(ShaderProgram& other) = delete;

        virtual void Use() = 0;

        virtual void SetBool(const std::string& name, bool value) = 0;
        virtual void SetInt(const std::string& name, int value) = 0;
        virtual void SetFloat(const std::string& name, float value) = 0;

    protected:
        std::string vertexPath = std::string();
        std::string fragmentPath = std::string();
    };

#if defined(_OPENGL)

    class OGLShaderProgram : public ShaderProgram {
        friend class ShaderProgram;

    public:
        OGLShaderProgram() = default;
        ~OGLShaderProgram() override;

        void Use() override;

        void Create();

        OGLShaderProgram(OGLShaderProgram& other) = delete;
        OGLShaderProgram& operator=(OGLShaderProgram& other) = delete;

        [[nodiscard]] GLint GetUniformLocation(const std::string& name) const;
        [[nodiscard]] static GLint GetAttributeLocation(const std::string& name) ;

        void SetBool(const std::string& name, bool value) override;
        void SetInt(const std::string& name, int value) override;
        void SetFloat(const std::string& name, float value) override;

    private:
        static std::shared_ptr<OGLShaderProgram> UsingFiles(const std::string& vertexPath, const std::string& fragmentPath);
        static void printOnError(GLuint shaderId);

        GLuint program = 0;
    };
#endif

}

#endif //SDL2_PLAYGROUND_SHADERPROGRAM_H
