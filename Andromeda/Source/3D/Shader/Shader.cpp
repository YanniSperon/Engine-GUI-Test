#include "Shader.h"
#include "Console.h"
#include "Global.h"
#include "ShaderManager.h"

#include <string>
#include <fstream>
#include <sstream>

namespace AD {
    Shader::Shader(const std::string& name)
        : m_ID(0), m_FakeUser(false), m_Path(name)
    {
        std::string geometryPath = name + std::string(".geom");
        std::string fragmentPath = name + std::string(".frag");
        LoadShader(geometryPath.c_str(), fragmentPath.c_str());

        //glUniformBlockBinding(m_ID, glGetUniformBlockIndex(m_ID, "ubo_Matrices"), 0);

        //glGenBuffers(1, &m_UBO);

        //glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
        //glBufferData(GL_UNIFORM_BUFFER, 128, nullptr, GL_DYNAMIC_DRAW);
        //glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBO, 0, 2 * sizeof(glm::mat4));
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_ID);
        //glDeleteBuffers(1, &m_UBO);
        Console::Warning("Deleted shader");
    }

    void Shader::LoadShader(const char* geometryShaderPath, const char* fragmentShaderPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        try
        {
            // open files
            vShaderFile.open(geometryShaderPath);
            fShaderFile.open(fragmentShaderPath);
            std::stringstream vShaderStream;
            std::stringstream fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            Console::Error("Shader Files \"%s\" And \"%s\" Not Successfully Loaded", geometryShaderPath, fragmentShaderPath);
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            Console::Error("Vertex Shader Failed Compilation \"%s\"", infoLog);
        };

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            Console::Error("Fragment Shader Failed Compilation \"%s\"", infoLog);
        };

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertex);
        glAttachShader(m_ID, fragment);
        glLinkProgram(m_ID);
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
            Console::Error("Vertex and Fragment Shader Linking Failed \"%s\"", infoLog);
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::Bind(/*const glm::mat4& projection, const glm::mat4& view*/)
    {
        glUseProgram(m_ID);
        //glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
        //glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection[0][0]);
        //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view[0][0]);
    }

    void Shader::SetBool(const std::string& name, bool value)
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
    }

    void Shader::SetInt(const std::string& name, int value)
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void Shader::SetFloat(const std::string& name, float value)
    {
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void Shader::Set2Float(const std::string& name, float x, float y)
    {
        glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
    }

    void Shader::Set3Float(const std::string& name, float x, float y, float z)
    {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
    }

    void Shader::Set4Float(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
    }

    void Shader::SetVec2(const std::string& name, const glm::vec2& value)
    {
        glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y);
    }

    void Shader::SetVec3(const std::string& name, const glm::vec3& value)
    {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::SetVec4(const std::string& name, const glm::vec4& value)
    {
        glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z, value.w);
    }

    void Shader::SetMat2(const std::string& name, const glm::mat2& value)
    {
        glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    void Shader::SetMat3(const std::string& name, const glm::mat3& value)
    {
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    void Shader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    GLuint Shader::GetID()
    {
        return m_ID;
    }

    void Shader::SetHasFakeUser(bool fakeUser)
    {
        m_FakeUser = fakeUser;
        if (!fakeUser) {
            ShaderManager::GetInstance()->MarkForCleanup();
        }
    }

    bool Shader::GetHasFakeUser()
    {
        return m_FakeUser;
    }

    std::string Shader::GetPath()
    {
        return m_Path;
    }

    void Shader::Unbind()
    {
        glUseProgram(0);
    }
}
