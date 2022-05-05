/**
 * @file TextureBufferObject.cpp
 * @author Ryan Purse
 * @date 14/03/2022
 */


#include "rendering/TextureBufferObject.h"
#include "imgui.h"

TextureBufferObject::TextureBufferObject(const glm::ivec2 &size)
    : mSize(size)
{
    init();
}

TextureBufferObject::TextureBufferObject(const glm::ivec2 &size, GLenum format, std::string debugName)
    : mSize(size), mFormat(format), mDebugName(std::move(debugName))
{
    init();
}

TextureBufferObject::~TextureBufferObject()
{
    deInit();
}

void TextureBufferObject::init()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &mName);
    glTextureParameteri(mName, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(mName, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(mName, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mName, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(mName, 1, mFormat, mSize.x, mSize.y);
}

void TextureBufferObject::deInit()
{
    glDeleteTextures(1, &mName);
}

void TextureBufferObject::changeSize(const glm::ivec2 &size)
{
    deInit();
    mSize = size;
    init();
}

void TextureBufferObject::imguiUpdate(bool *show)
{
    const std::string name = mDebugName + " | Id: " + std::to_string(mName);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    if (ImGui::Begin(name.c_str(), show))
    {
        ImVec2 regionSize = ImGui::GetContentRegionAvail();
        ImGui::Image(reinterpret_cast<void *>(mName), regionSize, ImVec2(0, 1), ImVec2(1, 0));
        
        if ((mSize.x != regionSize.x || mSize.y != regionSize.y) && !mDisableResizing)
        {
            changeSize(glm::ivec2(regionSize.x, regionSize.y));
            reattach();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void TextureBufferObject::setClearColour(const glm::vec4 &clearColour)
{
    mClearColour = clearColour;
}

unsigned int TextureBufferObject::getName() const
{
    return mName;
}
