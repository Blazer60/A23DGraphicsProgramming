/**
 * @file PostProcessShader.h
 * @author Ryan Purse
 * @date 14/05/2022
 */


#pragma once

#include "Pch.h"
#include "Shader.h"


/**
 * A shader that can only render to a full screen quad.
 * @author Ryan Purse
 * @date 14/05/2022
 */
class PostProcessShader
    : public Shader
{
public:
    PostProcessShader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath);
    virtual ~PostProcessShader();
    
    void render();
    
protected:
    virtual void onRender() = 0;
    
    unsigned int mVao { 0 };
    unsigned int mEbo { 0 };
    unsigned int mVbo { 0 };
    int          mEboCount { 0 };
};
