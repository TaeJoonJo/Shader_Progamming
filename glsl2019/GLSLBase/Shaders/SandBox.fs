#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Pos;
in vec2 v_UV;

uniform vec2 u_Points[5];

const vec2 c_Points[2] = {vec2(-0.4f, 0.2f), vec2(0.4, 0.3)};

void main()
{
	vec2 newUV = v_UV - vec2(0.5f, 0.5f);

	float pointGrey = 0;
	for(int i = 0; i < 2; ++i) {
		vec2 newPoint = c_Points[i];
		vec2 newVec = newPoint - newUV;

		float distance = sqrt(pow(newVec.x, 2) + pow(newVec.y, 4));

		if(distance < 0.1)
		{
			pointGrey += 0.5 * pow((1 - (distance / 0.1)), 2);
		}
	}
	//float dist = sqrt(pow(v_Pos.x, 2) + pow(v_Pos.y, 2));
	float dist = sqrt(pow(newUV.x, 2) + pow(newUV.y, 2));
	float grey = sin(dist * 3.141592 * 4 * 10);

	//if(dist < .5f && dist > .48f) {
	//	FragColor = vec4(1);
	//}
	//else {
	//	FragColor = vec4(0.f, 0.f, 0.f, 1.f);
	//}

	FragColor = vec4(pointGrey);
}