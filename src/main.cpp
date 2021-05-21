#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>

#if 1
#include "Graphics/Shader.h"
#include "Core/Logger.h"

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    FQW::Logger::Initialize();

    GLFWwindow* window;
    GLuint vertex_buffer;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    OPENGL_CALL( glGenBuffers(1, &vertex_buffer) );
    OPENGL_CALL( glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer) );
    OPENGL_CALL( glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW) );
    
    FQW::Shader shader("res/shaders/test.vs", "res/shaders/test.fs");


    mvp_location = shader.GetUniformLocation("MVP");


    vpos_location = shader.GetAttributeLocation("vPos");
    vcol_location = shader.GetAttributeLocation("vCol");
    
    OPENGL_CALL( glEnableVertexAttribArray(vpos_location) );
    OPENGL_CALL( glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0) );
    OPENGL_CALL( glEnableVertexAttribArray(vcol_location) );
    OPENGL_CALL( glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)(sizeof(float) * 2)) );

    float intensity = 1.0f;
    const float d_intensity = 0.001f;
    bool direction = true; // true = forward, false = backward
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;

        glm::mat4 m(1.0f);
        glm::mat4 p;
        glm::mat4 mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        OPENGL_CALL( glViewport(0, 0, width, height) );
        OPENGL_CALL( glClear(GL_COLOR_BUFFER_BIT) );

        m = glm::rotate(m, (float)glfwGetTime(), glm::vec3(0, 0, -1.0f));
        p = glm::ortho(-ratio, ratio, -1.0f, 1.0f);
        mvp = p * m;

        // calc intensity
        if (intensity <= 0)
        {
            direction = true;
        }
        else if (intensity >= 1.0f)
        {
            direction = false;
        }

        if (direction)
        {
            intensity += d_intensity;
        }
        else
        {
            intensity -= d_intensity;
        }

        shader.Use();
        shader.SetMatrix4fv("MVP", mvp);
        shader.SetFloat("u_intensity", intensity);

        OPENGL_CALL( glDrawArrays(GL_TRIANGLES, 0, 3) );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

#endif

#define TESTWINDOW 0
#if TESTWINDOW

#include "Core/Window.h"
#include "Core/Input.h"
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int main()
{
    using namespace FQW;

    Logger::Initialize(spdlog::level::trace);
    Window w(800, 600, "TestingWindow");
    Input::Initialize(w);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("res/meshes/cube.dae", aiProcess_Triangulate);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        FQW_ERROR("[ASSIMP] {}", importer.GetErrorString());
    }


    FQW_INFO("Start running");
    while (!w.IsClosed())
    {
        w.Clear();

        if (Input::IsKeyPressed(GLFW_KEY_A)) { FQW_TRACE("A"); }
        if (Input::IsKeyPressed(GLFW_KEY_W)) { FQW_TRACE("W"); }
        if (Input::IsKeyPressed(GLFW_KEY_D)) { FQW_TRACE("D"); }
        if (Input::IsKeyPressed(GLFW_KEY_S)) { FQW_TRACE("S"); }

        if (Input::IsKeyPressed(GLFW_KEY_SPACE)) {
            double time_s = Input::GetTime_s();
            double time_ms = Input::GetTime_ms();
            FQW_TRACE("Time: s{}, ms{}", time_s, time_ms);
        }

        if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
        {
            auto xy = Input::GetCursorPosition();
            FQW_TRACE("Left btn pressed {} {}", xy.first, xy.second); 
        }

        w.Update();
    }
    FQW_INFO("End running");
}
#endif