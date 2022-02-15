/**
 * @file UvBinderSystem.h
 * @author Ryan Purse
 * @date 15/02/2022
 */


#pragma once

#include "Ecs.h"
#include "Components.h"

/**
 * Binds the Vao to the Vbo and Ebo using the UvVertex specification given the specified mesh.
 * @author Ryan Purse
 * @date 15/02/2022
 */
class UvBinderSystem
        : public ecs::BaseSystem<RenderCoreElements, Vbo, Ebo, UvSharedMesh>
{
public:
    void onStart() override;
    void onUpdate() override;
};


