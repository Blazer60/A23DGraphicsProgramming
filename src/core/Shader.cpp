/**
 * @file Shader.cpp
 * @author Ryan Purse
 * @date 13/02/2022
 */


#include "Shader.h"
#include "gtc/type_ptr.hpp"
#include <fstream>
#include <sstream>

Shader::Shader(std::string_view vertexPath, std::string_view fragmentPath)
{
    mId = glCreateProgram();
    unsigned int vs = compile(GL_VERTEX_SHADER, vertexPath);
    unsigned int fs = compile(GL_FRAGMENT_SHADER, fragmentPath);
    
    glAttachShader(mId, vs);
    glAttachShader(mId, fs);
    glLinkProgram(mId);
    glValidateProgram(mId);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader()
{
    glDeleteProgram(mId);
}

unsigned int Shader::compile(unsigned int type, std::string_view path)
{
    std::ifstream file(path.data());
    if (file.bad() || file.fail())
        debug::log("Failed to open file at: " + std::string(path), debug::severity::Major);
    
    std::stringstream buffer;
    std::string line;
    
    while (std::getline(file, line))
        buffer << line << '\n';
    
    const std::string data(buffer.str());
    const char *dataPtr = data.c_str();
    
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &dataPtr, nullptr);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);  // OpenGL fails silently, so we need to check it ourselves.
    if (GL_TRUE == result)
        return id;
    
    const std::unordered_map<unsigned int, std::string> shaderTypes {
        { GL_VERTEX_SHADER, "Vertex" },
        { GL_FRAGMENT_SHADER, "Fragment" },
        { GL_GEOMETRY_SHADER, "Geometry" },
        { GL_COMPUTE_SHADER, "Fragment" },
    };
    
    // Failed to compile shader.
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char*)alloca(length * sizeof(char));  // No point doing a heap allocation.
    glGetShaderInfoLog(id, length, &length, message);
    glDeleteShader(id);
    
    debug::log("Failed to compile " + shaderTypes.at(type) + " shader:\n" + message,
               debug::severity::Major);
    return 0;
}

void Shader::bind() const
{
    glUseProgram(mId);
}

void Shader::unbind()
{
    glUseProgram(0);
}

int Shader::getLocation(const std::string &name)
{
    if (mCache.count(name) > 1)
        return mCache.at(name);
    
    int location = glGetUniformLocation(mId, name.data());
    if (location == -1)
        debug::log("Uniform '" + name + "' does not exist!", debug::severity::Warning);
    
    mCache[name] = location;
    return location;
}

void Shader::set(const std::string &uniformName, int value)
{
    glUniform1i(getLocation(uniformName), value);
}

void Shader::set(const std::string &uniformName, float value)
{
    glUniform1f(getLocation(uniformName), value);
}

void Shader::set(const std::string &uniformName, const glm::mat4 &value)
{
    glUniformMatrix4fv(getLocation(uniformName), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set(const std::string &uniformName, const glm::vec4 &value)
{
    glUniform4fv(getLocation(uniformName), 4, glm::value_ptr(value));
}

void Shader::set(const std::string &uniformName, const glm::vec3 &value)
{
    glUniform3fv(getLocation(uniformName), 3, glm::value_ptr(value));
}

void Shader::set(const std::string &uniformName, const glm::vec2 &value)
{
    glUniform2fv(getLocation(uniformName), 2, glm::value_ptr(value));
}
