#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
in vec2 v_OrignXY;

in float v_Rad;
void main()
{
	vec4 newColor = v_Color;
	float dis = sqrt(v_OrignXY.x * v_OrignXY.x + v_OrignXY.y * v_OrignXY.y);
	if( dis < v_Rad ) {
		newColor.a = dis / v_Rad;
	}
	else
		newColor.a = 0.f;

	FragColor = newColor;
}