/**
 * @file PostProcessingShader.h
 * @author Ryan Purse
 * @date 18/02/2022
 */


#pragma once

#include "Shader.h"

/**
 * A shader that only deals with textures as inputs and edits pixel data.
 * @author Ryan Purse
 * @date 18/02/2022
 */
class PostProcessingShader
        : public Shader
{
public:
    PostProcessingShader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath);
    
    void draw(unsigned int fbo, unsigned int texture, glm::vec2 screenSize);

protected:
    unsigned int    mVao       { 0 };
    unsigned int    mVbo       { 0 };
    unsigned int    mEbo       { 0 };
    int             mEboCount  { 0 };
};


