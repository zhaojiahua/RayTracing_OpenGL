#include "Globals.h"

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* mainWind = glfwCreateWindow(screenWidth, screenHeigh, "RayTracingRender", nullptr, nullptr);
	glfwMakeContextCurrent(mainWind);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to Initialize GLAD" << endl;
		return -1;
	}

	//设置按键响应回调函数
	glfwSetKeyCallback(mainWind,ZJH_KeyCallBack);

	//创建Mesh,Shader和Render,还有个Camera然后就可渲染了
	GLfloat vertices[] = {
			// pos	  normal		uv
		-1.5f,1.0f, -1.0f,	0.0f,0.0f,1.0f,	0.0f,1.0f,
		 1.5f,1.0f,  -1.0f,	0.0f,0.0f,1.0f,	1.0f,1.0f,
		-1.5f,-1.0f,-1.0f,	0.0f,0.0f,1.0f,	0.0f,0.0f,
		 1.5f,-1.0f, -1.0f,	0.0f,0.0f,1.0f,	1.0f,0.0f
	};
	GLuint indices[] = {
		2,1,0,	3,1,2
	};

	Mesh* mesh1 = new Mesh(vertices, sizeof(vertices), indices, sizeof(indices));
	Shader* shader1 = new Shader("shaders/vertexshader.c","shaders/fragmentshader.c");
	Render* render1 = new Render();
	Camera* camera1 = new Camera(0.1f, 100.0f, (GLfloat)(static_cast<GLfloat>(screenWidth) / static_cast<GLfloat>(screenHeigh)), 45.0f);

	//生成100个2维随机数(用于反走样)
	glm::vec2 randoffsets[100];
	for (int i = 0; i < 100; i++) {
		randoffsets[i] = glm::vec2(Random_double(0.0, 1.0), Random_double(0.0, 1.0));
	}

	glViewport(0, 0, viewWidht, viewHeight);
	glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
	while (!glfwWindowShouldClose(mainWind)) {
		glfwPollEvents();//只处理已经在消息队列中的消息并立即返回结果
		
		glClear(GL_COLOR_BUFFER_BIT);
		shader1->Use();
		shader1->SetMat4Uniform("p_matrix", camera1->mPerspectiveMatrix);
		shader1->SetMat4Uniform("v_matrix", camera1->mViewMatrix);
		shader1->SetVec2Uniform("screenResoution", glm::vec2(screenWidth, screenHeigh));
		shader1->SetVec2Uniform("nearPlane", glm::vec2(camera1->mNearWidth, camera1->mNearHeight));
		shader1->SetFloatUniform("near", camera1->mNear);
		for (int i = 0; i < 100; i++) {
			shader1->SetVec2Uniform("sampleoffsets[" + to_string(i) + "]", randoffsets[i]);
		}
		render1->Draw(mesh1);

		glfwSwapBuffers(mainWind);//交换缓冲(这个很重要,不然windows不知道要用这个缓冲来覆盖窗口)
	}

	delete(mesh1);
	delete(shader1);
	glfwTerminate();
	return 0;
}