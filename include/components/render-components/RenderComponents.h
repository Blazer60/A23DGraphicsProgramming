/**
 * @file RenderComponents.h
 * @author Ryan Purse
 * @date 21/02/2022
 */


#pragma once

#include "Pch.h"

/**
 * @brief Core elements used for rendering an item to the screen by OpenGL.
 * ONLY items that never change each frame. E.g.: No uniforms
 */
struct RenderInformation
{
    unsigned int    vao      { 0 };
    unsigned int    fbo      { 0 };
    int             eboCount { 0 };
    // Matrix Uniforms are not included since they need to be updated every frame.
};

/** Vertex Buffer Object. Stores vertex data on the GPU. */
struct Vbo
{
    unsigned int id;
};

/** Element Buffer Object. Tells the GPU what order each vertex should be rendered. */
struct Ebo
{
    unsigned int id;
};
