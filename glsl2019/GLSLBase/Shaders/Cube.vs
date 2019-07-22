#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;

uniform float u_Time;							// 1.f

const float PI = 3.141592;

uniform mat4 u_ProjView;
out vec4 v_Color;

void main()
{	
	vec4 newPos = vec4(a_Position, 1);

	gl_Position = u_ProjView * newPos;
	v_Color = a_Color;
}