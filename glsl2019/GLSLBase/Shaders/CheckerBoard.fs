#version 450
// Lecture6

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;

uniform float u_Time;
in vec2 v_UV;

// multiple texture
void multiple()
{	
	vec2 newUV = v_UV;
	newUV.y = 1.f - newUV.y;

	//float newTime = fract(u_Time);
	
	//vec4 newColor1 = texture(u_Texture, newUV);
	//vec4 newColor2 = texture(u_Texture1, newUV);

	vec4 newColor;
	//newColor = texture(u_Texture, vec2(newUV.x * 2, newUV.y));
	//newColor = texture(u_Texture1, vec2(fract(newUV.x*2), newUV.y));
	
	//if(newUV.x < 0.5f && newUV.y < 0.5f)
	//	newColor = texture(u_Texture, vec2(newUV.x * 2.f, newUV.y * 2.f));
	//else if(newUV.x < 0.5f)
	//	newColor = texture(u_Texture1, vec2(fract(newUV.x * 2.f), fract(newUV.y * 2.f)));
	//else if(newUV.x > 0.5f && newUV.y < 0.5f)
	//	newColor = texture(u_Texture1, vec2(fract(newUV.x * 2.f), fract(newUV.y * 2.f)));
	//else
	//	newColor = texture(u_Texture, vec2(newUV.x * 2.f, newUV.y * 2.f));

	if(newUV.x < 0.5f) {
      if(newUV.y < 0.5f)
         newColor = texture(u_Texture, vec2(newUV * 2));
      else newColor = texture(u_Texture1, vec2(newUV * 2));
   }
   else {
      if(newUV.y < 0.5f)
         newColor = texture(u_Texture2, vec2(newUV * 2));
      else newColor = texture(u_Texture3, vec2(newUV * 2));
   }

	FragColor = newColor;
} 

//block tex coord 5번 숙제
void block2()
{	 
	vec2 newUV = v_UV;
	float tile = 2.f;
	float tiling = 3.f;
	
	newUV = v_UV * tiling;
	//if(v_UV.x > floor(newUV.x) / tiling) 
	//	newUV.y += floor(newUV.y) / tile;
	newUV.y += floor(newUV.x) / tile;
	vec4 newColor = texture(u_Texture, newUV);

	FragColor = newColor;
}

//block tex coord 4번 숙제
void block()
{	
	vec2 newUV = v_UV;
	float tile = 2.f;
	float tiling = 2.f;
	
	newUV = v_UV * tiling;

	//newUV.y = newUV.y;
	//if(v_UV.y > floor(newUV.y) / tiling) 
	newUV.x += floor(newUV.y) / tile;

	vec4 newColor = texture(u_Texture, newUV);

	FragColor = newColor;
}

//twist tex coord 3번
void twist3()
{	
	vec2 newUV = v_UV;
	newUV.y = (2.f - floor(v_UV.y * 3.0f)) / 3.f;
	newUV.y += fract(v_UV.y * 3.f) / 3.f;

	vec4 newColor = texture(u_Texture, newUV);

	FragColor = newColor;
}

//twist tex coord 2번
void twist2()
{	
	vec2 newUV = v_UV;
	newUV.x = fract(newUV.x * 3.f);
	newUV.y = v_UV.y / 3.0f;
	newUV.y += (2.f - floor(v_UV.x * 3.f)) / 3.0f;

	vec4 newColor = texture(u_Texture, newUV);

	FragColor = newColor;
}

//twist tex coord 1번
void twist()
{	
	vec2 newUV = v_UV;
	newUV.x = fract(newUV.x * 3.f);
	// 0 ~ .99
	newUV.y = v_UV.y / 3.0f;
	// 0 ~ 0.3
	newUV.y += floor(v_UV.x * 3.f) / 3.0f;
	// 1,2,3 -> 1/3, 2/3, 3/3
	vec4 newColor = texture(u_Texture, newUV);

	FragColor = newColor;
}

 //19_05_01 blurr
void blurr()
{	
	// default
	/*vec2 newUV = v_UV;
	vec4 newColor = texture(u_Texture, newUV);
	FragColor = newColor;*/
	
	// simple mean blurr
	vec2 newTex[5];
	vec4 newColor[5];
	float width = 1.0 / 256.0;
	float height = 1.0 / 256.0;
	newTex[0] = vec2(v_UV.x, v_UV.y);
	newTex[1] = vec2(v_UV.x - width, v_UV.y);
	newTex[2] = vec2(v_UV.x + width, v_UV.y);
	newTex[3] = vec2(v_UV.x, v_UV.y - height);
	newTex[4] = vec2(v_UV.x, v_UV.y + height);
	newColor[0] = texture(u_Texture, newTex[0]);
	newColor[1] = texture(u_Texture, newTex[1]);
	newColor[2] = texture(u_Texture, newTex[2]);
	newColor[3] = texture(u_Texture, newTex[3]);
	newColor[4] = texture(u_Texture, newTex[4]);

	vec4 FinalColor = newColor[0] + newColor[1] + newColor[2] + newColor[3] + newColor[4]; 
	FinalColor /= 5;

	FragColor = FinalColor;//newColor[0];
}

// 위아래 데칼코마니
void decal()
{	
	vec2 newUV = v_UV;
	//newUV.y *= -1.f;

	newUV = fract(newUV * 1.f );
	newUV.y *= 2.f;
	vec2 temp = abs(newUV - vec2(0, 1.0));// + vec2(0, 1.0f);
	//vec2 temp = newUV - vec2(0.f,1.f);
	// 0~1 -> -0.5 ~ 0.5 -> 0.5 ~0.5 ~ .5 ~ 1.
	vec4 newColor = texture(u_Texture, temp);
	//newColor.y = 1.f - newColor.y;

	FragColor = newColor;
				//vec4(temp, 0, 1);
}

void main()
{
	//multiple();
	block2();
	//block();
	//twist3();
	//twist2();
	//twist();
	//blurr();
	//decal();
}