#version 450

layout(location = 0) in vec4 a_Position;
in vec2 a_UV;

out vec4 v_Pos;
out vec2 v_UV;

void main()
{
	gl_Position = a_Position;

	//v_Pos = a_Position;
	//v_Pos = vec4(a_UV.xy, 0, 1);
	v_Pos = vec4(a_Position.xy + vec2(.5, .5), 0, 1);
	v_UV = a_UV;
}