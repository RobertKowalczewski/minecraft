#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include"shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();
void GLAPIENTRY
MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void* userParam);

void setUniforms(Shader* shader, float left, float top, float xside, float yside) {
    shader->setFloat("left", left);
    shader->setFloat("top", top);
    shader->setFloat("xside", xside);
    shader->setFloat("yside", yside);
}
void drawMandel(GLFWwindow* window,unsigned int &shaderID, unsigned int &vao, unsigned int &ebo) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderID);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
}
void processInput(GLFWwindow* window, float& left, float& top, float& xside, float& yside,float& scale,
    unsigned int& shaderID, unsigned int& vao, unsigned int& ebo, Shader* shader) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        top += .01f / scale;

        drawMandel(window, shaderID, vao, ebo);
        setUniforms(shader, left, top, xside, yside);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        top -= .01f / scale;

        drawMandel(window, shaderID, vao, ebo);
        setUniforms(shader, left, top, xside, yside);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        left -= .01f / scale;

        drawMandel(window, shaderID, vao, ebo);
        setUniforms(shader, left, top, xside, yside);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        left += .01f / scale;

        drawMandel(window, shaderID, vao, ebo);
        setUniforms(shader, left, top, xside, yside);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        float change = .01f;
        scale *= 1 - change;

        float zoomPointX = (left + xside/2);
        float zoomPointY = (top + yside/2);
        float offsetX = -(zoomPointX * change);
        float offsetY = -(zoomPointY * change);

        //left -= offsetX;
        xside *= change + 1;
        //top -= offsetY;
        yside *= change + 1;


        drawMandel(window, shaderID, vao, ebo);
        setUniforms(shader, left, top, xside, yside);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        float change = -.01;
        scale *= 1 - change;

        float zoomPointX = (left + xside) / 2;
        float zoomPointY = (top + yside) / 2;
        float offsetX = -(zoomPointX * change);
        float offsetY = -(zoomPointY * change);

        //left -= offsetX;
        xside *= change + 1;
        //top += offsetY;
        yside *= change + 1;

        drawMandel(window, shaderID, vao, ebo);
        setUniforms(shader, left, top, xside, yside);
    }
}

int main2() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "glew error" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;


    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    float vertices[] = {
        // positions          // texture coords
        1.f, 1.f, 0.0f,    1.0f, 1.0f,   // top right
        1.f, -1.f, 0.0f,    1.0f, 0.0f,   // bottom right
        -1.f, -1.f, 0.0f,   0.0f, 0.0f,   // bottom left
        -1.f,  1.f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int elements[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    float left = -1.7f;
    float top = -0.25f;
    float xside = 1.f;
    float yside = 1.f;
    float scale = 1.f;

    Shader shader("res/shaders/vert_mandel.shader", "res/shaders/frag_mandel.shader");
    shader.use();
    shader.setInt("res_x", 800);
    shader.setInt("res_y", 800);
    shader.setInt("iterations", 255);



    setUniforms(&shader, left, top, xside, yside);


    //create vertex array object
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    //generate buffer, writes id into passed integer
    glGenBuffers(1, &vbo);
    //bind = select, argument is specyfing what purpose of this buffer is
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //copying positions into buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //ebo-element buffer object
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    //create vertex attribute in this case position
    //links vertex buffer with vertex attribute object, so later there is
    //no need to bind vertex buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    //enable vertex attrib 0
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    drawMandel(window, shader.ID, vao, ebo);


    while (!glfwWindowShouldClose(window))
    { 
        processInput(window, left, top, xside, yside,scale,shader.ID,vao,ebo, &shader);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader.ID);
    glfwTerminate();
    return 0;
}