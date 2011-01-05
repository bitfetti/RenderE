/* 
 * File:   Camera.cpp
 * Author: morten
 * 
 * Created on October 31, 2010, 2:12 PM
 */

#include "Camera.h"

#include <iostream>
#include "GL/glew.h"
#include "math/Mathf.h"
#include "textures/Texture2D.h"

namespace render_e {

Camera::Camera()
:Component(CameraType),cameraMode(ORTHOGRAPHIC),
        nearPlane(-1),farPlane(1),
        left(-1), right(1),
        bottom(-1),top(1), clearColor(0,0,0,1),renderToTexture(false){
    SetClearMask(COLOR_BUFFER|DEPTH_BUFFER);
    SetClearColor(Vector4(0,0,0,1));
}

Camera::~Camera() {
    SetRenderToTexture(false, COLOR_BUFFER, NULL);
}

void Camera::SetProjection(float fieldOfView, float aspect, float nearPlane, float farPlane){
    assert(nearPlane!=0.0f); // See http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml#notes
    this->cameraMode = PERSPECTIVE;
    this->fieldOfView = fieldOfView;
    this->aspect = aspect;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    
    // inspired by
    // Source code from
    // http://gpwiki.org/index.php/OpenGL_Tutorial_Framework:First_Polygon
    top = nearPlane * tan(fieldOfView * Mathf::PI / 360.0f);
    bottom = -top;
    left = bottom * aspect;
    right = top * aspect;
     
}

void Camera::SetOrthographic(float left, float right, float bottom,	float top,float nearPlane, float farPlane){
    this->cameraMode = ORTHOGRAPHIC;
    this->left = left;
    this->right = right;
    this->bottom = bottom;
    this->top = top;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
}

void Camera::SetClearMask(int clearBitMask){
    clearMaskNative = 0;
    if (clearBitMask&COLOR_BUFFER){
        clearMaskNative |= GL_COLOR_BUFFER_BIT;
    }
    if (clearBitMask&DEPTH_BUFFER){
        clearMaskNative |= GL_DEPTH_BUFFER_BIT;
    }
    if (clearBitMask&STENCIL_BUFFER){
        clearMaskNative |= GL_STENCIL_BUFFER_BIT;
    }
    clearMask = clearBitMask;
}

int Camera::GetClearMask(){
    return clearMask;
}

void Camera::Clear(){
    glClear(clearMaskNative);
}

void Camera::Setup(){
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (cameraMode==PERSPECTIVE){
        glFrustum(left, right, bottom, top, nearPlane, farPlane);
//        gluPerspective( /* field of view in degree */ fieldOfView,
//        /* aspect ratio */ aspect,
//        /* Z near */ nearPlane, /* Z far */ farPlane);
    } else {
        glOrtho(left, right, bottom, top, nearPlane, farPlane);
    }
    glMatrixMode(GL_MODELVIEW);
    if (renderToTexture){
        BindFrameBufferObject();
    }
}

void Camera::TearDown(){
    if (renderToTexture){
        UnBindFrameBufferObject();
    }
}

void Camera::SetClearColor(Vector4 clearColor){ 
    this->clearColor = clearColor; 
}

void Camera::SetRenderToTexture( bool doRenderToTexture , CameraBuffer framebufferTargetType, Texture2D *texture2d){
    if (renderToTexture == doRenderToTexture){
        return;
    }
    
    renderToTexture = doRenderToTexture;
    if (renderToTexture){
        glGenFramebuffersEXT (1, &framebufferId);
        framebufferTextureId = texture2d->GetTextureId();
        framebufferTextureType = GL_TEXTURE_2D;
        switch (framebufferTargetType) {
            case COLOR_BUFFER:
                this->framebufferTargetType = GL_COLOR_ATTACHMENT0;
                break;
            case DEPTH_BUFFER:
                this->framebufferTargetType = GL_DEPTH_ATTACHMENT;
                break;
            case STENCIL_BUFFER:
            default:
                this->framebufferTargetType = GL_STENCIL_ATTACHMENT;
        }
    } else {
        glDeleteFramebuffersEXT(1, &framebufferId);
    }
    
}

void Camera::BindFrameBufferObject(){
    assert(renderToTexture);
    
    // target parameter : GL_FRAMEBUFFER = read/write, GL_DRAW_FRAMEBUFFER = write only
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, framebufferId);
    
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, framebufferTargetType, framebufferTextureType, framebufferTextureId, 0);
}

void Camera::UnBindFrameBufferObject(){
    glBindFramebuffer( GL_FRAMEBUFFER, 0);
}
}
