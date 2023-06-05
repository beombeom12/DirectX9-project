matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture		g_DefaultTexture;

sampler		DefaultSampler = sampler_state
{
	texture = g_DefaultTexture;
	minfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	vector		vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(DefaultSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_MAIN_ALPHA_MIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor.rgba = tex2D(DefaultSampler, In.vTexUV).rgba;

	return Out;
}

PS_OUT PS_MAIN_ALPHA_MAX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor.rgba = tex2D(DefaultSampler, In.vTexUV).rgba;
	Out.vColor.a = In.vTexUV.y;
	return Out;
}

technique DefaultTechnique
{
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass AlphaMin
	{
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ALPHATESTENABLE = true;
		ALPHAREF = 0xcc;
		ALPHAFUNC = greater;
		BlendOp = add;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHA_MIN();
	}

	pass AlphaMax
	{
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ALPHATESTENABLE = false;
		ALPHAREF = 0xcc;
		ALPHAFUNC = less;
		BlendOp = max;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHA_MAX();
	}
}