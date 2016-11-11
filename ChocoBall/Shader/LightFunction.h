
#define DIFFUSE_LIGHT_NUM 4		// ディフューズライトの数
struct SLight{
	float3	diffuseLightDir[DIFFUSE_LIGHT_NUM];		//ディフューズライトの向き。
	float4  diffuseLightColor[DIFFUSE_LIGHT_NUM];	//ディフューズライトのカラー。
	float3	limLightDir;							//リムライトの方向。
	float4	limLightColor;							//リムライトの色。
	float3  ambient;								//アンビエントライト。
};

SLight g_light;		//!<ライト
float3 g_EyePosition;	// 視点
float3 g_EyeDir;

float g_Refractive;

float4 CalcDiffuseLight(float3 normal){
	float4 lig = 0.0f;
	// ディフューズライトを計算
	float3 diff = 0.0f;
	for (int i = 0; i < DIFFUSE_LIGHT_NUM; i++){
		lig.xyz += max(0.0f, dot(normal.xyz, -1.0f * (g_light.diffuseLightDir[i]))) * g_light.diffuseLightColor[i].xyz;
	}
	return lig;
}

float4 CalcSpeculerLight(float3 normal, float4 pos){
	float4 lig = 0.0f;
	float3 toEyeDir = normalize(g_EyePosition - pos);
	float3 R = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;
	for (int i = 0; i < DIFFUSE_LIGHT_NUM; i++){
		// スペキュラ成分を計算
		// 反射ベクトルを計算
		float3 L = -1.0f * (g_light.diffuseLightDir[i]);
		lig += g_light.diffuseLightColor[i] * pow(max(0.0f, dot(L, R)), 10) * g_light.diffuseLightColor[i].w;	// スペキュラ強度
	}
	return lig;
}

/*!
*@brief	リムライトを計算。
*/
float3 CalcLimLight(float3 normal)
{
	float lim = 0.0f;
	float baselim = 1.0f - abs(dot(normal, g_EyeDir));
	lim += baselim * max(0.0f, -dot(g_EyeDir, g_light.limLightDir));
	lim = pow(lim, 1.5f);
	return g_light.limLightColor.xyz * lim;
}

// ここからフレネル関連

textureCUBE g_CubeTex;
samplerCUBE cubeTexSampler =
sampler_state{
	Texture = <g_CubeTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

float CalcFresnel(
	float3 normal,		//ワールド座標系での法線。
	float4 pos,			//ワールド座標系での頂点座標。
	float Refractive1,	//現在光が進んでいる物の反射率。
	float Refractive2	//光が透過しようとしている物質の反射率。
	){
	// フレネル反射率計算。
	float3 EyeDir = normalize(pos - g_EyePosition);	//視線ベクトル算出。
	float InRad = abs(acos(dot(-EyeDir, normal)));	// 入射角算出。
	float Refractive = Refractive1 / Refractive2;	// 屈折率算出。
	float RefractiveRad = InRad * Refractive;	// 屈折角算出。
	float Rs = (sin(InRad - RefractiveRad) / sin(InRad + RefractiveRad)) * (sin(InRad - RefractiveRad) / sin(InRad + RefractiveRad));
	float Rp = (tan(InRad - RefractiveRad) / tan(InRad + RefractiveRad)) * (tan(InRad - RefractiveRad) / tan(InRad + RefractiveRad));

	return Rs + Rp / 2.0f;		// 反射率を返却。
}