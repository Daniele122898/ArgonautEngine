//
// Created by Daniele on 22/07/2021.
//

#include "Shader.h"

namespace Argonaut {
    Shader::Shader(const char *vertexPath, const char *fragmentPath) {
        // Retrieve the vertex/fragment source code from filepath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensure ifstream objects can throw exceptions
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure &e) {
            AG_CORE_ERROR("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ");
            AG_CORE_ERROR(e.what());
        }

        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        // compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        // print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            AG_CORE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
            AG_CORE_ERROR(infoLog);
        };

        // similiar for Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        // print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            memset(infoLog, 0, sizeof(infoLog));
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            AG_CORE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
            AG_CORE_ERROR(infoLog);
        };

        // shader Program
        ShaderId = glCreateProgram();
        glAttachShader(ShaderId, vertex);
        glAttachShader(ShaderId, fragment);
        glLinkProgram(ShaderId);
        // print linking errors if any
        glGetProgramiv(ShaderId, GL_LINK_STATUS, &success);
        if (!success) {
            memset(infoLog, 0, sizeof(infoLog));
            glGetProgramInfoLog(ShaderId, 512, nullptr, infoLog);
            AG_CORE_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED");
            AG_CORE_ERROR(infoLog);
        }

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::use() const {
        glUseProgram(ShaderId);
    }

    int Shader::getUniformLocation(const char *name) const {
        return glGetUniformLocation(ShaderId, name);
    }

    void Shader::setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ShaderId, name.c_str()), (int) value);
    }

    void Shader::setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ShaderId, name.c_str()), value);
    }

    void Shader::setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ShaderId, name.c_str()), value);
    }

    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(ShaderId, name.c_str()), 1, glm::value_ptr(value));
    }

    void Shader::setVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(ShaderId, name.c_str()), x, y);
    }

    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(ShaderId, name.c_str()), 1, glm::value_ptr(value));
    }

    void Shader::setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ShaderId, name.c_str()), x, y, z);
    }

    void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(ShaderId, name.c_str()), 1, glm::value_ptr(value));
    }

    void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(ShaderId, name.c_str()), x, y, z, w);
    }

    void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(ShaderId, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(ShaderId, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ShaderId, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    Shader::~Shader() {
        glDeleteProgram(ShaderId);
    }
}