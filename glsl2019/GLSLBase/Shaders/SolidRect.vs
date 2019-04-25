#version 450

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;

uniform float u_Time;							// 1.f

void main()
{
	float Pi   = 3.141592;
	float fRad = u_Time * Pi * 2;
	
	vec4 newPosition;
	newPosition.x = ((cos(fRad) * 0.8f) + a_Position.x);
	newPosition.y = ((sin(fRad) * 0.8f) + a_Position.y);
	newPosition.z = a_Position.z;
	newPosition.w = 1.f;

	gl_Position   = newPosition;
					//vec4(a_Position.xyz, 1);
}
