#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL\glew.h>
#include <glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "texture.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Open GL Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //V-SYNC

    glewInit();
    {
        LOG("Debug Logging is True"); //LOG command is set to only work in DEBUG compile
        LOG(glGetString(GL_VERSION)); //LOG OpenGL Version to console
        float positions[] = {
        // X   Y     TEXTURE(X, Y)
       -0.5f, -0.5f, 0.0f, 0.0f, // 0
        0.5f, -0.5f, 1.0f, 0.0f, // 1
        0.5f,  0.5f, 1.0f, 1.0f, // 2
       -0.5f,  0.5f, 0.0f, 1.0f  // 3
        };

        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };


        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, sizeof(indices));


        //Change Shader to Basic.Shader or image_texture.shader to see the difference
        Shader shader("resources/shaders/image_texture.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.0f, 0.7f, 0.8f, 1.0f);

        Texture texture("resources/textures/smile.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);


        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float b = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", 0.0f, b, 1.0f, 1.0f);

            renderer.Draw(va, ib, shader);
            
            if (b > 1.0f)
                increment = -0.05f;
            else if (b < 0.05f)
                increment = 0.02f;

            b += increment;
            
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
