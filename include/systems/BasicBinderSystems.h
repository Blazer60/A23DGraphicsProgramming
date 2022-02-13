/**
 * @file BasicBinderSystems.h
 * @author Ryan Purse
 * @date 12/02/2022
 */


#pragma once

#include "Ecs.h"
#include "Components.h"

/**
 * Bind a basic mesh to a Vbo and Ebo.
 * @author Ryan Purse
 * @date 12/02/2022
 */
class BasicMeshBinderSystem
        : public ecs::BaseSystem<std::shared_ptr<BasicMesh>, Vbo, Ebo>
{
public:
    void onStart() override;
    void onUpdate() override;
};


/**
 * Bind a Vao to the Vbo and Ebo using the Basic Vertex specification.
 * @author Ryan Purse
 * @date 12/02/2022
 */
class BasicVaoBinderSystem
        : public ecs::BaseSystem<Vao, Vbo, Ebo>
{
public:
    void onStart() override;
    void onUpdate() override;
};

