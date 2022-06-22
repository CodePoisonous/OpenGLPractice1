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
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
			-0.5f, -0.5f,	// 0
			 0.5f, -0.5f,	// 1 
			 0.5f,  0.5f,	// 2
			-0.5f,  0.5f,	// 3 
		};

		// 图形端点的索引序号
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
			
		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);			// positions 元素类型为 float, 坐标为xy两个元素一组
		va.AddBuffer(vb, layout);
		
		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;
		float r = 0.0f;
		float increment = 0.05f;
		// 循环直到用户关闭窗口
		while (!glfwWindowShouldClose(window))
		{
			// 渲染
			renderer.Clear();
			// GLCall(glClear(GL_COLOR_BUFFER_BIT));

			shader.Bind();			
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);	

			renderer.Draw(va, ib, shader);
			// va.Bind();
			// ib.Bind();
			// GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			// 交换前后缓冲区
			glfwSwapBuffers(window);

			// 轮询和处理事件
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}