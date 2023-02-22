#pragma once
#include "Mesh.h"
#include "Shader.h"

//给定一个Mesh和一个Shader,向屏幕渲染画面
class Render
{
	~Render();
public:
	Render();
	//渲染函数
	void Draw(Mesh* inmesh);
};

