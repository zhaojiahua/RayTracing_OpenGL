#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

out vec2 fragUV;
out vec3 fragNr;
out vec3 fragPos;

uniform mat4 p_matrix;
uniform mat4 v_matrix;

void main()
{
	fragPos = aPos;
	gl_Position = vec4(aPos, 1.0);
	fragUV = aUV;
	fragNr = aNormal;
}
