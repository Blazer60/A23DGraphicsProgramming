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
     * Colour = Intensity / (1 + (x/distance)^power), where x is how far away you are from the light source.
     */
    struct PointLight
    {
        /** Where the light is in world space */
        glm::vec4   position    { 0.f };
        
        /** Intensity of each colour channel. */
        glm::vec3   intensity   { 1.f };
        
        /** How far the light will travel before falling off half its intensity. */
        float       distance    { 10.f };
        
        /** Determines the rate of which the light 'falls off'. */
        float       power       { 1.f };
    };
}


