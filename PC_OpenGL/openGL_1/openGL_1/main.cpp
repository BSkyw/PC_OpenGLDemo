//
//  main.cpp
//  openGL_1
//
//  Created by Wky on 2020/7/14.
//  Copyright © 2020 wangky. All rights reserved.
//

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

#include "CommonUtil.h"
#include "GLProgram.h"

const int SCREEN_WIDTH = 360, SCREEN_HEIGHT = 640;
GLuint fbo1;
GLuint texture1 , aaaa, bbbb;

bool keys[1024];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(action == GLFW_PRESS) {
        keys[key] = true;
    }
    else if(action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
//     mouseX = xpos;
//     mouseY = ypos;
}
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

GLuint getTextureId(const char *path)
{
    GLuint texture_id;
    int width,height,nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //对齐字节
    if(!data)
    {
        std::cout << "Failed to load texture" << std::endl;
        stbi_image_free(data);
    }else{
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        if (nrChannels == 3){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        }
        if (nrChannels == 1){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        stbi_image_free(data);
    }
     return texture_id;
}


char* readFile(const char* fileName)
{
    char* result = 0;
    FILE* file = fopen(fileName, "r");
    if(file) {
        fseek(file, 0, SEEK_END);
        size_t fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        result = (char*)malloc(fileSize + 1);
        fread(result, fileSize, 1, file);
        result[fileSize] = 0;

        fclose(file);
    }
    else {
        printf("cannot find file %s\n", fileName);
    }

    return result;
}

//GLuint create_shader(const char *vPath, const char *fPath)
//{
//    GLuint shaderProgram;
//    const char* vertexCode = CommonUtil::readFile(vPath);//readFile(vPath);
//    const char* fragmentCode = CommonUtil::readFile(fPath);
//
//    GLint success;
//    GLchar infoLog[512];
//    GLuint vertexShader;
//    vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexCode, NULL);
//    glCompileShader(vertexShader);
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if(!success) {
//    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
//        printf("%s\n", infoLog);
//    }
//    GLuint fragmentShader;
//    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
//    glCompileShader(fragmentShader);
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if(!success) {
//    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
//        printf("%s\n", infoLog);
//    }
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if(!success) {
//    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//    }
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    return shaderProgram;
//}

void init_glfw(GLFWwindow **win)
{
    glfwSetErrorCallback(error_callback);
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    *win = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "great concept", NULL, NULL);
    if(!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(*win);
    
    glfwSetCursorPosCallback(*win, mouse_callback);
    glfwSetKeyCallback(*win, key_callback);

    // Set this to true so glew knows to use modern opengl
    glewExperimental = GL_TRUE;
    glewInit();
    
    int vWidth, vHeight;
    glfwGetFramebufferSize(*win, &vWidth, &vHeight);
    glViewport(0, 0, vWidth, vHeight);
    
    // enable depths testing to remove pixels which is behind other pixels
    glEnable(GL_DEPTH_TEST);
}

GLuint createRectangle()
{   glGenFramebuffers(1,&fbo1);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo1);
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 720, 1280,0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture1, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    //* 9 / 16
     const GLfloat vertices[] = {
         -1.0f,-1.0f , 0.0f, 0.0f,
         1.0f, -1.0f , 1.0f, 0.0f,
         -1.0f, 1.0f , 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 1.0f
     };
    unsigned int indices[] = {0,1,2,3};
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, aaaa);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return VAO;
}


GLuint createCube()
{
    const GLfloat vertices[] = {
        
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int indices[] = {
         0, 1, 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35
         
     };
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, getTextureId("image.jpg"));
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return VAO;
}

int main(int argc, char *argv[])
{
    char* file = new char[10];//ok;    //定义一个整型变量,用来存储用户输入的年龄
    printf("您今年多大了？: ");
    scanf("%s", file);
    printf("您的年龄是: %s岁 \n", file);

    GLFWwindow *win;
    init_glfw(&win);
    char *fff = "/Users/mac/Desktop/WechatIMG331.jpeg";
    aaaa = getTextureId(fff);
    bbbb = getTextureId("qian_000.png");
//    GLuint shaderProgram = create_shader("vertexshader.vs", "fragmentshader.frag");
    
    GLuint shaderProgram = GLProgram::createProgram("vertexshader.vs", "fragmentshader.frag");
    GLuint shaderProgram2 = GLProgram::createProgram("2DShader/vertexshader1.vs", "2DShader/fragmentshader1.frag");

    GLuint Rectangle = createRectangle();
    GLuint cube = createCube();

    while(!glfwWindowShouldClose(win)) {
        glfwPollEvents();

        glBindFramebuffer(GL_FRAMEBUFFER,fbo1);
//
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        glUseProgram(shaderProgram);
        GLuint tex1 = glGetUniformLocation(shaderProgram, "ourTexture");
//        GLuint tex2 = glGetUniformLocation(shaderProgram, "ourTexture2");

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, aaaa);
        glUniform1i(tex1,aaaa);
//        glm::mat4 view;
//        glm::mat4 projection;
//        glm::mat4 model;
//        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
//        model = glm::translate(model, glm::vec3(0,0,0));
//        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0,1,0));
//        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
//        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//        GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
//        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
//        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, texture1);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, aaaa);
////        glActiveTexture(GL_TEXTURE1);
////        glBindTexture(GL_TEXTURE_2D, bbbb);
        glUniform1i(tex1,0);
//        glUniform1i(tex2,1);
        glBindVertexArray(Rectangle);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

//        glBindFramebuffer(GL_FRAMEBUFFER,fbo1);
//        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUseProgram(shaderProgram);
        GLuint tex3 = glGetUniformLocation(shaderProgram, "ourTexture");
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, bbbb);
        glUniform1i(tex3,2);
        glBindVertexArray(Rectangle);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);
        GLuint tex4 = glGetUniformLocation(shaderProgram, "ourTexture");
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(tex4,3);
        glBindVertexArray(Rectangle);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        
//        glClearColor(0.2f, 0.6f, 0.8f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glUseProgram(shaderProgram);
//////        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
////        glBindVertexArray(0);
//        GLuint text1 = glGetUniformLocation(shaderProgram, "ourTexture");
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, texture1);
//        glUniform1i(text1, texture1);
//        glBindVertexArray(Rectangle ;
//        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);

//        glUseProgram(shaderProgram2);
//        glm::mat4 view;
//        glm::mat4 projection;
//        glm::mat4 model;
//        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
//        model = glm::translate(model, glm::vec3(0,0,0));
//        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0,1,0));
//        GLuint viewLoc = glGetUniformLocation(shaderProgram2, "view");
//        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//        GLuint projectionLoc = glGetUniformLocation(shaderProgram2, "projection");
//        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
//        GLuint modelLoc = glGetUniformLocation(shaderProgram2, "model");
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//        glBindVertexArray(cube);
//        glDrawElements(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_INT, 0);
//
//        glBindVertexArray(0);
        
        

        glfwSwapBuffers(win);
    }

    glfwTerminate();

    return 0;
}


