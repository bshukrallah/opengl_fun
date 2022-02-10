#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL\glew.h>
#include <glfw3.h>


#if DEV_MODE_LOGGING == true
#define LOG(x) std::cout << x << std::endl
#elif DEV_MODE_LOGGING == false
#define LOG(x)
#endif

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
    LOG(vertexShader);
    LOG(fragmentShader);
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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Open GL Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

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
    //indices are fed into the GPU via the indexBuffer...
    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    ShaderProgramSource shaderSource = ParseShader("resources/shaders/Basic.Shader");
    LOG("VERTEX SHADER");
    LOG(shaderSource.VertexSource);
    LOG("FRAGMENT SHADER");
    LOG(shaderSource.FragmentSource);

    std::string vertexShader = shaderSource.VertexSource;
    std::string fragmentShader = shaderSource.FragmentSource;

    unsigned int shader = CreateShader(vertexShader, fragmentShader);

    glUseProgram(shader);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //Use glDrawArrays when specifically drawing from an array (ex: float positions, above)
        //In our example, we are drawing from the indices, so we use glDrawElements

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //Triangle, number of indices, data type, nothing...)


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
