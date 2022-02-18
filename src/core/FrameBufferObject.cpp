/**
 * @file FrameBufferObject.cpp
 * @author Ryan Purse
 * @date 18/02/2022
 */


#include "FrameBufferObject.h"
#include "gtc/type_ptr.hpp"
#include "imgui.h"

FrameBufferObject::FrameBufferObject(glm::ivec2 size)
    : mSize(size)
{
    glCreateFramebuffers(1, &mFboId);
    glCreateTextures(GL_TEXTURE_2D, 1, &mTexId);
    
    glTextureParameteri(mTexId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(mTexId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(mTexId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mTexId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(mTexId, 1, GL_RGB8, size.x, size.y);
    glNamedFramebufferTexture(mFboId, GL_COLOR_ATTACHMENT0, mTexId, 0);
    
    glCreateRenderbuffers(1, &mRboId);
    glNamedRenderbufferStorage(mRboId, GL_DEPTH24_STENCIL8, size.x, size.y);
    glNamedFramebufferRenderbuffer(mTexId, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRboId);
    
    const unsigned int fboStatus = glCheckNamedFramebufferStatus(mFboId, GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        debug::log("Framebuffer error of: " + std::to_string(fboStatus), debug::severity::Major);
}

void FrameBufferObject::clear()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
    glClearNamedFramebufferfv(mFboId, GL_COLOR, 0, glm::value_ptr(mClearColour));
    glClearNamedFramebufferfv(mFboId, GL_DEPTH, 0, &mDepth);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::update()
{

}

void FrameBufferObject::imguiUpdate()
{
    const std::string name = "Buffer Object: " + std::to_string(mTexId);
    auto &style = ImGui::GetStyle();
    const auto temp = style.WindowPadding;
    style.WindowPadding = ImVec2(0.f, 0.f);
    if (ImGui::Begin(name.c_str(), nullptr))
    {
        // const ImVec2 size { static_cast<float>(mSize.x), static_cast<float>(mSize.y) };
        const ImVec2 size {
            ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x,
            ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y,
            };
        mSize = glm::vec2(size.x, size.y);
        ImGui::Image(reinterpret_cast<void *>(mTexId), size, ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
    style.WindowPadding = temp;
}

unsigned int FrameBufferObject::getId() const
{
    return mFboId;
}

const glm::vec2 &FrameBufferObject::getSize() const
{
    return mSize;
}

unsigned int FrameBufferObject::getTexture() const
{
    return mTexId;
}
