#include "sp/ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::shared_ptr<sp::ShaderProgram> sp::ShaderProgram::UsingFiles(const std::string &vertexPath, const std::string &fragmentPath) {

#if defined(_OPENGL)
    return std::static_pointer_cast<ShaderProgram>(sp::OGLShaderProgram::UsingFiles(vertexPath, fragmentPath));
#endif

}

std::shared_ptr<sp::OGLShaderProgram> sp::OGLShaderProgram::UsingFiles(const std::string &vertexPath, const std::string &fragmentPath) {
    auto shader = std::make_shared<OGLShaderProgram>();
    shader->vertexPath = vertexPath;
    shader->fragmentPath = fragmentPath;
    shader->Create();
    return shader;
}

sp::OGLShaderProgram::~OGLShaderProgram() {
    if(program) {
        glDeleteProgram(program);
        program = 0;
    }
}

void sp::OGLShaderProgram::Create() {
    // Get shader sources from file paths
    std::string vertCode;
    std::string fragCode;
    std::ifstream vFile;
    std::ifstream fFile;

    vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vFile.open(vertexPath);
        fFile.open(fragmentPath);
        std::stringstream vStream, fStream;

        vStream << vFile.rdbuf();
        fStream << fFile.rdbuf();

        vFile.close();
        fFile.close();

        vertCode = vStream.str();
        fragCode = fStream.str();
    } catch(std::ifstream::failure&) {
        std::cout << "ERROR: SHADER FILE COULD NOT BE READ" << std::endl;
    }
    const char* vCode = vertCode.c_str();
    const char* fCode = fragCode.c_str();

    // Compile shader components
    GLuint vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vCode, nullptr);
    glCompileShader(vertex);
    printOnError(vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fCode, nullptr);
    glCompileShader(fragment);
    printOnError(fragment);

    // Link shader program
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    // Check for link errors
    GLint success;
    GLchar log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cout << "ERROR: SHADER PROGRAM LINK FAILED\n" << log << std::endl;
    }

    // Clean up
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void sp::OGLShaderProgram::Use() {
    glUseProgram(program);
}



void sp::OGLShaderProgram::printOnError(GLuint shaderId) {
    GLchar log[512];
    GLint success;

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderId, 512, nullptr, log);
        std::cout << "ERROR: SHADER COMPILATION FAILED\n" << log << std::endl;
    }
}

GLint sp::OGLShaderProgram::GetUniformLocation(const std::string &name) const {
    return glGetUniformLocation(program, name.c_str());
}

GLint sp::OGLShaderProgram::GetAttributeLocation(const std::string &name) {
    return 0;
}

void sp::OGLShaderProgram::SetBool(const std::string &name, bool value) {
    glUniform1i(GetUniformLocation(name), (int)value);
}

void sp::OGLShaderProgram::SetInt(const std::string &name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void sp::OGLShaderProgram::SetFloat(const std::string &name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}
