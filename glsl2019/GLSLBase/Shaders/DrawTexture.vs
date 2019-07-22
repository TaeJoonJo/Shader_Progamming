#version 450

in vec4 a_Position;
in vec2 a_UV;

out vec2 v_UV;

uniform float u_SizeX;
uniform float u_SizeY;
uniform float u_X;
uniform float u_Y;

void main()
{
	vec4 newPos = vec4((a_Position.x * u_SizeX) + u_X, (a_Position.y * u_SizeY) + u_Y, a_Position.z, 1);

	gl_Position   = newPos;
					//vec4(a_Position.xyz, 1);

	v_UV = a_UV;
}