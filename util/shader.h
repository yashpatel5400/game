/*******************************************************************************
 * Filename     :   shader.h
 * Content      :   Shader loading helper code
 * Created      :   June 21, 2020
 * Authors      :   Yash Patel, Joey de Vries (modelled from https://learnopengl.com/Getting-started/Textures)
 * Language     :   C++17
*******************************************************************************/

#pragma once

#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace gl_helper {
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    GLuint compile(const char* vShaderCode, const char* fShaderCode) {
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // shader Program
        GLuint program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);

        glLinkProgram(program);
        checkCompileErrors(program, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return program;
    }

    void setBool(GLuint program, const std::string& name, bool value)
    {
        glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
    }

    void setInt(GLuint program, const std::string& name, int value)
    {
        glUniform1i(glGetUniformLocation(program, name.c_str()), value);
    }

    void setFloat(GLuint program, const std::string& name, float value)
    {
        glUniform1f(glGetUniformLocation(program, name.c_str()), value);
    }

    void setVec2(GLuint program, const std::string& name, const glm::vec2& value)
    {
        glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
    }
    void setVec2(GLuint program, const std::string& name, float x, float y)
    {
        glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
    }

    void setVec3(GLuint program, const std::string& name, const glm::vec3& value)
    {
        glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
    }
    void setVec3(GLuint program, const std::string& name, float x, float y, float z)
    {
        glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
    }

    void setVec4(GLuint program, const std::string& name, const glm::vec4& value)
    {
        glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
    }
    void setVec4(GLuint program, const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
    }

    void setMat2(GLuint program, const std::string& name, const glm::mat2& mat)
    {
        glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(GLuint program, const std::string& name, const glm::mat3& mat)
    {
        glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(GLuint program, const std::string& name, const glm::mat4& mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
}