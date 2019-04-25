#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Pos;
in vec2 v_UV;

uniform vec2 u_Points[5];
uniform float u_Time;

const vec2 c_Points[2] = {vec2(-0.4f, 0.2f), vec2(0.4, 0.3)};

void main()
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
			for(int i = 0; i < 2; ++i) {
				vec2 newPoint = c_Points[i];
				vec2 newVec = newPoint - newUV;

				float dist = sqrt(pow(newVec.x, 2) + pow(newVec.y, 2));

				if(dist < 0.1)
				{
					pointGrey += 0.5 * pow((1 - (dist / 0.1)), 2);
				}
			}
		}
	}
	
	FragColor = vec4(pointGrey);
}