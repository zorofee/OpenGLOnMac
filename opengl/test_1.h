
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "common/shader_m.h"
#include "common/camera.h"
#include "common/model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "common/stb_image.h"

unsigned int loadTexture(char const * path);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aUV;\n"
    "out vec2 v_uv;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x , aPos.y , aPos.z, 1.0);\n"
    "   v_uv = aUV;\n"
    "}\0";
const char *fragmentShaderSource1 = "#version 330 core\n"
    "in vec2 v_uv;\n"
    "uniform sampler2D texture1;\n"
    "out vec4 FragColor;\n"

    "void main()\n"
    "{\n"
    "   vec3 col = texture(texture1,v_uv).rgb;\n"
    "   FragColor = vec4(col, 1.0);\n"
    "}\n\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
    "in vec2 v_uv;\n"
    "uniform sampler2D texture1;\n"
    "out vec4 FragColor;\n"

    "void main()\n"
    "{\n"
    "   FragColor = vec4(v_uv,0.0,1.0);\n"
    "}\n\0";

int _main(){

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"Learn",NULL,NULL);

    glfwMakeContextCurrent(window);


    GLuint program1 = glCreateProgram();
    GLuint program2 = glCreateProgram();

    {
        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertShader,1,&vertexShaderSource,NULL);
        glShaderSource(fragShader,1,&fragmentShaderSource1,NULL);

        glCompileShader(vertShader);
        glCompileShader(fragShader);


        //必须要先attach shader，再link program
        glAttachShader(program1,vertShader);
        glAttachShader(program1,fragShader);
        glLinkProgram(program1);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
    }

    {
        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertShader,1,&vertexShaderSource,NULL);
        glShaderSource(fragShader,1,&fragmentShaderSource2,NULL);

        glCompileShader(vertShader);
        glCompileShader(fragShader);


        //必须要先attach shader，再link program
        glAttachShader(program2,vertShader);
        glAttachShader(program2,fragShader);
        glLinkProgram(program2);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
    }



    float vertData[] = {
        -1.0,-1.0,0.0, 0.0,1.0,
        -1.0,1.0,0.0,  0.0,0.0,
        1.0,1.0,0.0,   1.0,0.0,

        -1.0,-1.0,0.0, 0.0,1.0,
        1.0,-1.0,0.0,  1.0,1.0,
        1.0,1.0,0.0,   1.0,0.0  
    };

    GLuint VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertData),vertData,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    unsigned int cubeTexture = loadTexture("resources/textures/awesomeface.png");
    

    /////////////////////////////frame buffer////////////////////
    GLuint fbo;
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH * 2, SCR_HEIGHT * 2, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    while(!glfwWindowShouldClose(window)){
        glBindFramebuffer(GL_FRAMEBUFFER,fbo);
        glClearColor(1.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program2);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,6);
        
//////////////////////////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClearColor(1.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(program1);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureColorbuffer);
        
        glDrawArrays(GL_TRIANGLES,0,6);


        glBindFramebuffer(GL_FRAMEBUFFER,0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }


/*
//颜色渲染在图片上
    while(!glfwWindowShouldClose(window)){

        glClearColor(0.1,0.1,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program1);
        glBindVertexArray(VAO);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        
        glDrawArrays(GL_TRIANGLES,0,6);


        //glClearColor(1.0,0.0,0.0,1.0);
        //glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program2);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    */
}



// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}