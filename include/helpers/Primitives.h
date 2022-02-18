/**
 * @file Primitives.h
 * @author Ryan Purse
 * @date 12/02/2022
 */


#pragma once

#include "Components.h"

namespace primitives
{
    [[nodiscard]] BasicSharedMesh   basicCube();
    [[nodiscard]] BasicSharedMesh   basicTriangle();
    
    [[nodiscard]] UvSharedMesh      uvCube();
    [[nodiscard]] UvSharedMesh      uvPlane();
}