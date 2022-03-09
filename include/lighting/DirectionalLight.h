/**
 * @file DirectionalLight.h
 * @author Ryan Purse
 * @date 07/03/2022
 */


#pragma once


#include "detail/type_quat.hpp"

/**
 * A directional light (infinitely far away) that can be updated by imgui and sent to the shader.
 * @author Ryan Purse
 * @date 07/03/2022
 */
class DirectionalLight
{
public:
    DirectionalLight() = default;

    void onImguiUpdate();

protected:
    glm::vec3 mDirection { glm::normalize(glm::vec3(1.f, 1.f, 1.f)) };
public:
    const glm::vec3 &getDirection() const;
    
    const glm::vec3 &getIntensity() const;

protected:
    glm::vec3 mIntensity { 1.f };  // Intensity of each colour channel.
};


