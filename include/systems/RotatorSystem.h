/**
 * @file RotatorSystem.h
 * @author Ryan Purse
 * @date 15/02/2022
 */


#pragma once

#include "Ecs.h"
#include "Components.h"

/**
 * Rotates objects based on their Rotator settings.
 * @author Ryan Purse
 * @date 15/02/2022
 */
class RotatorSystem
        : public ecs::BaseSystem<Rotator, Transform>
{
public:
    void onStart() override;
    
    void onUpdate() override;
};


