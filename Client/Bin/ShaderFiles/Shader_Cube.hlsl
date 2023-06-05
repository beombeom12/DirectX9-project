
float4x4      g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4         gWorldCameraPosition;
float4         gWorldLightPosition;
textureCUBE      g_DefaultTexture;

sampler TextureSampler = sampler_state {
	texture = g_DefaultTexture;
	//minfilter = linear;
	//magfilter = linear;
	//mipfilter = linear;
};

float3 gLightColor;

/* VertexShader_진입점 */
struct VS_INPUT
{
	float3 vPosition : POSITION;
	float3 vNormal   : NORMAL;
	float3 vTexUV    : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 vPosition : POSITION;
	float3 vTexUV : TEXCOORD0;
	//float3 vWorldPos : TEXCOORD1;
	float3 vDiffuse : TEXCOORD1;
	float3 vViewDir : TEXCOORD2;
	float3 vReflection : TEXCOORD3;
};

VS_OUTPUT VS_MAIN(VS_INPUT Input)
{
	VS_OUTPUT         Output = (VS_OUTPUT)0;

	Output.vPosition = mul(vector(Input.vPosition, 1.f), g_WorldMatrix);

	float3   fLightDir = Output.vPosition.xyz - gWorldLightPosition.xyz;
	fLightDir = normalize(fLightDir);

	float3 viewDir = normalize(Output.vPosition.xyz - gWorldCameraPosition.xyz);
	Output.vViewDir = viewDir;

	Output.vPosition = mul(Output.vPosition, g_ViewMatrix);
	Output.vPosition = mul(Output.vPosition, g_ProjMatrix);

	float3 Worldnormal = mul(Input.vNormal, (float3x3)g_WorldMatrix);
	Worldnormal = normalize(Worldnormal);

	Output.vDiffuse = dot(-fLightDir, Worldnormal);
	Output.vReflection = reflect(fLightDir, Worldnormal);

	Output.vTexUV = Input.vTexUV;

	return Output;
}

/* w 나누기연산. */
/* 뷰포트 변환. */
/* 래스터라이즈  */

struct PS_INPUT
{
	//float4 vPosition : POSITION;
	//float3 vTexUV : TEXCOORD0;
	//float3 vWorldPos : TEXCOORD1;
	float3 vTexUV : TEXCOORD0;
	float3 vDiffuse : TEXCOORD1;
	float3 vViewDir : TEXCOORD2;
	float3 vReflection : TEXCOORD3;
};

struct PS_OUTPUT
{
	vector      vColor : COLOR0;
};

/* 화면에 출력해야할 픽셀의 최종적인 색을 결정하낟.  */
PS_OUTPUT PS_MAIN(PS_INPUT Input)
{
	PS_OUTPUT         Output = (PS_OUTPUT)0;

	//Out.vColor = texCUBE(TextureSampler, In.vTexUV);
	float4 albedo = texCUBE(TextureSampler, Input.vTexUV);
	float3 diffuse = gLightColor * albedo.rgb * saturate(Input.vDiffuse);

	float3 reflection = normalize(Input.vReflection);
	float3 viewDir = normalize(Input.vViewDir);
	float3 specular = 0;

	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 100.f);

		float4 specularIntensity = texCUBE(TextureSampler, Input.vTexUV);
		specular *= specularIntensity.rgb * gLightColor;
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

	Output.vColor = float4(ambient + diffuse + specular, 1);

	return Output;
}

technique      DefaultTechnique
{
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}