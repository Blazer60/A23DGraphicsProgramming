/**
 * @file MipViewerShader.cpp
 * @author Ryan Purse
 * @date 15/05/2022
 */


#include "MipViewer.h"
#include "FilePaths.h"
#include "ModelDestroyer.h"
#include "imgui.h"

MipViewer::MipViewer(std::shared_ptr<TextureBufferObject> inputTexture, std::string_view debugName) :
    mInputTexture(std::move(inputTexture)), mDebugName(debugName)
{
    const auto mesh = primitives::plane<UvVertex>()[0];
    mVao      = mesh.renderInformation.vao;
    mEboCount = mesh.renderInformation.eboCount;
    mEbo      = mesh.ebo;
    mVbo      = mesh.vbo;
    
    mOriginalSize = mInputTexture->getSize();
    init();
}

MipViewer::~MipViewer()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mEbo);
    glDeleteBuffers(1, &mVbo);
}

void MipViewer::init()
{
    mSize = mOriginalSize / static_cast<int>(glm::pow(2.f, mLevel));
    mFramebufferObject = std::make_unique<FramebufferObject>(mSize, GL_ONE, GL_ONE, GL_ALWAYS);
    mOutputTexture = std::make_shared<TextureBufferObject>(mSize, GL_RGB16, GL_LINEAR, GL_LINEAR, 1, mDebugName + " Mip Viewer");
    mFramebufferObject->attach(mOutputTexture, 0);
    mResize = false;
}

void MipViewer::render()
{
    if (mResize)
        init();
    
    mShader.bind();
    mFramebufferObject->bind();
    
    mShader.set("u_mip_level", mLevel);
    mShader.set("u_mip_size", glm::vec2(mSize));
    mShader.set("u_texture", mInputTexture->getName(), 2);
    mShader.set("u_mvp_matrix", glm::mat4(1.f));
    
    glBindVertexArray(mVao);
    glDrawElements(GL_TRIANGLES, mEboCount, GL_UNSIGNED_INT, 0);
}

void MipViewer::clear()
{
    mFramebufferObject->clear();
}

void MipViewer::imguiUpdate(bool *show)
{
    if (show && !*show)
        return;
    
    ImGui::PushID(mDebugName.c_str());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    if (ImGui::Begin(mDebugName.c_str(), show))
    {
        auto temp = mLevel;
        ImGui::SliderInt("Mip Level", &mLevel, 0, 7);
        if (temp != mLevel)
            mResize = true;
        ImGui::Checkbox("Fill?", &mFill);
        ImGui::Text("%i, %i", mSize.x, mSize.y);
        
        if (mFill)
        {
            ImVec2 regionSize = ImGui::GetContentRegionAvail();
            ImGui::Image(reinterpret_cast<void *>(mOutputTexture->getName()), regionSize, ImVec2(0, 1), ImVec2(1, 0));
        }
        else
        {
            const auto size = mOutputTexture->getSize();
            ImGui::Image(reinterpret_cast<void *>(mOutputTexture->getName()), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
        }
    }
    
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopID();
    

}
