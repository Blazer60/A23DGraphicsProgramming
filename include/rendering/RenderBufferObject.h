/**
 * @file RenderBufferObject.h
 * @author Ryan Purse
 * @date 14/03/2022
 */


#pragma once


/**
 * [Description Goes Here]
 * @author Ryan Purse
 * @date 14/03/2022
 */
class RenderBufferObject
{
public:
    explicit RenderBufferObject(const glm::ivec2 &size);
    ~RenderBufferObject();
    
    void changeSize(const glm::ivec2 &size);

protected:
    void init();
    void deInit();
    
    unsigned int    mName       { 0 };
public:
    unsigned int getName() const;

protected:
    glm::ivec2      mSize       { 1024 };
    GLenum          mFormat     { GL_DEPTH24_STENCIL8 };
    GLenum          mAttachment { GL_DEPTH_STENCIL_ATTACHMENT };
public:
    GLenum getAttachment() const;
};


