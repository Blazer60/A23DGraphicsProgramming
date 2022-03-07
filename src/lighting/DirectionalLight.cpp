/**
 * @file DirectionalLight.cpp
 * @author Ryan Purse
 * @date 07/03/2022
 */


#include "lighting/DirectionalLight.h"

#include "gtc/type_ptr.hpp"
#include "imgui.h"

void DirectionalLight::onImguiUpdate()
{
    if (ImGui::Begin("Directional Light"))
    {
        ImGui::DragFloat3("Direction", glm::value_ptr(mDirection));
        ImGui::ColorEdit3("Intensity", glm::value_ptr(mIntensity));
        ImGui::End();
    }
}
