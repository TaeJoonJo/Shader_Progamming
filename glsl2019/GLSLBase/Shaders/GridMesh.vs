#version 450

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;

uniform float u_Time;							// 1.f
uniform vec2 u_Points[5];

uniform mat4 u_ProjView;

const float PI = 3.141592;

out vec2 v_Tex;
out float v_Grey;
out vec3 v_Norm;
out vec3 v_Pos;

uniform sampler2D u_HeightmapTexture;

void Morphing()
{
	float grey = 0;

	for(int i = 0; i < 5; ++i)
	{
		vec2 target;
		vec2 source;
		target = a_Position.xy;
		source = u_Points[i];
		float dist = length(target - source) * 4 * PI * 2.f;			// 0 ~ 2PI
		grey += sin(dist - u_Time);
	}

	float r = (grey) * .1f + .5f;
	float beta = (a_Position.x + .5f) * 2.f * PI;
	float theta = (a_Position.y + .5f) * PI;

	vec3 newPos;
	
	vec3 SpherenewPos = vec3(
		r * sin(theta) * cos(beta),
		r * sin(theta) * sin(beta),
		r * cos(theta)
	);

	vec3 SquarenewPos = a_Position.xyz;
	
	float newTime = fract(u_Time);
	newPos = mix(SquarenewPos, SpherenewPos, newTime); //(SquarenewPos * (1.f - newTime)) + (SpherenewPos * newTime);

	gl_Position = vec4(SpherenewPos.xyz, 1);

	v_Grey = abs(r);
	v_Tex = vec2(0.5, 0.5) + a_Position.xy;
}

void Wave()
{
	float grey = 0;
	vec3 newPos = a_Position.xyz;

	for(int i = 0; i < 5; ++i)
	{
		vec2 target;
		vec2 source;
		target = a_Position.xy;
		source = u_Points[i];
		float dist = length(target - source) * 4 * PI * (i + 1) * 2.f;			// 0 ~ 2PI
		grey += sin(dist - u_Time);
	}
	
	newPos.z += grey * 0.005f;
	gl_Position = u_ProjView * vec4(newPos.xyz, 1);

	v_Grey = (grey + 1.f) / 2.f;
	v_Tex = vec2(0.5, 0.5) + a_Position.xy;
}

void FLAG()
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

   gl_Position = u_ProjView * newPos;
   v_Tex = vec2(0.5, 0.5) + a_Position.xy;
}

void Heightmap()
{	
	float gap = 2.f / 100.f;
	
	vec2 newTex = vec2(0.5, 0.5) + a_Position.xy;	// 0 ~ 1
	vec2 newTexRight = vec2(0.5, 0.5) + a_Position.xy + vec2(gap, 0.f);
	vec2 newTexUp = vec2(0.5, 0.5) + a_Position.xy + vec2(0.f, gap);

	//newTex.y = 1.f - newTex.y;
	//newTexRight.y = 1.f - newTexRight.y;
	//newTexUp.y = 1.f - newTexUp.y;

	float height = texture(u_HeightmapTexture, newTex).r * .4f;
	float heightRight = texture(u_HeightmapTexture, newTexRight).r * .4f;
	float heightUp = texture(u_HeightmapTexture, newTexUp).r * .4f;

	vec4 newPosCenter = vec4(a_Position.xy, a_Position.z + height, 1.f);
	vec4 newPosRight = vec4(a_Position.xy + vec2(gap, 0.f), a_Position.z + heightRight, 1.f);
	vec4 newPosUp = vec4(a_Position.xy + vec2(0.f, gap), a_Position.z + heightUp, 1.f);

	vec4 diff1 = newPosRight - newPosCenter;
	vec4 diff2 = newPosUp - newPosCenter;

	vec3 norm = cross(diff1.xyz, diff2.xyz);

	gl_Position = u_ProjView * newPosCenter;

	v_Tex = newTex;
	v_Grey = height;
	v_Norm = normalize(norm);
	v_Pos = newPosCenter.xyz;
}

void main()
{	
	FLAG();
	//Wave();	
	//Morphing();
	//Heightmap();
}