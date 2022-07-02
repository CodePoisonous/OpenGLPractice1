#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

int main(void)
{
	GLFWwindow* window;

	// 初始化glfw
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口模式窗口及其 OpenGL 上下文
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// 使window成为当前的上下文
	glfwMakeContextCurrent(window);

	// 设置刷新率为60帧
	glfwSwapInterval(1);

	// 判断glew初始化是否成功
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;
	
	// 获取gl版本号
	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		// 图形端点位置xy坐标
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,	// 0
			 50.0f, -50.0f, 1.0f, 0.0f,	// 1 
			 50.0f,  50.0f, 1.0f, 1.0f,	// 2
			-50.0f,  50.0f, 0.0f, 1.0f	// 3 
		};

		// 图形端点的索引序号
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
				
		GLCall(glEnable(GL_BLEND));									// 启用混合，根据src和destination的alpha值设置混合后的RGBA值
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));	// 计算方法：dest RGBA = src RGBA * src A + dest RGBA * (1 - src A)

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);			// positions 元素类型为 float, 坐标为xy两个元素一组
		layout.Push<float>(2);			// texture
		va.AddBuffer(vb, layout);
		
		IndexBuffer ib(indices, 6);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);		// orthographic matrix 正交矩阵
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		Texture texture("res/textures/ChernoLogo.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		// Setup ImGui binding
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);

		// Setup style
		ImGui::StyleColorsDark();

		// 循环直到用户关闭窗口
		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);
		while (!glfwWindowShouldClose(window))
		{
			// 渲染
			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();
			
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);

			}

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				//renderer.Draw(va, ib, shader);
				shader.SetUniformMat4f("u_MVP", mvp);
			}

			renderer.Draw(va, ib, shader);

			// Show a simple window.
			{
				ImGui::SliderFloat3("translation A", &translationA.x, 0.0f, 960.0f);
				ImGui::SliderFloat3("translation B", &translationB.x, 0.0f, 960.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


			// 交换前后缓冲区
			glfwSwapBuffers(window);

			// 轮询和处理事件
			glfwPollEvents();
		}
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}