/**
 * @file TextureBufferObject.h
 * @author Ryan Purse
 * @date 14/03/2022
 */


#pragma once


#include <functional>

/**
 * Contains a texture that can be read and written to by openGl.
 * Most commonly used by attaching itself to framebuffer objects.
 * @author Ryan Purse
 * @date 14/03/2022
 */
class TextureBufferObject
{
    friend class FrameBufferObject;
public:
    explicit TextureBufferObject(const glm::ivec2 &size);
    
    /**
     * @see <a href="https://www.khronos.org/opengl/wiki/Image_Format">Image Formatting</a>
     * @param format - The OpenGL format with the syntax GL_(components)(size)[type].
     */
    TextureBufferObject(const glm::ivec2 &size, GLenum format, std::string debugName="");
    
    ~TextureBufferObject();
    
    void imguiUpdate();
    
    void changeSize(const glm::ivec2 &size);
    
    void setClearColour(const glm::vec4 &clearColour);
    
protected:
    void init();
    void deInit();
    
    std::function<void()> reattach  { [](){} };
    const bool mDisableResizing { true };
    
    unsigned int    mName           { 0 };
    GLenum          mFormat         { GL_RGB16 };
    glm::ivec2      mSize           { 1024 };
    glm::vec4       mClearColour    { 0.f };
    std::string     mDebugName      { "" };
};


