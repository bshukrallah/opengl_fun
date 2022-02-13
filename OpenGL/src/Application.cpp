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


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};


static ShaderProgramSource ParseShader(const std::string& filePath) {
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#Vertex") != std::string::npos)
            type = ShaderType::VERTEX;
        else if (line.find("#Fragment") != std::string::npos)
            type = ShaderType::FRAGMENT;
        else ss[(int)type] << line << "\n";
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    //Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader\n";
        std::cout << message << "\n";
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);


    return program;
}

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

    glfwSwapInterval(1);

    glewInit();
    {

        LOG("Debug Logging is True"); //LOG command is set to only work in DEBUG compile
        LOG(glGetString(GL_VERSION)); //LOG OpenGL Version to console
        float positions[] = {
       -0.5f, -0.5f,
        0.5f,  0.5f,
        0.5f, -0.5f,
       -0.5f,  0.5f
        };

        unsigned int indices[] = {
            0,1,2,
            0,3,1
        };



        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);




        IndexBuffer ib(indices, sizeof(indices));

        ShaderProgramSource shaderSource = ParseShader("resources/shaders/Basic.Shader");

        std::string vertexShader = shaderSource.VertexSource;
        std::string fragmentShader = shaderSource.FragmentSource;

        unsigned int shader = CreateShader(vertexShader, fragmentShader);

        glUseProgram(shader);

        int location = glGetUniformLocation(shader, "u_Color");

        ASSERT(location != -1);
        //glUniform4f(location, 0.0f, 0.7f, 0.8f, 1.0f);

        float b = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shader);
            GLCall(glUniform4f(location, 0.0f, b, 1.0f, 1.0f));
            va.Bind();
            ib.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //Triangle, number of indices, data type, nothing...)


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

        glDeleteProgram(shader);
    }

    glfwTerminate();
    return 0;
}
