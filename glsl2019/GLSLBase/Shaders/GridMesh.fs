#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_TextureSnow;
uniform sampler2D u_TextureGrass;

const vec3 c_Light1 = vec3(0.f, -1.f, 0.f);
const vec3 c_CameraDir = vec3(0.f, -1.f, 0.5f);

in vec2 v_Tex;
in float v_Grey;
in vec3 v_Norm;
in vec3 v_Pos;

// HEIGHTMAP
void main()
{
	vec2 newTex = v_Tex;
	//newTex.y = 1.f - newTex.y;

	//vec4 newColor = texture(u_Texture, newTex) * v_Grey;
	vec4 snowColor = texture(u_TextureSnow, newTex * 10.f) * v_Grey;
	vec4 grassColor = texture(u_TextureGrass, newTex * 10.f) * (1.f - v_Grey);
	vec4 finalColor = snowColor + grassColor;

	float a = 0.4f;
	float d = 0.5f;
	float s = 1.f;

	vec3 lightDir = c_Light1 - v_Pos;
	vec3 ambient = vec3(1.f, 1.f, 1.f);
	float diffuse = clamp(dot(lightDir, v_Norm), 0.f, 1.f);
	vec3 diffuseColor = vec3(1.f, 1.f, 1.f);
	vec3 reflectDir = reflect(lightDir, v_Norm);
	vec3 viewDir = v_Pos - c_CameraDir;
	vec3 specularColor = vec3(1.f, 1.f, 1.f);
	float specular = clamp(dot(viewDir, reflectDir), 0.f, 1.f);
	specular = pow(specular, 12);

	vec3 newColor = ambient*a + diffuseColor*diffuse*d + specularColor*specular*s;

	//FragColor = finalColor;
	FragColor = vec4(newColor, 1);
}

// FLAG
//void main()
//{
//	vec2 newTex = v_Tex;
//	newTex.y = 1.f - newTex.y;
//	//vec4 newColor;
//	//newColor = vec4(v_Grey);
//	//FragColor = newColor;
//	vec4 newColor = texture(u_Texture, newTex) * v_Grey;
//
//	FragColor = newColor;
//}