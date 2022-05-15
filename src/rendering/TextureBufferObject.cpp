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
    init(GL_NEAREST);
}

TextureBufferObject::TextureBufferObject(
    const glm::ivec2 &size, GLenum format,
    GLenum minMagFilter, std::string debugName)
    :
    mSize(size), mFormat(format), mDebugName(std::move(debugName))
{
    init(minMagFilter);
}

TextureBufferObject::~TextureBufferObject()
{
    deInit();
}

void TextureBufferObject::init(GLenum minMagFilter)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &mName);
    glTextureParameteri(mName, GL_TEXTURE_MIN_FILTER, minMagFilter);
    glTextureParameteri(mName, GL_TEXTURE_MAG_FILTER, minMagFilter);
    glTextureParameteri(mName, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mName, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(mName, 1, mFormat, mSize.x, mSize.y);
}

void TextureBufferObject::deInit()
{
    glDeleteTextures(1, &mName);
}

void TextureBufferObject::imguiUpdate(bool *show, bool fill)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    if (ImGui::Begin(mDebugName.c_str(), show))
    {
        if (fill)
        {
            ImVec2 regionSize = ImGui::GetContentRegionAvail();
            ImGui::Image(reinterpret_cast<void *>(mName), regionSize, ImVec2(0, 1), ImVec2(1, 0));
        }
        else
        {
            ImGui::Image(reinterpret_cast<void *>(mName), ImVec2(mSize.x, mSize.y), ImVec2(0, 1), ImVec2(1, 0));
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

const glm::ivec2 &TextureBufferObject::getSize() const
{
    return mSize;
}
