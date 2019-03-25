#version 450

in vec3 a_Position;
in vec3 a_Vel;

uniform float u_Time;
void main()
{
	vec3 newPot = a_Position.xyz;
	
	float newTime = fract(u_Time);			// 0 ~ 1 Repeat

	newPot.x += (newTime * a_Vel.x);
	newPot.y += (newTime * a_Vel.y);

	gl_Position   = vec4(newPot.xyz, 1);
}