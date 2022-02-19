/**
 * @file Primitives.h
 * @author Ryan Purse
 * @date 12/02/2022
 */


#pragma once

#include "Components.h"

namespace primitives
{
    // Basic Meshes
    [[nodiscard]] SharedMesh  basicCube();
    [[nodiscard]] SharedMesh  basicTriangle();
    
    // UV Meshes
    [[nodiscard]] SharedMesh  uvCube();
    [[nodiscard]] SharedMesh  uvPlane();
}