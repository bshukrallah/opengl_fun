#include "TestClearColor.h"
#include <GL/glew.h>
#include "../vendor/imgui/imgui.h"


namespace test {

	TestClearColor::TestClearColor()
		: m_ClearColor {0.78f, 0.86f, 0.99f, 1.0f}
	{
	
	}

	TestClearColor::~TestClearColor()
	{
	
	}

	void TestClearColor::OnUpdate(float deltatime)
	{
	
	}

	void TestClearColor::OnRender()
	{
		glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void TestClearColor::OnImGuiRender()
	{
		//ImGui::Begin("Adjust Background Color");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//ImGui::Text("Adjust the background color");
		ImGui::ColorEdit4("Background Color", m_ClearColor);
		//ImGui::End();
	}

}