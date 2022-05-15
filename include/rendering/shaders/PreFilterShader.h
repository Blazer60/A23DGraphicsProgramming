/**
 * @file PreFilterShader.h
 * @author Ryan Purse
 * @date 14/05/2022
 */


#pragma once

#include "Pch.h"
#include "TextureBufferObject.h"
#include "PostProcessShader.h"


/**
 * Runs just after the main geometry and lighting pass to setup post-processing bloom.
 * @author Ryan Purse
 * @date 14/05/2022
 */
class PreFilterShader
    : public PostProcessShader
{
public:
    PreFilterShader(
        std::shared_ptr<FramebufferObject> preFilter,
        std::shared_ptr<TextureBufferObject> inputTexture);
    
protected:
    void onRender() override;
    
    std::shared_ptr<FramebufferObject> mPreFilter;
    std::shared_ptr<TextureBufferObject> mInputTexture;
};
