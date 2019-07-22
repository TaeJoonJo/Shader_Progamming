#version 450
// Lecture6

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

in vec2 v_UV;

uniform float u_BlurSize = 5.f;
uniform vec2 u_TexelSize = vec2(1.f / 1024.f, 1.f / 1024.f);

void main()
{	
	float size = u_BlurSize / 2.f;
	vec2 xDir = vec2(1.f, 0.f);
	vec2 yDir = vec2(0.f, 1.f);
	vec3 newColor = vec3(0.f, 0.f, 0.f);
	float count = 0.f;
	float maxDis = length(size*vec2(1.0, 0.0)*u_TexelSize);

	for(float x = -size; x < size; x += 1.f) {
		for(float y = -size; y < size; y += 1.f) {
			vec2 newTex = v_UV + x*xDir*u_TexelSize + y*yDir*u_TexelSize;
			float dis = length(newTex - v_UV);
			float add = clamp(1.0 - dis/maxDis, 0.f, 1.f);
			add = pow(add, 5);
			vec3 temp = texture(u_Texture, newTex).rgb;
			temp = clamp(temp - vec3(1.f), 0.f, 100.f);
			newColor += temp * add;
			count += 1.f;
		}
	}

	FragColor = vec4(newColor / count + texture(u_Texture, v_UV).rgb, 1.f) ;
}