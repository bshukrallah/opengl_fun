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

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

void updateshader(std::string& s_path) {

    if (s_path == "resources/shaders/image_texture.shader") {
        s_path = "resources/shaders/Basic.shader";
        std::cout << "Flipped to Basic Shader\n";
    }
    else {
        s_path = "resources/shaders/image_texture.shader";
        std::cout << "Flipped to Texture Shader\n";
    }


    GLCall(Shader shader(s_path));
    GLCall(shader.Bind());
    GLCall(shader.SetUniform4f("u_Color", 0.0f, 0.7f, 0.8f, 1.0f));
    GLCall(Texture texture("resources/textures/smile.png"));
    GLCall(texture.Bind());
    GLCall(shader.SetUniform1i("u_Texture", 0));
    GLCall(shader.Unbind());


};
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
    window = glfwCreateWindow(640*2, 480*2, "Open GL Window", NULL, NULL);
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
       -75.0f, -75.0f, 0.0f, 0.0f, // 0
        150.0f, -75.0f, 1.0f, 0.0f, // 1
        150.0f,  150.0f, 1.0f, 1.0f, // 2
       -75.0f,  150.0f, 0.0f, 1.0f  // 3
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
        //Set up 4X3 ratio (left, right, bottom, top, zNear, zFar...
        //if you multiple each of the first 4 numbers, you get 4X3, which is the aspect ratio of our window.
        glm::mat4 proj = glm::ortho(-200.0f, 200.0f, -150.0f, 150.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-80, 0, 0));


        //Change Shader to Basic.Shader or image_texture.shader to see the difference

        std::string shader_path = "resources/shaders/image_texture.shader";

        Shader shader(shader_path);
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

        // Setup Dear ImGui context
        const char* glsl_version = "#version 130";
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        glm::vec3 translation(50, -40, 0);

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        float b = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 mvp = proj * view * model;


            shader.Bind();
            shader.SetUniform4f("u_Color", 0.0f, b, 1.0f, 1.0f);
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
            
            if (b > 1.0f)
                increment = -0.05f;
            else if (b < 0.05f)
                increment = 0.02f;

            b += increment;
            ImGui::ShowDemoWindow(&show_demo_window);
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Control Panel");

                ImGui::Text("Move the smiley face around...");               // 
                ImGui::SliderFloat2("Translation", &translation.x, -100.0f, 200.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

                //if (ImGui::Button("Swap Texture"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                   //updateshader(shader_path);


                  
                //ImGui::SameLine();


                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
