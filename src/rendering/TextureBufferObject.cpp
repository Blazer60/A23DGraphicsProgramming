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

TextureBufferObject::TextureBufferObject(const glm::ivec2 &size, GLenum format)
    : mSize(size), mFormat(format)
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

void TextureBufferObject::imguiUpdate()
{
    const std::string name = "Buffer Object: " + std::to_string(mName);
    auto &style = ImGui::GetStyle();
    const auto temp = style.WindowPadding;
    style.WindowPadding = ImVec2(0.f, 0.f);
    if (ImGui::Begin(name.c_str(), nullptr))
    {
        // const ImVec2 size {
        //     ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x,
        //     ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y,
        // };
        // mSize = glm::vec2(size.x, size.y);
        auto size = ImVec2(mSize.x, mSize.y);
        ImGui::Image(reinterpret_cast<void *>(mName), size, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
    }
    style.WindowPadding = temp;
}

void TextureBufferObject::setClearColour(const glm::vec4 &clearColour)
{
    mClearColour = clearColour;
}
