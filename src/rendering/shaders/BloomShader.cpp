/**
 * @file BloomShader.cpp
 * @author Ryan Purse
 * @date 15/05/2022
 */


#include "BloomShader.h"
#include "Vertices.h"
#include "Primitives.h"
#include "FilePaths.h"

BloomShader::BloomShader()
    : Shader(path::shaders() + "ScreenOverlay.vert", path::shaders() + "post-processing/BloomDownSample.frag")
{
    const auto mesh = primitives::plane<UvVertex>()[0];
    mVao      = mesh.renderInformation.vao;
    mEboCount = mesh.renderInformation.eboCount;
    mEbo      = mesh.ebo;
    mVbo      = mesh.vbo;
}

BloomShader::~BloomShader()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mEbo);
    glDeleteBuffers(1, &mVbo);
}

void BloomShader::downSample(TextureBufferObject *input, const int mipLevel, FramebufferObject *output)
{
    bind();
    glBindVertexArray(mVao);
    output->bind();
    
    set("u_mvp_matrix", glm::mat4(1.f));
    set("u_mip_level", mipLevel);
    set("u_mip_size", output->getSize() * 2);
    
    glBindTextureUnit(0, input->getName());
    set("u_texture", 0);
    
    glDrawElements(GL_TRIANGLES, mEboCount, GL_UNSIGNED_INT, 0);
}
