/**
 * @file BloomShader.cpp
 * @author Ryan Purse
 * @date 15/05/2022
 */


#include "Bloom.h"
#include "Vertices.h"
#include "Primitives.h"
#include "FilePaths.h"
#include "imgui.h"

Bloom::Bloom()
{
    const auto mesh = primitives::plane<UvVertex>()[0];
    mVao      = mesh.renderInformation.vao;
    mEboCount = mesh.renderInformation.eboCount;
    mEbo      = mesh.ebo;
    mVbo      = mesh.vbo;
}

Bloom::~Bloom()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mEbo);
    glDeleteBuffers(1, &mVbo);
}

void Bloom::downSample(TextureBufferObject *input, const int mipLevel, FramebufferObject *output)
{
    mDownSample.bind();
    glBindVertexArray(mVao);
    output->bind();
    
    mDownSample.set("u_mvp_matrix", glm::mat4(1.f));
    mDownSample.set("u_mip_level", mipLevel);
    mDownSample.set("u_mip_size", output->getSize());
    
    glBindTextureUnit(0, input->getName());
    mDownSample.set("u_texture", 0);
    
    glDrawElements(GL_TRIANGLES, mEboCount, GL_UNSIGNED_INT, 0);
}

void Bloom::upSample(
    TextureBufferObject *lastUpSample, const int upSampleMipLevel,
    TextureBufferObject *downSample, const int downSampleMipLevel,
    FramebufferObject *output)
{
    mUpSample.bind();
    glBindVertexArray(mVao);
    output->bind();
    
    mUpSample.set("u_mvp_matrix", glm::mat4(1.f));
    mUpSample.set("u_up_mip_level", upSampleMipLevel);
    mUpSample.set("u_down_mip_level", downSampleMipLevel);
    mUpSample.set("u_mip_size", output->getSize() / 2);
    mUpSample.set("u_scale", mBloomScale);
    
    auto downSampleSize = downSample->getSize() / static_cast<int>(glm::pow(2, downSampleMipLevel));
    if (output->getSize() != downSampleSize)
        debug::log("Not the same size!");
    
    glBindTextureUnit(0, lastUpSample->getName());
    glBindTextureUnit(1, downSample->getName());
    
    mUpSample.set("u_up_sample_texture", 0);
    mUpSample.set("u_down_sample_texture", 1);
    
    glDrawElements(GL_TRIANGLES, mEboCount, GL_UNSIGNED_INT, 0);
}

void Bloom::imGuiUpdate()
{
    ImGui::DragFloat("Bloom Radius", &mBloomScale, 0.01f);
}
