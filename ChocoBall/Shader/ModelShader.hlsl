
#include "LightFunction.h"
#include "BloomFunction.h"
#include "ShadowFunction.h"

float4x4 World;	// ワールド変換行列宣言
float4x4 View;	// ビュー変換行列宣言
float4x4 Proj;	// 射影変換行列宣言
float4x4 Rota;	// 回転行列。法線をモデルと同じ量だけ回転させるために必要
float4x4 LightViewProj;		// ライトビュープロジェクション行列

// 深度書き込み用
float4 g_PintoPoint;	// どの位置にピントを合わせるか(行列変換後の値)
float4x4 g_PintoWorld; // ピント用のワールド行列
float2 g_DepthFarNear;

float Alpha;		// 透明度

// スキン行列
#define MAX_MATRICES 26
float4x3 g_WorldMatrixArray[MAX_MATRICES]:WORLDMATRIXARRAY;
float g_numBone;		// 骨の数

float4x4 g_CameraRotaInverse;	// カメラの回転行列の逆行列

float g_Bias;	// セルフシャドウ時に使用する。

texture g_Texture;			// テクスチャ
sampler g_TextureSampler = 
sampler_state{
	Texture = <g_Texture>;
	MipFilter = LINEAR;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture g_ZMask;	// Zマスク
sampler g_ZMaskSampler =
sampler_state{
	Texture = <g_ZMask>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

texture g_normalMap;	// 法線マップ
sampler g_normalMapSampler = 
sampler_state{
	Texture = <g_normalMap>;
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

// 頂点情報入力用構造体
struct VS_INPUT{
	float4	pos		: POSITION;
	float4  BlendWeights:BLENDWEIGHT;
	float4  BlendIndices:BLENDINDICES;
	float4	color	: COLOR0;
	float3  normal  : NORMAL0;	// ワールド座標における法線?
	float2  uv		: TEXCOORD0;
	float3  tangent : TANGENT0;	// 接ベクトル(ポリゴンから発される法線と直角に交わるベクトル、ポリゴンに沿う)
};

struct VS_INPUT_INSTANCING{
	VS_INPUT base;
	float4 mWorld1	: TEXCOORD1;		//ワールド行列の1行目
	float4 mWorld2	: TEXCOORD2;		//ワールド行列の2行目
	float4 mWorld3	: TEXCOORD3;		//ワールド行列の3行目
	float4 mWorld4	: TEXCOORD4;		//ワールド行列の4行目
	float4 mRota1:TEXCOORD5;
	float4 mRota2:TEXCOORD6;
	float4 mRota3:TEXCOORD7;
	float4 mRota4:TEXCOORD8;
};

// 頂点情報出力用構造体
struct VS_OUTPUT{
	float4	pos		: POSITION;
	float4	color	: COLOR0;
	float2  uv		: TEXCOORD0;
	float3  normal	: TEXCOORD1;
	float4  WorldPos	: TEXCOORD2;	// ワールド空間での頂点座標
	float4  ShadowPos	: TEXCOORD3;
	float3  tangent :  TEXCOORD4;	// 接ベクトル
	float4 pos2:TEXCOORD5;
	float4 PintoPoint:TEXCOORD6;
};

struct PS_OUTPUT{
	float4 color:COLOR0;
	float4 depth:COLOR1;
};

float4 CalcPinto(){
	float3 PintoPoint = g_PintoPoint.xyz;
	PintoPoint = mul(PintoPoint, g_PintoWorld);
	float4 ret = mul(float4(PintoPoint, 1.0f), View);
	return mul(ret, Proj);
}

// 頂点シェーダー
VS_OUTPUT VS_Main(VS_INPUT In,uniform bool UseBorn){
	VS_OUTPUT Out = (VS_OUTPUT)0;
	float4 pos = float4(0.0f,0.0f,0.0f,1.0f);

	if (UseBorn){
		// ブレンドするボーンのインデックス
		int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);

		// ブレンドレート
		float BlendWeightsArray[4] = (float[4])In.BlendWeights;
		int IndexArray[4] = (int[4])IndexVector;
		float LastWeight = 0.0f;
		for (int iBone = 0; iBone < g_numBone - 1; iBone++){
			LastWeight = LastWeight + BlendWeightsArray[iBone];
			pos.xyz += mul(In.pos, g_WorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
			Out.normal += mul(In.normal, g_WorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
			Out.tangent += mul(In.tangent, g_WorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
		}
		LastWeight = 1.0f - LastWeight;
		pos.xyz += mul(In.pos, g_WorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight;
		Out.normal += mul(In.normal, g_WorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight;
		Out.tangent += mul(In.tangent, g_WorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight;
	}
	else{
		pos = mul(In.pos, World);
		Out.normal = mul(In.normal, Rota);
		Out.tangent = mul(In.tangent, Rota);
	}

	Out.pos = mul(pos, View);
	Out.pos = mul(Out.pos, Proj);
	Out.WorldPos = pos;
	Out.ShadowPos = mul(pos, LightViewProj);
	Out.color = In.color;
	Out.uv = In.uv;
	Out.pos2 = Out.pos;

	// ピントを合わせる頂点を座標変換。
	Out.PintoPoint = CalcPinto();

	return Out;
}

VS_OUTPUT VS_Main_Instancing(VS_INPUT_INSTANCING In, uniform bool UseBorn)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	//if (!UseBorn){
	float4 pos;
	float4x4 worldMat;
	worldMat[0] = In.mWorld1;
	worldMat[1] = In.mWorld2;
	worldMat[2] = In.mWorld3;
	worldMat[3] = In.mWorld4;

	float4x4 rotationMat;
	rotationMat[0] = In.mRota1;
	rotationMat[1] = In.mRota2;
	rotationMat[2] = In.mRota3;
	rotationMat[3] = In.mRota4;

	pos = mul(In.base.pos, worldMat);			//モデルのローカル空間からワールド空間に変換。
	Out.WorldPos = pos;
	Out.ShadowPos = mul(pos, LightViewProj);
	pos = mul(pos, View);			//ワールド空間からビュー空間に変換。
	pos = mul(pos, Proj);	//ビュー空間から射影空間に変換。
	Out.pos = pos;
	Out.color = In.base.color;
	Out.uv = In.base.uv;
	Out.normal = mul(In.base.normal, rotationMat);	//法線を回す。
	Out.tangent = mul(In.base.tangent, rotationMat);		// 接ベクトルを回す
	Out.pos2 = Out.pos;
	//}
	//else{

	//}

	// ピントを合わせる頂点を座標変換。
	Out.PintoPoint = CalcPinto();

	return Out;
}


struct Flags{
	bool Tex;		//テクスチャの有無。
	bool Shadow;	//影を描画するか。
	bool VSM;		//影はVSMか。
	bool NMap;		//法線マップを使用するか。
	bool ZMask;		//Zマスクを使用するか。
	bool Fresnel;	//フレネルをかけるか。
	bool Bloom;		//輝度を埋め込んで光らせるか。
	bool Lim;		//リムライトを使用するか。
};

#define BASIC 0
#define TEX 1
#define SHADOW 2
#define SHADOW_VSM 3
#define NMAP 4
#define ZMASK 5
#define FRESNEL 6
#define BLOOM 7
#define LIM 8
#define TEX_SHADOW_VSM_ZMASK 9
#define TEX_LIM 10
#define TEX_FRESNEL 11
#define TEX_BLOOM 12
#define TEX_SHADOW_VSM_FRESNEL 13
#define TEX_SHADOW_VSM 14
#define TEX_SHADOW 15

static Flags g_FlagsType[] = {
	{ false, false, false, false, false, false, false, false },	// ライトのみを当てた描画。
	{ true, false, false, false, false, false, false, false },	// テクスチャ追加。
	{ false, true, false, false, false, false, false, false },	// シャドウ。
	{ false, true, true, false, false, false, false, false },	// ソフトシャドウ。
	{ false, false, false, true, false, false, false, false },// 法線マップ。
	{ false, false, false, false, true, false, false, false },// Zマスク。
	{ false, false, false, false, false, true, false, false },// フレネル反射。
	{ false, false, false, false, false, false, true, false },// ブルーム。
	{ false, false, false, false, false, false, false, true },// リムライト。
	{ true, true, true, false, true, false, false, false },// テクスチャ、ソフトシャドウ、Zマスク。
	{ true, false, false, false, false, false, false, true },// テクスチャ、リムライト。
	{ true, false, false, false, false, true, false, false },//テクスチャ、フレネル反射。
	{ true, false, false, false, false, false, true, false },//テクスチャ、ブルーム。
	{ true, true, true, false, false, true, false, false },//テクスチャ、ソフトシャドウ、フレネル反射。
	{ true, true, true, false, false, false, false, false },//テクスチャ、ソフトシャドウ。
	{ true, true, false, false, false, false, false, false },//テクスチャ、シャドウ。
};

// ピクセルシェーダ
PS_OUTPUT PS_Main(VS_OUTPUT In, uniform Flags flags){
	PS_OUTPUT o = (PS_OUTPUT)0;

	if (flags.ZMask){
		float4 screenPos = In.WorldPos;
		screenPos = mul(screenPos, View);			// ビュー座標に変換
		screenPos = mul(screenPos, Proj);			// プロジェクション座標に変換
		screenPos.xyz /= screenPos.w;
		screenPos.xy *= float2(0.5f, -0.5f);			//-0.5～0.5の範囲にする
		screenPos.xy += 0.5f;						//0.0～1.0の範囲する。
		float4 zmask = tex2D(g_ZMaskSampler, screenPos.xy);
		clip(screenPos.z - zmask.z);	//ピクセルを破棄するかテスト。
	}

	float3 normal;		// 法線マップに書き込まれている法線
	if (flags.NMap){
		normal = tex2D(g_normalMapSampler, In.uv);	// ここで得られる値は0.0から1.0(本来は-1.0から1.0の意味でなければならない)
		// -1.0～1.0の範囲に調整する
		normal = (normal * 2.0f) - 1.0f;

		float3 biNormal;	// 従ベクトル(ポリゴンに沿うベクトル、三次元空間では軸が三つ必要なため、法線と接ベクトルと従ベクトルを使用する)
		biNormal = normalize(cross(In.tangent, In.normal));	// 接ベクトルとポリゴンから出る法線の外積を求め、従ベクトルを求める

		float4x4 TangentSpaceMatrix;	// ポリゴンのローカル座標(ポリゴンを中心とした三軸の向き)を格納した行列
		TangentSpaceMatrix[0] = float4(In.tangent, 0.0f);	// 接ベクトル
		TangentSpaceMatrix[1] = float4(biNormal, 0.0f);		// 
		TangentSpaceMatrix[2] = float4(In.normal, 0.0f);	// 
		TangentSpaceMatrix[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);	// 

		// ポリゴンの基底軸(ワールド座標から見たポリゴンの軸の向き)と法線マップから得た値を使ってワールド座標での法線を求める
		normal = TangentSpaceMatrix[0] * normal.x + TangentSpaceMatrix[1] * normal.y + TangentSpaceMatrix[2] * normal.z;
	}
	else
	{
		normal = In.normal;
	}

	// ディフューズライトの計算
	float4 light = CalcDiffuseLight(normal);

	// リムライトの計算
	if (flags.Lim){
		light.xyz += CalcLimLight(normal);
		// 簡易リムライト
		//float fresnel = 0.0f;
		//// 法線をカメラ座標系に変換する
		//float3 normalInCamera = mul(normal, g_CameraRotaInverse);
		//fresnel = 1.0f - abs(dot(normalInCamera, float3(0.0f, 0.0f, 1.0f)));
		////fresnel *= fresnel;
		//fresnel = pow(fresnel, 1.0f);
		//light.xyz += fresnel;
	}

	// スペキュラライトを計算
	light += CalcSpeculerLight(normal, In.WorldPos);

	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);

	if (flags.Tex){
		color += tex2D(g_TextureSampler, In.uv);	// テクスチャを貼り付ける
	}
	else{
		color.xyz = In.color.xyz;
	}

	if (flags.Shadow){
		// 環境光は残してディフューズライトとスペキュラライトに影フィルタをかける
		light.xyz *= vsm(In.ShadowPos, normal, flags.VSM);
	}

	// アンビエントライトを加算
	light.xyz += g_light.ambient;

	if (flags.Bloom){
		// αに輝度を埋め込む
		color.a = CalcLuminance(color.xyz);
	}
	else{
		color.w = Alpha;
	}

	if (flags.Fresnel){
		float alpha = color.a;
		// 視線の反射ベクトル算出。
		float3 vReflect = reflect(mul(g_EyeDir, World), normal);
		float4 work1 = texCUBE(cubeTexSampler, vReflect);
		work1.a = 1.0f;
		// 視線の屈折ベクトル算出。
		float3 vRefract = refract(mul(g_EyeDir, World), normal, g_Refractive);
		float4 work2 = texCUBE(cubeTexSampler, vRefract);
		work2.a = 1.0f;
		// フレネル反射率計算。
		float fresnel = CalcFresnel(normal, In.WorldPos, 1.000293f/*地球の大気の屈折率。*/, g_Refractive);
		// 求めた反射率でブレンディング。
		color.xyz *= (1.0f - fresnel) * alpha;
		color.xyz += work1.xyz * fresnel * alpha;
		color.xyz += work2.xyz * (1.0f - alpha)/* * fresnel*/;
		color.a = /*1.0f*/alpha;
		//color += lerp(work1, work2, fresnel);
		//color.a = fresnel*0.5 + 0.5;
		//color.xyz += (work1.xyz * fresnel)/* + (work2 * (1.0f - fresnel))*/;
		//color.a = fresnel;
		//color.xyz += (1.0f - color.a) * work2.xyz;
		//color.a += (1.0f - fresnel);
	}

	color.xyz *= light;	// テクスチャのカラーとライトを乗算

	// 深度の書き込み
	float4 OutDepth;
	{
		// ピントを合わせる位置を計算
		float pinto = (In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
		float offset = 0.5f - pinto;

		// 頂点の座標がFarNearのどの位置にあるか計算
		float linerDepth = (In.pos2.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
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
		OutDepth = float4(Depth, Depth, Depth, 1.0f);
	}

	PS_OUTPUT Out = (PS_OUTPUT)0;
	Out.color = color;
	Out.depth = OutDepth;
	return Out;
}

float4 ZMaskPsShader(VS_OUTPUT In) : COLOR0 {
	float4 screenPos = In.WorldPos;
	screenPos = mul(screenPos, View);			// ビュー座標に変換
	screenPos = mul(screenPos, Proj);			// プロジェクション座標に変換
	return screenPos.z / screenPos.w;
}

technique Boneless{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[BASIC]);
	}
};

technique Boneless_Tex{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(false);	// 頂点シェーダ
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX]);		// ピクセルシェーダ
	}
};

technique Skin_Tex_Fresnel{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(true);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_FRESNEL]);
	}
}

technique Boneless_Tex_Fresnel/*NotNormalMapNonAnimationFresnelTec*/{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_FRESNEL]);
	}
}

technique Boneless_Tex_Shadow_VSM_ZMask{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_SHADOW_VSM_ZMASK]);
	}
};

technique Boneless_Tex_Shadow_ZMask{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_SHADOW_VSM_ZMASK]);
	}
};

technique Boneless_Tex_Shadow_VSM{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_SHADOW_VSM]);
	}
};

technique Boneless_Tex_Shadow{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_SHADOW]);
	}
};

technique Boneless_Tex_Shadow_VSM_Fresnel/*NotNormalMapNonAnimationFresnelShadowTec*/{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_SHADOW_VSM_FRESNEL]);
	}
}

technique Bonelsee_Tex_Bloom/*NotNormalMapNonAnimationBloomTec*/{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_BLOOM]);
	}
}

technique Skin_Tex_Lim{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(true);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_LIM]);
	}
};

technique Boneless_Tex_Lim{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_LIM]);
	}
};

technique Skin_Tex{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(true);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_LIM]);
	}
};

//インスタンシング描画用のテクニック(フレネル)
technique IBoneless_Tex_Fresnel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VS_Main_Instancing(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_FRESNEL]);
	}
}

//インスタンシング描画用のテクニック(リムライト)。
technique IBoneless_Tex_Lim{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main_Instancing(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_LIM]);
	}
};

// ZMask用のテクスチャ生成テクニック。
technique ZMask{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(false);
		PixelShader = compile ps_3_0 ZMaskPsShader();
	}
}