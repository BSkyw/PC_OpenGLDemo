//
//  GLProgram.cpp
//  openGL_1
//
//  Created by Wky on 2020/7/27.
//  Copyright © 2020 wangky. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "GLProgram.h"
#include "CommonUtil.h"


GLuint GLProgram::createProgram(const char *vspath, const char *fspath)
{
    GLuint program = INVALID_PROGRAM;
    if (vspath != nullptr && fspath != nullptr) {
        const char* vertexCode = CommonUtil::readFile(vspath);//readFile(vPath);
        const char* fertexCode = CommonUtil::readFile(fspath);//readFile(vPath);

        GLuint vsS = loadShader(GL_VERTEX_SHADER, vertexCode);
        if (vsS == INVALID_SHADER) {
            GLLOGE("ERROR：请检查顶点着色器！%s",vspath);
            return INVALID_PROGRAM;
        }
        GLuint fsS = loadShader(GL_FRAGMENT_SHADER, fertexCode);
        if (fsS == INVALID_SHADER) {
            GLLOGE("ERROR：请检查片元着色器！%s",fspath);
            return INVALID_PROGRAM;
        }
        program = glCreateProgram();
        if (program == INVALID_PROGRAM) {
            GLLOGE("ERROR：请检查GL上下文，GL环境，GL搭建流程！");
            GL_DELETE_SHADER(vsS);
            GL_DELETE_SHADER(fsS);
            return INVALID_PROGRAM;
        }
        glAttachShader (program, vsS);
        glAttachShader (program, fsS);
        glLinkProgram(program);
        GLint success = INVALID_SHADER;
        GLchar infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            GLLOGE("create program error：%s", infoLog );
            glDeleteShader(vsS);
            glDeleteShader(fsS);
        }
        glDeleteShader(vsS);
        glDeleteShader(fsS);
        
    }
    return program;
}

GLuint GLProgram::loadShader(GLenum type, const char *shaderStr){
    GLuint vfShader = glCreateShader(type);
    glShaderSource(vfShader, 1, &shaderStr, NULL);
    glCompileShader(vfShader);
    GLint success = INVALID_SHADER;
    glGetShaderiv(vfShader, GL_COMPILE_STATUS, &success);
    GLchar infoLog[512];
    if(!success){
        glGetShaderInfoLog(vfShader, 512, NULL, infoLog);
        GLLOGE("%s", infoLog);
        glDeleteShader(vfShader);
        return INVALID_SHADER;
    }
    return vfShader;
}
