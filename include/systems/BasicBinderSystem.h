/**
 * @file BasicBinderSystems.h
 * @author Ryan Purse
 * @date 12/02/2022
 */


#pragma once

#include "Ecs.h"
#include "Components.h"

/**
 * Bind a Vao to the Vbo and Ebo using the Basic Vertex specification given the specified mesh.
 * @author Ryan Purse
 * @date 12/02/2022
 */
class BasicBinderSystem
        : public ecs::BaseSystem<RenderCoreElements, Vbo, Ebo, BasicSharedMesh>
{
public:
    void onStart() override;
    void onUpdate() override;
};

