#include "Test.h"

#include "imgui/imgui.h"

namespace test {

	TestMenu::TestMenu(Test*& CurrentTestPointer)
		: m_CurrentTest(CurrentTestPointer)
	{

	}

	TestMenu::~TestMenu()
	{

	}

	void TestMenu::OnImGuiRender()
	{
		 for (auto& test_it : m_Tests)
		 {
			 if (ImGui::Button(test_it.first.c_str()))
				 m_CurrentTest = test_it.second();
		 }
	}

}