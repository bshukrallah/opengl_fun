#include <iostream>
#include <GL/glew.h>
#include "Triforce.h"
#include "../Shader.h"
#include "../vendor/imgui/imgui.h"
#include "../Renderer.h"
#include "../vendor/glm/gtc/matrix_transform.hpp"

namespace test {


	Triforce::Triforce()
		: m_TranslationA{ -50, -40, 0 }, m_TranslationB{ 130, -40, 0 }, m_Proj(glm::ortho(-200.0f, 200.0f, -150.0f, 150.0f, -1.0f, 1.0f)), m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-80, 0, 0)))
	
	{

		GLfloat positions[] = //x, y, z ... RGB
		{
			-0.5f,    -0.5f * float(sqrt(3)) / 3,     0.0f, 0.6f, 0.3f, 0.6f, //Lower left corner
			 0.5f,    -0.5f * float(sqrt(3)) / 3,     0.0f, 0.0f, 0.3f, 0.7f, //Lower right corner
			 0.0f,     0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 0.0f, 0.2f, 1.0f, //Upper corner
			-0.5f / 2, 0.5f * float(sqrt(3)) / 6,     0.0f, 0.9f, 0.45f, 0.87f, //Inner left
			 0.5f / 2, 0.5f * float(sqrt(3)) / 6,     0.0f, 0.8f, 0.6f, 0.17f, //Inner right
			 0.0f,    -0.5f * float(sqrt(3)) / 3,     0.0f, 0.4f, 0.6f, 0.0f  //Inner down
		};

		GLuint indices[] =
		{
			0, 3, 5, //Lower left triangle
			3, 2, 4, //Lower right triangle
			5, 4, 1  //Upper triangle
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices));


		m_Shader = std::make_unique<Shader>("resources/shaders/Triforce.shader");
		m_Shader->Bind();

	}


	Triforce::~Triforce() {}

	void Triforce::OnUpdate(float deltatime)
	{
	}

	void Triforce::OnRender()
	{
		Renderer renderer;

		{

			m_Shader->Bind();
			//m_Shader->SetUniformMat4f("u_MVP", mvp);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
		{

			m_Shader->Bind();
			//m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void Triforce::OnImGuiRender()
	{
		//ImGui::Begin("Control Panel");
		static bool show_app_metrics = true;
		ImGui::Text("A Triforce");

		ImGui::SliderFloat2("Object1", &m_TranslationA.x, -100.0f, 200.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::End();

	}


}
