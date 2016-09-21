
//#define ENABLE_NORMAL_MAP		//定義すると法線マップが有効になる。
//#define ENABLE_VERTEX_COLOR	//定義すると頂点カラーが有効になる。

float4x4 World;	// ワールド変換行列宣言
float4x4 View;	// ビュー変換行列宣言
float4x4 Proj;	// 射影変換行列宣言
float4x4 Rota;	// 回転行列。法線をモデルと同じ量だけ回転させるために必要
float3 EyePosition;	// 視点
float4x4 LightViewProj;		// ライトビュープロジェクション行列

float2 g_FarNear;

// 深度書き込み用
float4 g_PintoPoint;	// どの位置にピントを合わせるか(行列変換後の値)
float4x4 g_PintoWorld; // ピント用のワールド行列
float2 g_DepthFarNear;

float Alpha;		// 透明度

#define DIFFUSE_LIGHT_NUM 4		// ディフューズライトの数
float4 diffuseLightDirection[DIFFUSE_LIGHT_NUM];	// ディフューズライトの方向
float4 diffuseLightColor[DIFFUSE_LIGHT_NUM];	// ディフューズライトのカラー
float4 ambientLight;							// 環境光

// スキン行列
#define MAX_MATRICES 26
float4x3 g_WorldMatrixArray[MAX_MATRICES]:WORLDMATRIXARRAY;
float g_numBone;		// 骨の数

float4x4 g_CameraRotaInverse;	// カメラの回転行列の逆行列

float g_luminance;	// 輝度の光の量を調整するための変数


//int g_MaxInstance;
//#define MAX_INSTANCE g_MaxInstance
//float4x4 g_InstancingWorlds[MAX_INSTANCE]:WORLDMATRIXARRAY;	// インスタンシング描画用のワールド行列配列

texture g_Texture;			// テクスチャ
sampler g_TextureSampler = 
sampler_state{
	Texture = <g_Texture>;
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture g_ShadowMap;	// シャドウマップ
sampler g_ShadowMapSampler =
sampler_state{
	Texture = <g_ShadowMap>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
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
#if !defined(ENABLE_VERTEX_COLOR)
	float4	color	: COLOR0;
#endif
	float3  normal  : NORMAL0;	// ワールド座標における法線?
	float2  uv		: TEXCOORD0;
#if !defined(ENABLE_NORMAL_MAP)
	float3  tangent : TANGENT0;	// 接ベクトル(ポリゴンから発される法線と直角に交わるベクトル、ポリゴンに沿う)
#endif
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
#if !defined(ENABLE_VERTEX_COLOR)
	float4	color	: COLOR0;
#endif
	float2  uv		: TEXCOORD0;
	float3  normal	: TEXCOORD1;
	float4  WorldPos	: TEXCOORD2;	// ワールド空間での頂点座標
	float4  ShadowPos	: TEXCOORD3;
#if !defined(ENABLE_NORMAL_MAP)
	float3  tangent :  TEXCOORD4;	// 接ベクトル
#endif
	float4 pos2:TEXCOORD5;
	float4 PintoPoint:TEXCOORD6;
};

struct PS_OUTPUT{
	float4 color:COLOR0;
	float4 depth:COLOR1;
};
// 頂点シェーダ
VS_OUTPUT BasicTransform(VS_INPUT In /*頂点情報(ローカル座標*/)
{
	//ここでいろいろ自由に触ったり、座標変換を行ったりする。

	VS_OUTPUT Screen = (VS_OUTPUT)0;
	float4 pos;
	pos = mul(In.pos, World);	// ワールド座標に変換
	Screen.WorldPos = pos;
	pos = mul(pos, View);			// ビュー座標に変換
	pos = mul(pos, Proj);			// プロジェクション座標に変換
	Screen.pos = pos;
#if !defined(ENABLE_VERTEX_COLOR)
	Screen.color = In.color;
#endif
	Screen.uv = In.uv;
	Screen.normal = mul(In.normal, Rota);		// 法線を回す
#if !defined(ENABLE_NORMAL_MAP)
	Screen.tangent = mul(In.tangent, Rota);		// 接ベクトルを回す
#endif
	Screen.pos2 = Screen.pos;

	float3 PintoPoint = g_PintoPoint.xyz;
	PintoPoint = mul(PintoPoint, g_PintoWorld);
	Screen.PintoPoint = mul(float4(PintoPoint, 1.0f), View);
	Screen.PintoPoint = mul(Screen.PintoPoint, Proj);

	return Screen;	// 頂点情報(スクリーン座標)←スクリーン座標を返さなければエラーとなってしまう。
}

VS_OUTPUT VSMainInstancing(VS_INPUT_INSTANCING In, uniform bool UseBorn)
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
	float3 PintoPoint = g_PintoPoint.xyz;
	PintoPoint = mul(PintoPoint, g_PintoWorld);
	Out.PintoPoint = mul(float4(PintoPoint, 1.0f), View);
	Out.PintoPoint = mul(Out.PintoPoint, Proj);

	return Out;
}

VS_OUTPUT ShadowVertex(VS_INPUT In){
	VS_OUTPUT Out = (VS_OUTPUT)0;
	float4 pos;
	pos = mul(In.pos, World);
	Out.WorldPos = pos;
	Out.ShadowPos = mul(pos, LightViewProj);
	pos = mul(pos, View);
	pos = mul(pos, Proj);
	Out.pos = pos;
#if !defined(ENABLE_VERTEX_COLOR)
	Out.color = In.color;
#endif
	Out.uv = In.uv;
	Out.normal = mul(In.normal, Rota);
#if !defined(ENABLE_NORMAL_MAP)
	Out.tangent = mul(In.tangent, Rota);
#endif
	Out.pos2 = Out.pos;

	float3 PintoPoint = g_PintoPoint.xyz;
	PintoPoint = mul(PintoPoint, g_PintoWorld);
	Out.PintoPoint = mul(float4(PintoPoint, 1.0f), View);
	Out.PintoPoint = mul(Out.PintoPoint, Proj);

	return Out;
}

VS_OUTPUT AnimationVertex(VS_INPUT In){
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// ブレンドするボーンのインデックス
	int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);

	// ブレンドレート
	float BlendWeightsArray[4] = (float[4])In.BlendWeights;
	int IndexArray[4] = (int[4])IndexVector;
	float LastWeight = 0.0f;
	float3 pos = 0.0f;
	float3 normal = 0.0f;
	for (int iBone = 0; iBone < g_numBone - 1; iBone++){
		LastWeight = LastWeight + BlendWeightsArray[iBone];
		pos += mul(In.pos, g_WorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
		normal += mul(In.normal, g_WorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
	}
	LastWeight = 1.0f - LastWeight;
	pos += (mul(In.pos, g_WorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight);
	normal += mul(In.normal, g_WorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight;

	Out.WorldPos = float4(pos.xyz, 1.0f);
	Out.pos = mul(Out.WorldPos, View);
	Out.pos = mul(Out.pos, Proj);
	Out.normal = normal;// mul(normalize(normal), Rota);
#if !defined(ENABLE_VERTEX_COLOR)
	Out.color = In.color;
#endif
	Out.uv = In.uv;
	Out.pos2 = Out.pos;

	float3 PintoPoint = g_PintoPoint.xyz;
	PintoPoint = mul(PintoPoint, g_PintoWorld);
	Out.PintoPoint = mul(float4(PintoPoint, 1.0f), View);
	Out.PintoPoint = mul(Out.PintoPoint, Proj);

	return Out;
}

float4 CalcDiffuseLight(float3 normal){
	float4 lig = 0.0f;
	// ディフューズライトを計算
	float3 diff = 0.0f;
	for (int i = 0; i < DIFFUSE_LIGHT_NUM; i++){
		lig.xyz += max(0.0f, dot(normal.xyz, -diffuseLightDirection[i].xyz)) * diffuseLightColor[i].xyz;
	}
	return lig;
}

float4 CalcSpeculerLight(float3 normal,float4 worldpos){
	float4 lig = 0.0f;
	float3 toEyeDir = normalize(EyePosition - worldpos);
	float3 R = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;
	for (int i = 0; i < DIFFUSE_LIGHT_NUM; i++){
		// スペキュラ成分を計算
		// 反射ベクトルを計算
		float3 L = -diffuseLightDirection[i].xyz;
		lig += diffuseLightColor[i] * pow(max(0.0f, dot(L, R)), 10) * diffuseLightColor[i].w;	// スペキュラ強度
	}
	return lig;
}

/*!
* @brief	アルファに埋め込む輝度を計算。
*/
float CalcLuminance(float3 color)
{
	float luminance = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f)) + g_luminance;
	if (luminance > 1.0f){
		luminance = 1.0f / luminance;
	}
	else{
		luminance = 0.0f;
	}
	return luminance;
}

float vsm(float4 ShadowPos,float3 normal){
	// 影の描画
	float2 shadowMapUV = float2(0.5f, -0.5f) * ShadowPos.xy / ShadowPos.w + float2(0.5f, 0.5f);
	float4 shadow_val = float4(1.0f,1.0f,1.0f,1.0f);
	if (shadowMapUV.x <= 1.0f && shadowMapUV.x >= 0.0f){
		if (shadowMapUV.y <= 1.0f && shadowMapUV.y >= 0.0f){
			if (dot(float3(0.0f, 1.0f, 0.0f), normal) >= 0.1f){
				shadow_val = tex2D(g_ShadowMapSampler, shadowMapUV);
			}
		}
	}
	float depth = (ShadowPos.z - g_FarNear.y) / (g_FarNear.x - g_FarNear.y);

	// VSM(分散シャドウマップ)
	float Variance = shadow_val.y - (shadow_val.x * shadow_val.x);
	return max(shadow_val.x >= depth,Variance + (depth - shadow_val.x));
}

PS_OUTPUT ShadowPixel(VS_OUTPUT In, uniform bool hasNormalMap, uniform bool hasZMask ,uniform bool isIluminance){

	// 基本描画
	float3 normal;		// 法線マップに書き込まれている法線
#if !defined(ENABLE_NORMAL_MAP)
	if (hasNormalMap){
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
#endif
	{
		normal = In.normal;
	}
	if (hasZMask){
		float4 screenPos = In.WorldPos;
		screenPos = mul(screenPos, View);			// ビュー座標に変換
		screenPos = mul(screenPos, Proj);			// プロジェクション座標に変換
		screenPos.xyz /= screenPos.w;
		screenPos.xy *= float2(0.5f, -0.5f);			//-0.5～0.5の範囲にする
		screenPos.xy += 0.5f;						//0.0～1.0の範囲する。
		float4 zmask = tex2D(g_ZMaskSampler, screenPos.xy);
		clip(screenPos.z - zmask.z);
	}

	float4 color = tex2D(g_TextureSampler, In.uv);	// テクスチャを貼り付ける
	// ディフューズライトの計算
	float4 light = CalcDiffuseLight(normal);
	// スペキュラライトを計算
	light += CalcSpeculerLight(normal, In.WorldPos);
	// アンビエントライトを加算
	light.xyz += ambientLight;

	color *= light;	// テクスチャのカラーとライトを乗算

	float shadow = vsm(In.ShadowPos, normal);
	color.xyz = (1.0f - shadow) * shadow + shadow * color.xyz;
	//color.xyz *= vsm(In.ShadowPos,normal);

	//if ((shadow_val.z >= depth) <= ((Variance + (depth - shadow_val.z)) / Variance))
	//if (depth > shadow_val.z){
	//	// 影になっている
	//	color.xyz *= shadow_val.xyz;	// 影を書き込む
	//}

	if (isIluminance){
		// αに輝度を埋め込む
		color.a = CalcLuminance(color.xyz);
	}
	else{
		color.w = Alpha;
	}

	// 深度の書き込み
	float4 OutDepth;
	{
		if (hasZMask){
			float4 work;
			work.xyz = In.pos2.xyz / In.pos2.w;
			work.xy *= float2(0.5f, -0.5f);			//-0.5～0.5の範囲にする
			work.xy += 0.5f;						//0.0～1.0の範囲する。
			float4 zmask = tex2D(g_ZMaskSampler, work.xy);
			clip(work.z - zmask.z);	// ZMaskテクスチャから深度を受け取って、書き込まれた深度よりも奥のピクセルならそのピクセルを破棄する
		}

		// ピントを合わせる位置を計算
		float pinto = /*0.5f*/(In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
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

// ピクセルシェーダ
PS_OUTPUT TextureShader(VS_OUTPUT In, uniform bool hasNormalMap,uniform bool hasIluminance){
	float3 normal;		// 法線マップに書き込まれている法線
#if !defined(ENABLE_NORMAL_MAP)
	if (hasNormalMap){
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
#endif
	{
		normal = In.normal;
	}
	// ディフューズライトの計算
	float4 light = CalcDiffuseLight(normal);
	// スペキュラライトを計算
	light += CalcSpeculerLight(normal,In.WorldPos);
	// アンビエントライトを加算
	light.xyz += ambientLight;

	float4 color = tex2D(g_TextureSampler, In.uv);	// テクスチャを貼り付ける
	color *= light;	// テクスチャのカラーとライトを乗算
	if (hasIluminance){
		color.a = CalcLuminance(color.xyz);
	}
	else{
		color.w = Alpha;
	}

	// 深度の書き込み
	float4 OutDepth;
	{
		// ピントを合わせる位置を計算
		float pinto = /*0.5f*/(In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
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

PS_OUTPUT NoWorkingPixelShader(VS_OUTPUT In, uniform bool hasNormalMap){
	float3 normal;		// 法線マップに書き込まれている法線
#if !defined(ENABLE_NORMAL_MAP)
	if (hasNormalMap){
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
#endif
	{
		normal = In.normal;
	}

	// ディフューズライトの計算
	float4 light = CalcDiffuseLight(normal);
	// スペキュラライトを計算
	light += CalcSpeculerLight(normal,In.WorldPos);
	// アンビエントライトを加算
	light.xyz += ambientLight;
#if !defined(ENABLE_VERTEX_COLOR)
	float4 color = In.color;	// テクスチャを貼り付ける
#else
	float4 color = 1.0f;	// テクスチャを貼り付ける
#endif
	color *= light;	// テクスチャのカラーとライトを乗算
	color.w = Alpha;


	// 深度の書き込み
	float4 OutDepth;
	{
		// ピントを合わせる位置を計算
		float pinto = /*0.5f*/(In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
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

PS_OUTPUT FresnelShader(VS_OUTPUT In, uniform bool hasNormalMap,uniform bool hasShadow,uniform bool hasluminance){
	float3 normal;		// 法線マップに書き込まれている法線
#if !defined(ENABLE_NORMAL_MAP)
	if (hasNormalMap){
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
#endif
	{
		normal = In.normal;
	}

	// ディフューズライトの計算
	float4 light = CalcDiffuseLight(normal);
	// スペキュラライトを計算
	light += CalcSpeculerLight(normal, In.WorldPos);
	// アンビエントライトを加算
	light.xyz += ambientLight;

	// ファーライティングを計算
	// 法線をカメラ座標系に変換する
	float3 normalInCamera = mul(normal, g_CameraRotaInverse);
	float fresnel = 1.0f - abs(dot(normalInCamera, float3(0.0f, 0.0f, 1.0f)));
	fresnel *= fresnel;// pow(fresnel, 1.5f);
	float4 color = tex2D(g_TextureSampler, In.uv);	// テクスチャを貼り付ける
	color *= light;	// テクスチャのカラーとライトを乗算
	color += fresnel;
	float4 shadow_val = float4(1.0f, 1.0f, 1.0f, 1.0f);

	if (hasShadow){
		//// 影の描画
		//float4 ShadowPos = In.ShadowPos;
		//float2 shadowMapUV = float2(0.5f, -0.5f) * ShadowPos.xy / ShadowPos.w + float2(0.5f, 0.5f);
		//if (shadowMapUV.x <= 1.0f && shadowMapUV.x >= 0.0f){
		//	if (shadowMapUV.y <= 1.0f && shadowMapUV.y >= 0.0f){
		//		if (dot(float3(0.0f, 1.0f, 0.0f), normal) >= 0.1f){
		//			shadow_val = tex2D(g_ShadowMapSampler, shadowMapUV);
		//		}
		//	}
		//}
		//float depth = (ShadowPos.z - g_FarNear.y) / (g_FarNear.x - g_FarNear.y);

		//if (depth > shadow_val.z){
		//	// 影になっている
		//	color.xyz *= shadow_val.xyz;
		//}
		float shadow = vsm(In.ShadowPos, normal);
		color.xyz = (1.0f - shadow) * shadow + shadow * color.xyz;
		//color.xyz *= vsm(In.ShadowPos,normal);
	}
	
	if (hasluminance){
		// αに輝度を埋め込む
		color.a = CalcLuminance(color.xyz);
	}
	else{
		color.w = Alpha;
	}

	// 深度の書き込み
	float4 OutDepth;
	{
		// ピントを合わせる位置を計算
		float pinto = /*0.5f*/(In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
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

technique ShadowTec{
	pass p0{
		VertexShader = compile vs_3_0 ShadowVertex();
		PixelShader = compile ps_3_0 ShadowPixel(true, false,false);
	}
};

technique TextureTec{
	pass p0{
		VertexShader = compile vs_3_0 BasicTransform();	// 頂点シェーダ
		PixelShader = compile ps_3_0 TextureShader(true,false);		// ピクセルシェーダ
	}
};

technique BasicTec{
	pass p0{
		VertexShader = compile vs_3_0 BasicTransform();
		PixelShader = compile ps_3_0 NoWorkingPixelShader(true);
	}
};

technique NotNormalMapAnimationTec{
	pass p0{
		VertexShader = compile vs_3_0 AnimationVertex();
		PixelShader = compile ps_3_0 TextureShader(false,false);
	}
};

technique NotNormalMapShadowTec{
	pass p0{
		VertexShader = compile vs_3_0 ShadowVertex();
		PixelShader = compile ps_3_0 ShadowPixel(false, false,false);
	}
};

technique NotNormalMapTextureTec{
	pass p0{
		VertexShader = compile vs_3_0 BasicTransform();	// 頂点シェーダ
		PixelShader = compile ps_3_0 TextureShader(false,false);		// ピクセルシェーダ
	}
};

technique NotNormalMapBasicTec{
	pass p0{
		VertexShader = compile vs_3_0 BasicTransform();
		PixelShader = compile ps_3_0 NoWorkingPixelShader(false);
	}
};

technique NotNormalMapAnimationFresnelTec{
	pass p0{
		VertexShader = compile vs_3_0 AnimationVertex();
		PixelShader = compile ps_3_0 FresnelShader(false,false,true);
	}
}

technique NotNormalMapNonAnimationFresnelTec{
	pass p0{
		VertexShader = compile vs_3_0 BasicTransform();
		PixelShader = compile ps_3_0 FresnelShader(false,false,false);
	}
}

technique ZMask{
	pass p0{
		VertexShader = compile vs_3_0 BasicTransform();
		PixelShader = compile ps_3_0 ZMaskPsShader();
	}
}


technique ShadowMaskTec{
	pass p0{
		VertexShader = compile vs_3_0 ShadowVertex();
		PixelShader = compile ps_3_0 ShadowPixel(false, true,false);
	}
};

technique NotNormalMapNonAnimationFresnelShadowTec{
	pass p0{
		VertexShader = compile vs_3_0 ShadowVertex();
		PixelShader = compile ps_3_0 FresnelShader(false,true,false);
	}
}

technique NotNormalMapNonAnimationBloomTec{
	pass p0{
		VertexShader = compile vs_3_0 BasicTransform();
		PixelShader = compile ps_3_0 TextureShader(false,true);
	}
}

technique NotNormalMapNonAnimationFresnelBloomTec{
	pass p0{
		VertexShader = compile vs_3_0 BasicTransform();
		PixelShader = compile ps_3_0 FresnelShader(false, false, true);
	}
}

//インスタンシング描画用のテクニック(フレネル)
technique NonAnimationInstancing_Fresnel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancing(false);
		PixelShader = compile ps_3_0 FresnelShader(false, false, false);
	}
}

//インスタンシング描画用のテクニック(フレネル)
technique AnimationInstancing_Fresnel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancing(true);
		PixelShader = compile ps_3_0 FresnelShader(false, false, false);
	}
}

//インスタンシング描画用のテクニック(フレネル、影あり)
technique NonAnimationInstancing_SFresnel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancing(false);
		PixelShader = compile ps_3_0 FresnelShader(false, true, false);
	}
}

//インスタンシング描画用のテクニック(フレネル、影あり)
technique AnimationInstancing_SFresnel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancing(true);
		PixelShader = compile ps_3_0 FresnelShader(false, true, false);
	}
}