#version 450
// Lecture6

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

uniform float u_SizeX;
uniform float u_SizeY;
uniform float u_X;
uniform float u_Y;

uniform int u_Number;		// idigit
in vec2 v_UV;

void main()
{	
	vec2 newUV = v_UV;
	//newUV.y = 1.f - newUV.y;

	vec4 newColor = texture(u_Texture, newUV);

	FragColor = newColor;
}