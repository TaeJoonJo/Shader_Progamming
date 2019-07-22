#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

uniform float u_Number;		// idigit
uniform float u_ResolX;
uniform float u_ResolY;
in vec2 v_UV;

// 
void main()
{	
	vec2 newUV = v_UV;
	newUV.y = 1.f - newUV.y;

	int x_index = int(floor(fract((u_Number) / (u_ResolX)) * u_ResolX));
	int y_index = int(floor(float(u_Number) / (u_ResolY)));

	newUV.x = (newUV.x + x_index) / (u_ResolX);
	newUV.y = (newUV.y + y_index) / (u_ResolY);
	//newUV.x = u_ResolX - newUV.x;

	vec4 newColor;

	newColor = texture(u_Texture, newUV);
	FragColor = newColor;
}
