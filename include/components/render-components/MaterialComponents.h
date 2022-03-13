/**
 * @file MaterialComponents.h
 * @author Ryan Purse
 * @date 09/03/2022
 */


#pragma once

#include "glm.hpp"
#include "TextureLoader.h"

struct MtlMaterial;

/** An empty shell do define a material with. */
struct Material
{
    // You should inherit and define your own material from this class.
    Material() = default;
    explicit Material(const MtlMaterial &mtl)
    {
        debug::log("Not Implemented! Define your own conversion constructor for MtlMaterial", debug::severity::Fatal);
    }
};

/**
 * @brief What a material looks like within a .mtl file. You can define a constructor to translate to your own custom one to support
 */
struct MtlMaterial
        : public Material
{
    MtlMaterial() = default;
    ~MtlMaterial() = default;
    
    unsigned int    nS      { 0 };
    glm::vec3       kA      { 0.f };
    glm::vec3       kD      { 0.f };
    glm::vec3       kS      { 0.f };
    unsigned int    d       { 1 };
    unsigned int    illum   { 2 };
    std::string     MapKd   { "" };
    std::string     MapNs   { "" };
};

/**
 * @brief Explicit definition to tell loaders that is should not load material files.
 */
struct NoMaterial
        : public Material
{
    NoMaterial() = default;
};

/**
 * @brief A material to describe how light should react with the surface of the model. This includes a single texture.
 */
struct BlinnPhongMaterial
        : public Material
{
    BlinnPhongMaterial()  = default;
    ~BlinnPhongMaterial() = default;
    
    explicit BlinnPhongMaterial(const MtlMaterial &mtl)
        : ambientColour(mtl.kA), diffuseColour(mtl.kD),
          specularColour(mtl.kS), specularExponent(mtl.nS),
          diffuseTextureId(load::texture(mtl.MapKd)) {}
    
    glm::vec3       ambientColour       { 0.2f };
    glm::vec3       diffuseColour       { 0.8f };
    glm::vec3       specularColour      { 1.f };
    unsigned int    specularExponent    { 10 };
    unsigned int    diffuseTextureId    { 0 };
};