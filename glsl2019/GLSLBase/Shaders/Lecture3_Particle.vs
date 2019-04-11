#version 450

in vec3 a_Position;
in vec3 a_Vel;

in vec2 a_StartLife;

uniform float u_Time;
uniform bool u_Repeat = true;

const vec3 c_Gravity = vec3(0.f, -5.f, 0.f);

void main()
{
	vec3 newPot = a_Position.xyz;

	float startTime = a_StartLife.x;
	float lifeTime = a_StartLife.y;

	float newTime = u_Time - startTime;	
	
	if(newTime >= 0) 
	{
		float life = newTime;

		float remainingLife = lifeTime - life;

		if(u_Repeat == true) {
			remainingLife = 1.f;
			newTime = mod(newTime, lifeTime);
		}

		if(remainingLife > 0)
			newPot += (newTime * a_Vel) + (0.5 * c_Gravity * newTime * newTime);
		else
			newPot = vec3(10000.f, 10000.f, 10000.f);
	}
	else
		newPot = vec3(10000.f, 10000.f, 10000.f);

	gl_Position = vec4(newPot.xyz, 1);
}




//void main()
//{
//	vec3 newPot = a_Position.xyz;
//
//	float startTime = a_StartLife.x;
//	float lifeTime = a_StartLife.y;
//
//	float newTime = u_Time - startTime;	
//	
//	if(newTime >= 0) 
//	{
//		float life = newTime;
//
//		float remainingLife = lifeTime - life;
//
//		if(u_Repeat == true) {
//			remainingLife = 1.f;
//			newTime = mod(newTime, lifeTime);
//		}
//
//		if(remainingLife > 0)
//			newPot += (newTime * a_Vel) + (0.5 * c_Gravity * newTime * newTime);
//		else
//			newPot = vec3(10000,10000,10000);
//	}
//	else
//		newPot = vec3(10000,10000,10000);
//
//	gl_Position = vec4(newPot.xyz, 1);
//}