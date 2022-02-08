#include <iostream>
#include <GL\glew.h>
#include <glfw3.h>


#if DEV_MODE_LOGGING == true
#define LOG(x) std::cout << x << std::endl
#elif DEV_MODE_LOGGING == false
#define LOG(x)
#endif


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    float positions[6] = {
       -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f,  0.0f

    };

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glewInit();
    LOG("Debug Logging is True");
    LOG(glGetString(GL_VERSION));
    //Create a buffer
    unsigned int buffer;
    //We need 1 buffer, and assign a pointer to the unsigned int above
    glGenBuffers(1, &buffer);
    //Tell opengl what to do with the buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
