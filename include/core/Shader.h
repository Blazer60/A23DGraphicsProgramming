/**
 * @file Shader.h
 * @author Ryan Purse
 * @date 13/02/2022
 */


#pragma once

#include "Pch.h"
#include <filesystem>

/**
 * Holds and compiles the shader program for OpenGL.
 * @author Ryan Purse
 * @date 13/02/2022
 */
class Shader
{
public:
    Shader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath);
    ~Shader();
    
    void bind() const;
    static void unbind();
    
    /**
     * @brief Sets a uniform within the shader.
     * @param uniformName - The name within the shader.
     * @param value - The value you want to pass to it.
     */
    void set(const std::string &uniformName, int value);
    
    /**
     * @brief Sets a uniform within the shader.
     * @param uniformName - The name within the shader.
     * @param value - The value you want to pass to it.
     */
    void set(const std::string &uniformName, float value);
    
    /**
     * @brief Sets a uniform within the shader.
     * @param uniformName - The name within the shader.
     * @param value - The value you want to pass to it.
     */
    void set(const std::string &uniformName, const glm::mat4 &value);
    
    /**
     * @brief Sets a uniform within the shader.
     * @param uniformName - The name within the shader.
     * @param value - The value you want to pass to it.
     */
    void set(const std::string &uniformName, const glm::vec4 &value);
    
    /**
     * @brief Sets a uniform within the shader.
     * @param uniformName - The name within the shader.
     * @param value - The value you want to pass to it.
     */
    void set(const std::string &uniformName, const glm::vec3 &value);
    
    /**
     * @brief Sets a uniform within the shader.
     * @param uniformName - The name within the shader.
     * @param value - The value you want to pass to it.
     */
    void set(const std::string &uniformName, const glm::vec2 &value);
    

protected:
    unsigned int mId { 0 };
    std::unordered_map<std::string , int> mCache;
    
    int getLocation(const std::string &name);
    
    static unsigned int compile(unsigned int type, std::string_view path);
};


