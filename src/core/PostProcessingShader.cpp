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
    using VertexSpecification = UvVertex;
    
    const auto mesh = primitives::uvPlane();
    
    glCreateBuffers(1, &mVbo);
    glCreateBuffers(1, &mEbo);
    glNamedBufferData(mVbo, mesh->verticesCount() * sizeof(VertexSpecification), mesh->verticesData(), GL_STATIC_DRAW);
    glNamedBufferData(mEbo, mesh->indicesCount() * sizeof(unsigned int), mesh->indicesData(), GL_STATIC_DRAW);
    mEboCount = mesh->indicesCount();
    
    glCreateVertexArrays(1, &mVao);
    
    const unsigned int bindingIndex = 0;
    const unsigned int offSet = 0;
    const unsigned int stride = sizeof(VertexSpecification);
    
    glEnableVertexArrayAttrib(mVao, 0);
    // vao, attrib index, count, type, normalized, offset
    glVertexArrayAttribFormat(mVao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexSpecification, position));
    glVertexArrayAttribBinding(mVao, 0, bindingIndex);
    
    glEnableVertexArrayAttrib(mVao, 1);
    glVertexArrayAttribFormat(mVao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(VertexSpecification, uv));
    glVertexArrayAttribBinding(mVao, 1, bindingIndex);
    
    glVertexArrayVertexBuffer(mVao, bindingIndex, mVbo, offSet, stride);
    glVertexArrayElementBuffer(mVao, mEbo);
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
