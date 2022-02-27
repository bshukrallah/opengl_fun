#include <GL/glew.h>
#include "../vendor/imgui/imgui.h"
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

#include "TestTexture2D.h"
#include "../Renderer.h"

namespace test {

	TestTexture2D::TestTexture2D()
		: m_TranslationA{ -50, -40, 0 }, m_TranslationB{ 130, -40, 0 }, m_Proj(glm::ortho(-200.0f, 200.0f, -150.0f, 150.0f, -1.0f, 1.0f)), m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-80, 0, 0)))

	{
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

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions));

				VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices));


		//Change Shader to Basic.Shader or image_texture.shader to see the difference
		m_Shader = std::make_unique<Shader>("resources/shaders/image_texture.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.0f, 0.7f, 0.8f, 1.0f);


		m_Texture = std::make_unique<Texture>("resources/textures/smile.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltatime)
	{
	}

	void TestTexture2D::OnRender()
	{
		Renderer renderer;
		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		//ImGui::Begin("Control Panel");
		static bool show_app_metrics = true;
		ImGui::Text("Move the smiley face around...");               // 
		ImGui::SliderFloat2("Object1", &m_TranslationA.x, -100.0f, 200.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat2("Object2", &m_TranslationB.x, -100.0f, 200.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::End();

	}

}