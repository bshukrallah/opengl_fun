#pragma once
#include "Test.h"
#include "../VertexBufferLayout.h"
#include "../VertexArray.h"
#include <memory>



namespace test {

	class Triforce : public Test
	{
	public:
		Triforce();
		~Triforce();

		void OnUpdate(float deltatime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		glm::vec3 m_TranslationA;
		glm::mat4 m_Proj, m_View;

	};
}