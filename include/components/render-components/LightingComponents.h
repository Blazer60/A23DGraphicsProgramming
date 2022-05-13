/**
 * @file LightingComponents.h
 * @author Ryan Purse
 * @date 23/03/2022
 */


#pragma once

#include "Pch.h"
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
    
    /**
     * A light source that emits from a single point.
     * @see struct Position
     */
    struct PointLight
    {
        /** Intensity of each colour channel. */
        glm::vec3   intensity   { 1.f };
        
        /** How far the light will travel before being completely cut off. */
        float       distance    { 10.f };
        
        /** Increases the overall brightness of the light. */
        float       power       { 1.f };
    };
}


