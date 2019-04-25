#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;
uniform float u_Time;
in vec2 v_UV;
void main()
{	
	vec2 newUV = v_UV;
	newUV.y *= -1.f;

	vec4 newColor = vec4(0);

	vec4 newColor2 = texture(u_Texture, newUV);
	//newColor2.y = 1.f - newColor2.y;
	//
	//if(newColor.a < 0.05f)
	//	discard;

	//FragColor = newColor;

	float newTime = fract(u_Time);
	float x = v_UV.x * 2 * 3.141592;
	float sinResult = ((sin(x + u_Time) + 1) / 2);// * (v_UV.x * 10);
	float Thick = 0.3 * v_UV.x;

	if(v_UV.y - Thick < sinResult &&  v_UV.y > sinResult)
		newColor = vec4(1);

	vec4 newColor3 = newColor * newColor2;

	FragColor = newColor2;
}