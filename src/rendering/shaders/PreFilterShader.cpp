/**
 * @file PreFilterShader.cpp
 * @author Ryan Purse
 * @date 14/05/2022
 */


#include "PreFilterShader.h"
#include "Primitives.h"
#include "FramebufferObject.h"
#include "FilePaths.h"
#include "ext/matrix_transform.hpp"
#include "gtc/matrix_transform.hpp"


PreFilterShader::PreFilterShader(
    std::shared_ptr<FramebufferObject> preFilter,
    std::shared_ptr<TextureBufferObject> inputTexture)
    :
    PostProcessShader(path::shaders() + "/ScreenQuadV2.vert", path::shaders() + "/post-processing/pre-filter/PreFilter.frag"),
    mPreFilter(std::move(preFilter)),
    mInputTexture(std::move(inputTexture))
{
}

void PreFilterShader::onRender()
{
    mPreFilter->bind();
    set("u_mvp_matrix", glm::mat4(1.f));
    set("u_light_key_threshold", glm::vec3(1.f));
}
