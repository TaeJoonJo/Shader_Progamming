#version 450
// Lecture6

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

uniform int u_Number;		// idigit
in vec2 v_UV;

// Draw Three Number
void ThreeNumber()
{	
	vec2 newUV = v_UV;
	newUV.y = 1.f - newUV.y;

	int a = u_Number / 100;
	int b = u_Number / 10 % 10;
	int c = u_Number % 10;

	int arr[] = {a, b, c};

	vec4 newColor;

	// ±³¼ö´Ô ÄÚµå
	//newUV.x = fract(newUV.x * 3.f);

	newUV.x *= 3.f;
	int newIndex = int(floor(newUV.x));

	newColor = texture(u_Texture, vec2(newUV.x, (newUV.y + arr[newIndex]) * 0.1f));

	FragColor = newColor;
}

// Draw One Number
void OneNumber()
{	
	vec2 newUV = v_UV;
	newUV.y = 1.f - newUV.y;

	int startpoint = 0;

	float newNumber = (u_Number + startpoint) ;
	newUV.y = fract((newUV.y + newNumber) * 0.1f);

	vec4 newColor = texture(u_Texture, newUV);

	FragColor = newColor;
}

void main()
{
	ThreeNumber();
	//OneNumber();
}