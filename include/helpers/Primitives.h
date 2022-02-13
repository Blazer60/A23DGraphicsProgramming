/**
 * @file Primitives.h
 * @author Ryan Purse
 * @date 12/02/2022
 */


#pragma once

#include "Components.h"

namespace primitives
{
    [[nodiscard]] std::shared_ptr<BasicMesh> basicCube();
    [[nodiscard]] std::shared_ptr<BasicMesh> basicTriangle();
}