float4x4 World;
float4x4 View;
float4x4 Proj;

float2 g_FarNear;	// 遠平面と近平面。xに遠平面、yに近平面

// スキン行列
#define MAX_MATRICES 26
float4x3 g_WorldMatrixArray[MAX_MATRICES]:WORLDMATRIXARRAY;
float g_numBone;		// 骨の数

struct VS_INPUT{
	float4	pos		: POSITION;
	float4  BlendWeights:BLENDWEIGHT;
	float4  BlendIndices:BLENDINDICES;
};

struct VS_INPUT_INSTANCING{
	VS_INPUT base;
	float4 worldMat1:TEXCOORD1;
	float4 worldMat2:TEXCOORD2;
	float4 worldMat3:TEXCOORD3;
	float4 worldMat4:TEXCOORD4;
};


// 頂点情報出力用構造体
struct VS_OUTPUT{
	float4	pos		: POSITION;
	float4  depth	: TEXCOORD;
};

VS_OUTPUT VS_ShadowMain(VS_INPUT In, uniform bool isBone){
	float4x4 LightViewProj;	// ライトから見たときのビュープロジェクション行列
	LightViewProj = mul(View, Proj);
	VS_OUTPUT Out = (VS_OUTPUT)0;

	if (isBone){
		float4 pos = 0.0f;
		// ブレンドするボーンのインデックス
		int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);

		// ブレンドレート
		float BlendWeightsArray[4] = (float[4])In.BlendWeights;
		int IndexArray[4] = (int[4])IndexVector;
		float LastWeight = 0.0f;
		for (int iBone = 0; iBone < g_numBone - 1; iBone++){
			LastWeight = LastWeight + BlendWeightsArray[iBone];
			pos.xyz += mul(In.pos, g_WorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
		}
		LastWeight = 1.0f - LastWeight;
		pos.xyz += (mul(In.pos, g_WorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight);

		Out.pos = float4(pos.xyz, 1.0f);
	}
	else{
		Out.pos = mul(In.pos, World);
	}
	Out.pos = mul(Out.pos, LightViewProj);
	Out.depth = Out.pos;
	return Out;
}

VS_OUTPUT VS_ShadowMain_I(VS_INPUT_INSTANCING In, uniform bool isBone){
	float4x4 LightViewProj;	// ライトから見たときのビュープロジェクション行列
	LightViewProj = mul(View, Proj);

	float4x4 worldMat;
	worldMat[0] = In.worldMat1;
	worldMat[1] = In.worldMat2;
	worldMat[2] = In.worldMat3;
	worldMat[3] = In.worldMat4;

	VS_OUTPUT Out = (VS_OUTPUT)0;

	if (isBone){
		//float4 pos = 0.0f;
		//	// ブレンドするボーンのインデックス
		//	int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);

		//	// ブレンドレート
		//	float BlendWeightsArray[4] = (float[4])In.BlendWeights;
		//int IndexArray[4] = (int[4])IndexVector;
		//float LastWeight = 0.0f;
		//for (int iBone = 0; iBone < g_numBone - 1; iBone++){
		//	LastWeight = LastWeight + BlendWeightsArray[iBone];
		//	pos.xyz += mul(In.pos, g_WorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
		//}
		//LastWeight = 1.0f - LastWeight;
		//pos.xyz += (mul(In.pos, g_WorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight);

		//Out.pos = float4(pos.xyz, 1.0f);
	}
	else{
		Out.pos = mul(In.base.pos, worldMat);
	}
	Out.pos = mul(Out.pos, LightViewProj);
	Out.depth = Out.pos;
	return Out;
}


float4 PS_ShadowMain(VS_OUTPUT In)	: COLOR{
	float z = (In.depth.z - g_FarNear.y) / (g_FarNear.x - g_FarNear.y);
	float dx = ddx(z);
	float dy = ddy(z);
	float Depth_Depth = (z * z) + (0.25f * (dx * dx + dy * dy));
	return float4(z,Depth_Depth,0.0f,1.0f);
}

technique BonelessShadowMapping{
	pass p0{
		VertexShader = compile vs_3_0 VS_ShadowMain(false);
		PixelShader = compile ps_3_0 PS_ShadowMain();
	}
};

technique BoneShadowMapping{
	pass p0{
		VertexShader = compile vs_3_0 VS_ShadowMain(true);
		PixelShader = compile ps_3_0 PS_ShadowMain();
	}
};

technique BonelessShadowMapping_I{
	pass p0{
		VertexShader = compile vs_3_0 VS_ShadowMain_I(false);
		PixelShader = compile ps_3_0 PS_ShadowMain();
	}
}

technique BoneShadowMapping_I{
	pass p0{
		VertexShader = compile vs_3_0 VS_ShadowMain_I(true);
		PixelShader = compile ps_3_0 PS_ShadowMain();
	}
}

struct VS_BlurOutput{
	float4 pos	: POSITION;
	float2 tex0	: TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float2 tex3 : TEXCOORD3;
	float2 tex4 : TEXCOORD4;
	float2 tex5 : TEXCOORD5;
	float2 tex6 : TEXCOORD6;
	float2 tex7 : TEXCOORD7;
};
texture g_blur;	//ブラーテクスチャ

sampler g_blurSampler =
sampler_state
{
	Texture = <g_blur>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

float2 g_TexSize;		//テクスチャのサイズ。
float2 g_offset;				//オフセット
float  g_weight[8];				//ガウスフィルタの重み。

/*!
* @brief	Xブラー頂点シェーダー。
*/
VS_BlurOutput VSXBlur(VS_INPUT In)
{
	VS_BlurOutput Out;
	Out.pos = In.pos;
	float2 tex = (In.pos * 0.5f) + 0.5f;
		tex.y = 1.0f - tex.y;
	tex += float2(0.5 / g_TexSize.x, 0.5 / g_TexSize.y);
	Out.tex0 = tex + float2(-1.0f / g_TexSize.x, 0.0f);
	Out.tex1 = tex + float2(-3.0f / g_TexSize.x, 0.0f);
	Out.tex2 = tex + float2(-5.0f / g_TexSize.x, 0.0f);
	Out.tex3 = tex + float2(-7.0f / g_TexSize.x, 0.0f);
	Out.tex4 = tex + float2(-9.0f / g_TexSize.x, 0.0f);
	Out.tex5 = tex + float2(-11.0f / g_TexSize.x, 0.0f);
	Out.tex6 = tex + float2(-13.0f / g_TexSize.x, 0.0f);
	Out.tex7 = tex + float2(-15.0f / g_TexSize.x, 0.0f);
	return Out;
}
/*!
* @brief	Xブラーピクセルシェーダー。
*/
float4 PSXBlur(VS_BlurOutput In) : COLOR
{
	float4 Color;
	Color = g_weight[0] * (tex2D(g_blurSampler, In.tex0)
		+ tex2D(g_blurSampler, In.tex7 + g_offset));
	Color += g_weight[1] * (tex2D(g_blurSampler, In.tex1)
		+ tex2D(g_blurSampler, In.tex6 + g_offset));
	Color += g_weight[2] * (tex2D(g_blurSampler, In.tex2)
		+ tex2D(g_blurSampler, In.tex5 + g_offset));
	Color += g_weight[3] * (tex2D(g_blurSampler, In.tex3)
		+ tex2D(g_blurSampler, In.tex4 + g_offset));
	Color += g_weight[4] * (tex2D(g_blurSampler, In.tex4)
		+ tex2D(g_blurSampler, In.tex3 + g_offset));
	Color += g_weight[5] * (tex2D(g_blurSampler, In.tex5)
		+ tex2D(g_blurSampler, In.tex2 + g_offset));
	Color += g_weight[6] * (tex2D(g_blurSampler, In.tex6)
		+ tex2D(g_blurSampler, In.tex1 + g_offset));
	Color += g_weight[7] * (tex2D(g_blurSampler, In.tex7)
		+ tex2D(g_blurSampler, In.tex0 + g_offset));
	return Color;
}
/*!
* @brief	Yブラー頂点シェーダー。
*/
VS_BlurOutput VSYBlur(VS_INPUT In)
{
	VS_BlurOutput Out;
	Out.pos = In.pos;
	float2 tex = (In.pos * 0.5f) + 0.5f;
		tex.y = 1.0f - tex.y;
	tex += float2(0.5 / g_TexSize.x, 0.5 / g_TexSize.y);

	Out.tex0 = tex + float2(0.0f, -1.0f / g_TexSize.y);
	Out.tex1 = tex + float2(0.0f, -3.0f / g_TexSize.y);
	Out.tex2 = tex + float2(0.0f, -5.0f / g_TexSize.y);
	Out.tex3 = tex + float2(0.0f, -7.0f / g_TexSize.y);
	Out.tex4 = tex + float2(0.0f, -9.0f / g_TexSize.y);
	Out.tex5 = tex + float2(0.0f, -11.0f / g_TexSize.y);
	Out.tex6 = tex + float2(0.0f, -13.0f / g_TexSize.y);
	Out.tex7 = tex + float2(0.0f, -15.0f / g_TexSize.y);
	return Out;
}
/*!
* @brief	Yブラーピクセルシェーダー。
*/
float4 PSYBlur(VS_BlurOutput In) : COLOR
{
	float4 Color;
	Color = g_weight[0] * (tex2D(g_blurSampler, In.tex0)
		+ tex2D(g_blurSampler, In.tex7 + g_offset));
	Color += g_weight[1] * (tex2D(g_blurSampler, In.tex1)
		+ tex2D(g_blurSampler, In.tex6 + g_offset));
	Color += g_weight[2] * (tex2D(g_blurSampler, In.tex2)
		+ tex2D(g_blurSampler, In.tex5 + g_offset));
	Color += g_weight[3] * (tex2D(g_blurSampler, In.tex3)
		+ tex2D(g_blurSampler, In.tex4 + g_offset));
	Color += g_weight[4] * (tex2D(g_blurSampler, In.tex4)
		+ tex2D(g_blurSampler, In.tex3 + g_offset));
	Color += g_weight[5] * (tex2D(g_blurSampler, In.tex5)
		+ tex2D(g_blurSampler, In.tex2 + g_offset));
	Color += g_weight[6] * (tex2D(g_blurSampler, In.tex6)
		+ tex2D(g_blurSampler, In.tex1 + g_offset));
	Color += g_weight[7] * (tex2D(g_blurSampler, In.tex7)
		+ tex2D(g_blurSampler, In.tex0 + g_offset));
	return Color;
}


/*!
* @brief	横ブラー
*/
technique XBlur
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSXBlur();
		PixelShader = compile ps_3_0 PSXBlur();
	}
}

/*!
* @brief	縦ブラー
*/
technique YBlur
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSYBlur();
		PixelShader = compile ps_3_0 PSYBlur();
	}
}
