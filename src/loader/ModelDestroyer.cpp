/**
 * @file ModelDestroyer.cpp
 * @author Ryan Purse
 * @date 18/05/2022
 */


#include "ModelDestroyer.h"

void destroy::material(BlinnPhongMaterial &mat)
{
    glDeleteTextures(1, &mat.diffuseTextureId);
    mat.diffuseTextureId = 0;
}

void destroy::material(EmissivePbrMaterial &mat)
{
    glDeleteTextures(1, &mat.diffuseTextureId);
    glDeleteTextures(1, &mat.emissiveTextureId);
    
    mat.diffuseTextureId = 0;
    mat.emissiveTextureId = 0;
}

void destroy::material(NoMaterial &mat)
{

}
