#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "headers/shader.h"
#include <stb/stb_image.h>
#include<chrono>
#include<queue>

#include"headers/constants.h"
#include "headers/transform.h"
#include "headers/camera.h"
#include "headers/chunk.h"
#include "headers/chunkManager.h"

using namespace std::chrono;

struct Light: public Transform {
    float ambient;
    glm::vec3 color;
    glm::vec3 pos;
    Light(float ambient, glm::vec3 color, glm::vec3 pos) {
        this->ambient = ambient;
        this->color = color;
        this->pos = pos;
    }
};


#define ASSERT(x) if(!(x)) __debugbreak();
void GLAPIENTRY
MessageCallback(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

void processInput(GLFWwindow* window, Camera* camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->keyboardInput("f", 1);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->keyboardInput("b", 1);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->keyboardInput("l", 1);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->keyboardInput("r", 1);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->keyboardInput("u", 1);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->keyboardInput("d", 1);
}

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        std::cout << "glfw init error" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    //flips the texture images
    stbi_set_flip_vertically_on_load(true);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPosCallback(window, mouse_callback);

    //glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "glew error" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_MULTISAMPLE);

    //glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(MessageCallback, 0);

    //Chunk chunk = Chunk(data, glm::vec3(0.0f, 0.0f, 0.0f));

    Camera camera = Camera(glm::vec3(0.0f,300.0f, 10.0f), 1, .1, 1,45.0f,1.f,0.1f,100.f);
    glfwGetCursorPos(window, &camera.mouse.x, &camera.mouse.y);

    Light light = Light(.1, glm::vec3(1.0f,1.0f,1.0f), glm::vec3(50, 500, 20));

    Shader shader = Shader("res/shaders/vert3d.glsl", "res/shaders/frag3d.glsl");
    shader.use();



    unsigned int texture0;
    glGenTextures(1, &texture0);
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture

    glBindTexture(GL_TEXTURE_2D, texture0);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int tWidth, tHeight, nrChannels;
    unsigned char* datai = stbi_load("res/textures/dirt.png", &tWidth, &tHeight, &nrChannels, 0);
    if (datai) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tWidth, tHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, datai);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(datai);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 16/9.0f, 0.1f, 1000.0f);


    unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
    unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
    unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    float normals[] = {
        0.0f,0.0f,1.0f,
        0.0f,-1.0f,0.0f,
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        -1.0f,0.0f,0.0f,
        0.0f,0.0f,-1.0f
    };
    glUniform1fv(glGetUniformLocation(shader.ID, "normals"), 18, normals);

    ChunkManager chunkManger = ChunkManager(modelLoc, shader.ID, &camera);

    shader.use();
    shader.setVec3("lightColor", light.color);
    shader.setVec3("lightPos",light.pos);

    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, texture0);
   
    while (!glfwWindowShouldClose(window))
    {
        auto start = high_resolution_clock::now();
        
        while(!chunkManger.chunksAwaitingGpu.empty()) {
            chunkManger.chunksAwaitingGpu.front()->sendData();
            chunkManger.chunksAwaitingGpu.pop();

            if (duration_cast<milliseconds>(high_resolution_clock::now() - start).count() > 5) break;
        }

        shader.use();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        camera.mouseInput(mx,my);

        shader.setVec3("viewPos", camera.pos);


        glm::mat4 lookAt = camera.getViewMat();
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(lookAt));
        chunkManger.renderChunks(shader);

        processInput(window, &camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader.ID);
    glfwTerminate();

    return 0;
}