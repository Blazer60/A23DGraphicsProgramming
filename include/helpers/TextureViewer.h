/**
 * @file TextureViewer.h
 * @author Ryan Purse
 * @date 18/05/2022
 */


#pragma once

#include "Pch.h"
#include "TextureBufferObject.h"

/**
 * @author Ryan Purse
 * @date 18/05/2022
 */
class TextureViewer
{
public:
    TextureViewer() = default;
    ~TextureViewer() = default;
    
    void imGuiUpdate(const std::string name, TextureBufferObject *texture, bool *show);
protected:
    virtual void onImGuiUpdate(TextureBufferObject *texture);
};
