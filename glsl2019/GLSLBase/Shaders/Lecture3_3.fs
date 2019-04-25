#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

in vec4 v_Color;
in vec2 v_OrignXY;

in float v_Rad;
void main()
{
	vec4 newColor = v_Color;
	float dis = sqrt(v_OrignXY.x * v_OrignXY.x + v_OrignXY.y * v_OrignXY.y);
	if( dis < v_Rad ) {
		newColor.a = pow( 1 - dis / v_Rad, 7);
	}
	else
		newColor.a = 0.f;
	
	FragColor = newColor;

	//FragColor = texture(u_Texture, v_OrignXY);
}