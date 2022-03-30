/**
 * @file LightingComponents.h
 * @author Ryan Purse
 * @date 23/03/2022
 */


#pragma once

#include "glm.hpp"

namespace light
{
    /**
     * @brief A light source that is considered to be infinitely far away. Such as the sun.
     */
    struct DirectionalLight
    {
        glm::vec3 direction { glm::normalize(glm::vec3(1.f, 1.f, 1.f)) };
        glm::vec3 intensity { 1.f };  // Intensity of each colour channel.
    };
}


