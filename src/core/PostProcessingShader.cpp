/**
 * @file PostProcessingShader.cpp
 * @author Ryan Purse
 * @date 18/02/2022
 */


#include "PostProcessingShader.h"
#include "Primitives.h"

PostProcessingShader::PostProcessingShader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath)
    : Shader(vertexPath, fragmentPath)
{
    const auto mesh = primitives::plane<UvVertex>()[0];
    mVao = mesh.renderInformation.vao;
    mEboCount = mesh.renderInformation.eboCount;
    mEbo = mesh.ebo;
    mVbo = mesh.vbo;
}

void PostProcessingShader::draw(unsigned int fbo, unsigned int texture, glm::vec2 screenSize)
{
    glUseProgram(mId);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindVertexArray(mVao);
    
    glBindTextureUnit(0, texture);
    set("u_texture", 0);
    
    glDrawElements(GL_TRIANGLES, mEboCount, GL_UNSIGNED_INT, 0);
}
