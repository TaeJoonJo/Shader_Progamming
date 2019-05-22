#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

in vec2 v_Tex;
in float v_Grey;

void main()
{
	vec2 newTex = v_Tex;
	newTex.y = 1.f - newTex.y;
	//vec4 newColor;
	//newColor = vec4(v_Grey);
	//FragColor = newColor;
	vec4 newColor = texture(u_Texture, newTex) * v_Grey;

	FragColor = newColor;
}