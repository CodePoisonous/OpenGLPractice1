#include "TestBatchRender.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {


	TestBatchRender::TestBatchRender()
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Translation(200, 200, 0)
	{
		// 图形端点位置xy坐标
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,	// 0
			 50.0f, -50.0f, 1.0f, 0.0f,	// 1 
			 50.0f,  50.0f, 1.0f, 1.0f,	// 2
			-50.0f,  50.0f, 0.0f, 1.0f,	// 3 

			 50.0f,  50.0f, 0.0f, 0.0f,
			150.0f,  50.0f, 1.0f, 0.0f,
			150.0f, 150.0f, 1.0f, 1.0f,
			 50.0f, 150.0f, 0.0f, 1.0f
		};

		// 图形端点的索引序号
		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		// 混合
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);			// positions 元素类型为 float, 坐标为xy两个元素一组
		layout.Push<float>(2);			// texture
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();

		m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestBatchRender::~TestBatchRender()
	{

	}

	void TestBatchRender::OnUpdate(float deltaTime)
	{

	}

	void TestBatchRender::OnRender()
	{ 
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void TestBatchRender::OnImGuiRender()
	{
		ImGui::SliderFloat3("translation", &m_Translation.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}