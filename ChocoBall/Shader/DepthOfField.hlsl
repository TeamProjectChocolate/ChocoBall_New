// 被写界深度用シェーダー
texture g_scene;	//シーンテクスチャ。

sampler g_SceneSampler =
sampler_state
{
	Texture = <g_scene>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = POINT;
};


struct VS_DEPTH_INPUT{
	float4	pos		: POSITION;
	float4  BlendWeights:BLENDWEIGHT;
	float4  BlendIndices:BLENDINDICES;
};
struct VS_DEPTH_INPUT_INSTANCING{
	VS_DEPTH_INPUT base;
	float4 mWorld1	: TEXCOORD1;		//ワールド行列の1行目
	float4 mWorld2	: TEXCOORD2;		//ワールド行列の2行目
	float4 mWorld3	: TEXCOORD3;		//ワールド行列の3行目
	float4 mWorld4	: TEXCOORD4;		//ワールド行列の4行目
};

struct VS_OUTPUT_DEPTH{
	float4 pos : POSITION;
	float4 wpos : TEXCOORD0;
	float4 PintoPoint:TEXCOORD1;
};

struct VS_DEPTH_INPUT_PRIM{
	float4	pos		: POSITION;
	float2  uv		: TEXCOORD0;
};
struct VS_OUTPUT_DEPTH_PRIM{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
	float2 SplitedUV : TEXCOORD1;	// 分割したuv
	float4 wpos : TEXCOORD2;
	float4 PintoPoint:TEXCOORD3;
};


texture g_DepthSample;

sampler g_DepthSampler = 
sampler_state{
	Texture = <g_DepthSample>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = POINT;
};

texture g_PrimTex;

sampler g_PrimTexSampler = 
sampler_state{
	Texture = <g_PrimTex>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = POINT;
};

texture g_ZMaskSample;

sampler g_ZMaskSampler =
sampler_state{
	Texture = <g_ZMaskSample>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = POINT;
};

float4x4 g_World;
float4x4 g_View;
float4x4 g_Proj;
float2 g_FarNear:register(c85);
float4 g_PintoPoint;	// どの位置にピントを合わせるか(行列変換後の値)
float4x4 g_PintoWorld; // ピント用のワールド行列

// スキン行列
#define MAX_MATRICES 26
float4x3 g_WorldMatrixArray[MAX_MATRICES]:WORLDMATRIXARRAY;
float g_numBone;		// 骨の数

// パーティクル用
int Split_X;	// 画像分割数(横)
int Split_Y;	// 画像分割数(縦)
int NowCol;		// 現コマ数(横)
int NowRow;		// 現コマ数(縦)
float Ratio_X;	// 元の画像サイズから生成された画像サイズを割った比率(横)
float Ratio_Y;	// 元の画像サイズから生成された画像サイズを割った比率(縦)

VS_OUTPUT_DEPTH VS_DepthSampling(VS_DEPTH_INPUT In, uniform bool IsAnimation){
	VS_OUTPUT_DEPTH Out = (VS_OUTPUT_DEPTH)0.0f;
	float4 pos;
	if (!IsAnimation){
		pos = mul(In.pos, g_World);
		pos = mul(pos, g_View);
		Out.pos = mul(pos, g_Proj);
		Out.wpos = Out.pos;
	}
	else{
		// ブレンドするボーンのインデックス
		int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);

		// ブレンドレート
		float BlendWeightsArray[4] = (float[4])In.BlendWeights;
		int IndexArray[4] = (int[4])IndexVector;
		float LastWeight = 0.0f;
		float3 work = (float3)0.0f;
		for (int iBone = 0; iBone < g_numBone - 1; iBone++){
			LastWeight = LastWeight + BlendWeightsArray[iBone];
			work += mul(In.pos, g_WorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
		}
		LastWeight = 1.0f - LastWeight;
		work += (mul(In.pos, g_WorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight);
		pos = mul(float4(work, 1.0f), g_View);
		Out.pos = mul(pos, g_Proj);
		Out.wpos = Out.pos;
	}

	float3 PintoPoint = g_PintoPoint.xyz;
	PintoPoint = mul(PintoPoint, g_PintoWorld);
	Out.PintoPoint = mul(float4(PintoPoint, 1.0f), g_View);
	Out.PintoPoint = mul(Out.PintoPoint, g_Proj);
	return Out;
}

VS_OUTPUT_DEPTH VS_DepthSampling_Instancing(VS_DEPTH_INPUT_INSTANCING In, uniform bool IsAnimation){
	VS_OUTPUT_DEPTH Out = (VS_OUTPUT_DEPTH)0.0f;
	float4 pos;
	if (!IsAnimation){
		float4x4 worldMat;
		worldMat[0] = In.mWorld1;
		worldMat[1] = In.mWorld2;
		worldMat[2] = In.mWorld3;
		worldMat[3] = In.mWorld4;

		pos = mul(In.base.pos, worldMat);
		pos = mul(pos, g_View);
		Out.pos = mul(pos, g_Proj);
		Out.wpos = Out.pos;
	}
	else{
		// ブレンドするボーンのインデックス
		int4 IndexVector = D3DCOLORtoUBYTE4(In.base.BlendIndices);

		// ブレンドレート
		float BlendWeightsArray[4] = (float[4])In.base.BlendWeights;
		int IndexArray[4] = (int[4])IndexVector;
		float LastWeight = 0.0f;
		float3 work = (float3)0.0f;
		for (int iBone = 0; iBone < g_numBone - 1; iBone++){
			LastWeight = LastWeight + BlendWeightsArray[iBone];
			work += mul(In.base.pos, g_WorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
		}
		LastWeight = 1.0f - LastWeight;
		work += (mul(In.base.pos, g_WorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight);
		pos = mul(float4(work, 1.0f), g_View);
		Out.pos = mul(pos, g_Proj);
		Out.wpos = Out.pos;
	}

	float3 PintoPoint = g_PintoPoint.xyz;
	PintoPoint = mul(PintoPoint, g_PintoWorld);
	Out.PintoPoint = mul(float4(PintoPoint, 1.0f), g_View);
	Out.PintoPoint = mul(Out.PintoPoint, g_Proj);
	return Out;
}

float4 PS_DepthSampling(VS_OUTPUT_DEPTH In,uniform bool hasZMask) :COLOR{

	if (hasZMask){
		float4 work;
		work.xyz = In.wpos.xyz / In.wpos.w;
		work.xy *= float2(0.5f, -0.5f);			//-0.5～0.5の範囲にする
		work.xy += 0.5f;						//0.0～1.0の範囲する。
		float4 zmask = tex2D(g_ZMaskSampler, work.xy);
		clip(work.z - zmask.z);
	}

	// ピントを合わせる位置を計算
	float pinto = /*0.5f*/(In.PintoPoint.z - g_FarNear.y) / (g_FarNear.x - g_FarNear.y);
	float offset = 0.5f - pinto;

	// 頂点の座標がFarNearのどの位置にあるか計算
	float linerDepth = (In.wpos.z - g_FarNear.y) / (g_FarNear.x - g_FarNear.y);
	// ピントを合わせた場所を中心(0.5f)とし、ピントが合うように調整
	float Depth = clamp(linerDepth + offset,0.0f,1.0f);

	// 鮮明に映る範囲を広げるために深度値を調整する処理
	if (Depth > 0.6f){
		Depth = 2.5f * (1.0f - Depth);
	}
	else if(Depth <= 0.6f && Depth >= 0.4f){
		Depth = 1.0f;
	}
	else{
		Depth = 2.5f * Depth;
	}
	// 手前側は急激にぼかす(ピントが0.1等の小さい値の際、手前側がぼけなくなるため)
	if(linerDepth < pinto){
		Depth = Depth * (linerDepth / pinto);
	}

	return float4(Depth, Depth, Depth, 1.0f);
}

// 頂点シェーダ
VS_OUTPUT_DEPTH_PRIM VS_DepthSampling_Primitive(VS_DEPTH_INPUT_PRIM In /*頂点情報(ローカル座標*/)
{
	VS_OUTPUT_DEPTH_PRIM Out;
	float4 pos;
	pos = mul(In.pos, g_World);	// ワールド座標に変換
	Out.wpos = Out.pos = pos;

	float2 uv = Out.uv = In.uv;
	uv.x *= 1.0f / Split_X;
	uv.x *= Ratio_X;
	uv.x += ((float)NowCol / Split_X) * Ratio_X;
	uv.y *= 1.0f / Split_Y;
	uv.y *= Ratio_Y;
	uv.y += ((float)NowRow / Split_Y) * Ratio_Y;
	Out.SplitedUV = uv;

	float3 PintoPoint = g_PintoPoint.xyz;
	PintoPoint = mul(PintoPoint, g_PintoWorld);
	Out.PintoPoint = mul(float4(PintoPoint, 1.0f), g_View);
	Out.PintoPoint = mul(Out.PintoPoint, g_Proj);

	return Out;	// 頂点情報(スクリーン座標)←スクリーン座標を返さなければエラーとなってしまう。
};

// ピクセルシェーダ
float4 PS_DepthSampling_Primitive(VS_OUTPUT_DEPTH_PRIM In) : COLOR0{
	// ピントを合わせる位置を計算
	float pinto = (In.PintoPoint.z - g_FarNear.y) / (g_FarNear.x - g_FarNear.y);
	float offset = 0.5f - pinto;

	// 頂点の座標がFarNearのどの位置にあるか計算
	float linerDepth = (In.wpos.z - g_FarNear.y) / (g_FarNear.x - g_FarNear.y);
	// ピントを合わせた場所を中心(0.5f)とし、ピントが合うように調整
	float Depth = clamp(linerDepth + offset, 0.0f, 1.0f);

	// 鮮明に映る範囲を広げるために深度値を調整する処理
	if (Depth > 0.6f){
		Depth = 2.5f * (1.0f - Depth);
	}
	else if (Depth <= 0.6f && Depth >= 0.4f){
		Depth = 1.0f;
	}
	else{
		Depth = 2.5f * Depth;
	}
	// 手前側は急激にぼかす(ピントが0.1等の小さい値の際、手前側がぼけなくなるため)
	if (linerDepth < pinto){
		Depth = Depth * (linerDepth / pinto);
	}

	float4 TexColor = tex2D(g_PrimTexSampler, In.SplitedUV);
	return float4(Depth, Depth, Depth,TexColor.w);
};

struct VS_INPUT{
	float4	pos : POSITION;
};

struct VS_OUTPUT{
	float4 pos : POSITION;
	float2  tex : TEXCOORD;
};

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

texture g_Texture;
sampler g_TextureSampler = 
sampler_state{
	Texture = <g_Texture>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = POINT;
};

/*!
* @brief	ファイナル。
*/
VS_OUTPUT VSFinal(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.pos = In.pos;		//トランスフォーム済み頂点なのでそのまま
	Out.tex = (In.pos.xy * 0.5f) + 0.5f;
	Out.tex.y = 1.0f - Out.tex.y;
	//Out.tex += g_offset;
	return Out;
}
float4 PSFinal(VS_OUTPUT In) : COLOR
{
	float2 uv = In.tex;
	float4 Depth = tex2D(g_DepthSampler, uv);	// 深度テクスチャ
	float4 Color1 = tex2D(g_TextureSampler, uv);	// ボケなしテクスチャ
	float4 Color2 = tex2D(g_blurSampler, uv);// ボケテクスチャ
	float4 Color = float4(Color1.xyz * Depth.x + Color2.xyz * (1.0f - Depth.x), 1.0f);
	return Color;
}

// 深度情報抽出テクニック(ボーンなし)
technique DepthSampling_NonAnimation{
	pass p0{
		VertexShader = compile vs_3_0 VS_DepthSampling(false);
		PixelShader = compile ps_3_0 PS_DepthSampling(false);
	}
}
// 深度情報抽出テクニック(ボーンあり)
technique DepthSampling_Animation{
	pass p0{
		VertexShader = compile vs_3_0 VS_DepthSampling(true);
		PixelShader = compile ps_3_0 PS_DepthSampling(false);
	}
}
// 深度情報抽出テクニック(ボーンなし、ZMask)
technique DepthSampling_NonAnimation_Z{
	pass p0{
		VertexShader = compile vs_3_0 VS_DepthSampling(false);
		PixelShader = compile ps_3_0 PS_DepthSampling(true);
	}
}
// 深度情報抽出テクニック(ボーンあり、ZMask)
technique DepthSampling_Animation_Z{
	pass p0{
		VertexShader = compile vs_3_0 VS_DepthSampling(true);
		PixelShader = compile ps_3_0 PS_DepthSampling(true);
	}
}
// 深度情報抽出テクニック(ボーンなし、インスタンシング)
technique DepthSampling_NonAnimation_I{
	pass p0{
		VertexShader = compile vs_3_0 VS_DepthSampling_Instancing(false);
		PixelShader = compile ps_3_0 PS_DepthSampling(false);
	}
}
// 深度情報抽出テクニック(ボーンあり、インスタンシング)
technique DepthSampling_Animation_I{
	pass p0{
		VertexShader = compile vs_3_0 VS_DepthSampling_Instancing(true);
		PixelShader = compile ps_3_0 PS_DepthSampling(false);
	}
}
// 深度情報抽出テクニック(ボーンなし、ZMask、インスタンシング)
technique DepthSampling_NonAnimation_ZI{
	pass p0{
		VertexShader = compile vs_3_0 VS_DepthSampling_Instancing(false);
		PixelShader = compile ps_3_0 PS_DepthSampling(true);
	}
}
// 深度情報抽出テクニック(ボーンあり、ZMask、インスタンシング)
technique DepthSampling_Animation_ZI{
	pass p0{
		VertexShader = compile vs_3_0 VS_DepthSampling_Instancing(true);
		PixelShader = compile ps_3_0 PS_DepthSampling(true);
	}
}

// 深度情報抽出テクニック(パーティクル)
technique DepthSampling_Primitive{
	pass p0{
		VertexShader = compile vs_3_0 VS_DepthSampling_Primitive();
		PixelShader = compile ps_3_0 PS_DepthSampling_Primitive();
	}
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
technique Final
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSFinal();
		PixelShader = compile ps_3_0 PSFinal();
	}
}