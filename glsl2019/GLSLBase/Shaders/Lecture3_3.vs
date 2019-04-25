#version 450

//Attributes
in vec3 a_Position;
in vec4 a_Temp;
in vec3 a_Vel;
in float a_Value;
in vec4 a_Color;

// Varying --> fragment shader input
out vec4 v_Color;
out vec2 v_OrignXY;
out float v_Rad;

uniform float u_UTime;
uniform float u_Time;
uniform bool  u_Repeat = true;

const float PI = 3.141592;

const mat3 c_RP = mat3(  0, 1, 0,
						 1, 0, 0,
						 0, 0, 0  );

const vec3 c_Gravity = vec3(0.f, 0.f, 0.f);

void main()
{
	vec3 newPos = a_Position.xyz;

	vec3 newVel = a_Vel.xyz;

	float startTime = a_Temp.x;
	float lifeTime = a_Temp.y;
	float amp = a_Temp.z;
	float ratio = a_Temp.w;

	float newTime = u_Time - startTime;
	
	vec4 newColor = a_Color;

	if(newTime >= 0) {
		//newPos.x += sin(a_Value * PI * 2);
		//newPos.y += cos(a_Value * PI * 2);

		newTime = mod(newTime, lifeTime);

		newColor.a = pow(newColor.a - newTime / lifeTime, 5);

		newVel = newVel + (c_Gravity * newTime * a_Value);

		newPos = newPos + (a_Vel * newTime) + (c_Gravity * .5f * newTime * newTime);

		vec3 vSin = a_Vel * c_RP;
		newPos = newPos + (vSin * sin(newTime * PI * 2 * ratio) * amp);
	}
	else {
		newPos = vec3(10000, 10000, 10000);
	}

	gl_Position = vec4(newPos.xyz, 1);

	v_Rad = abs(a_Position.x);
	v_OrignXY = a_Position.xy;
	v_Color = newColor;
}