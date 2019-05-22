#version 450

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;

uniform float u_Time;							// 1.f

const float PI = 3.141592;

out vec2 v_Tex;
out float v_Grey;

void main()
{	
	vec4 newPos = a_Position;

	// 0 ~ 1
	float additionalValueX = newPos.x + 0.5f;	// 0 ~ 1
	float additionalValueY = newPos.y + 0.5f;	// 0 ~ 1

	float periodX = 1.f + (1.f - (additionalValueY)) * .5f;
	float periodY = 1.f + (additionalValueX) * 1.f;

   // x :: -0.5 ~ 0.5 -> +0.5 -> 0~1 -> *2 *PI -> 0~2PI
   float valueX = (additionalValueY * 2 * PI * periodX) - u_Time * 4.f;
   float valueY = (additionalValueX * 2 * PI * periodY) - u_Time * 2.f;

   float sinValueX = sin(valueX) * .05f;
   float sinValueY = sin(valueY) * .25f;

   // y scale
   	newPos.y = newPos.y * (((1.f - additionalValueX) * .5f) + .5f);

   // x
   newPos.x = newPos.x + sinValueX * additionalValueX;
   // y
   newPos.y = newPos.y + sinValueY * additionalValueX;

   v_Grey = sinValueY + 0.5f;

   gl_Position = newPos;
   v_Tex = vec2(0.5, 0.5) + a_Position.xy;
}
