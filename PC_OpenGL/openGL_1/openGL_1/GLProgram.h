//
//  GLProgram.h
//  openGL_1
//
//  Created by Wky on 2020/7/27.
//  Copyright © 2020 wangky. All rights reserved.
//

#ifndef GLProgram_h
#define GLProgram_h
#include <GL/glew.h>
#include "GLDefine.h"

class GLProgram
{
public:
    static GLuint createProgram(const char*vspath, const char*fspath);
private:
    
    static GLuint loadShader(GLenum type, const char *shaderStr);
};

#endif /* GLProgram_h */
