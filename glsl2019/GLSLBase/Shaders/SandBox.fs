#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Pos;
in vec2 v_UV;

uniform vec2 u_Points[5];
uniform float u_Time;

const vec2 c_Points[5] = {vec2(-0.4f, 0.5f), vec2(0.4, 0.3), vec2(0.f, 0.f), vec2(-0.5f, -0.5f), vec2(0.5f, -0.5f)};

void Radar()
{
	vec2 newUV = v_UV - vec2(0.5f, 0.5f);

	float pointGrey = 0;

	float distance = length(newUV);
	float newTime = fract(u_Time);
	float ringwidth = 0.1;

	if(distance < newTime + ringwidth) {
		if(distance > newTime) {
			float temp = (distance - newTime) / ringwidth;
			pointGrey = temp;
		}
		else{
			for(int i = 0; i < 5; ++i) {
				vec2 newPoint = c_Points[i];
				vec2 newVec = newPoint - newUV;

				float dist = sqrt(pow(newVec.x, 2) + pow(newVec.y, 2));

				if(dist < 0.1)
				{
					pointGrey += 0.8 * pow((1 - (dist / 0.1)), 2);
				}
			}
		}
	}
	
	FragColor = vec4(pointGrey);
}

void Waves()
{
	vec4 newColor = vec4(0);
	vec2 newUV = v_UV - vec2(0.5f, 0.5f); // - 0.5~0.5 x, y

	for(int i = 0; i < 5; ++i) {
		vec2 newPoint = c_Points[i];
		vec2 newVec = newPoint - newUV;
		float dist = length(newVec) * 8 * 3.141592 + u_Time;
		newColor += vec4(sin(dist));
	}
	FragColor = newColor;
}

void main()
{
	//Radar();
	Waves();
}