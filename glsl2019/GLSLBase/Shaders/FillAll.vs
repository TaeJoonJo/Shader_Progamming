#version 450

layout(location = 0) in vec4 a_Position;

void main()
{
	gl_Position   = //newPosition;
					vec4(a_Position.xyz, 1);
}