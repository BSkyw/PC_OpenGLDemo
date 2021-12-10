//
//  GLDefine.h
//  openGL_1
//
//  Created by Wky on 2020/7/27.
//  Copyright © 2020 wangky. All rights reserved.
//

#ifndef GLDefine_h
#define GLDefine_h



#define  GLLOGE(...) printf("<[GLCore] ERROR:> ");printf(__VA_ARGS__);printf("\n");


//OpenGL 宏定义
#define GLCheckError() (glGetError() == GL_NO_ERROR)

#define GL_DELETE_ARRAY_VERTEX(x) if(x){glDeleteVertexArrays(1,&x);x = 0;}

#define GL_DELETE_TEXTURE(x) if(x) { glDeleteTextures(1,&x); x = 0; }

#define GL_DELETE_SHADER(x) if(x) { glDeleteShader(x); x = 0;}

#define GL_DELETE_PROGRAM(x) if(x) { glDeleteProgram(x); x = 0;}

#define GL_DELETE_FRAMEBUFFER(x) if(x) { glDeleteFramebuffers(1,&x); x = 0;}

#define GL_DELETE_RENDERBUFFER(x) if(x) { glDeleteRenderbuffers(1,&x); x = 0;}

#define GL_DELETE_BUFFERS(x) if(x) { glDeleteBuffers(1,&x); x = 0;}

#define INVALID_GLVALUE         -1
#define INVALID_TEXTURE         0
#define INVALID_PROGRAM         0
#define INVALID_FRAMEBUFFER     0
#define INVALID_SHADER          0
#define INVALID_RENDERBUFFER    0
#define INVALID_BUFFERS         0
#endif /* GLDefine_h */
