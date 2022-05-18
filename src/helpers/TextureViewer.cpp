/**
 * @file TextureViewer.cpp
 * @author Ryan Purse
 * @date 18/05/2022
 */


#include "TextureViewer.h"
#include "imgui.h"


void TextureViewer::onImGuiUpdate(TextureBufferObject *texture)
{
    ImVec2 regionSize = ImGui::GetContentRegionAvail();
    ImGui::Image(reinterpret_cast<void *>(texture->getName()), regionSize, ImVec2(0, 1), ImVec2(1, 0));
}

void TextureViewer::imGuiUpdate(const std::string name, TextureBufferObject *texture, bool *show)
{
    if (show && !*show)
        return;
    
    ImGui::PushID(name.c_str());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    if (ImGui::Begin(name.c_str(), show))
        onImGuiUpdate(texture);  // Can be overridden
    
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopID();
}
