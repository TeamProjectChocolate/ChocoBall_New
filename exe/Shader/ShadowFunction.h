
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

float vsm(float4 ShadowPos, float3 normal, uniform bool IsVSM){
	// 影の描画
	float2 shadowMapUV = float2(0.5f, -0.5f) * ShadowPos.xy / ShadowPos.w + float2(0.5f, 0.5f);
	float2 shadow_val = float2(1.0f, 1.0f);
	if (shadowMapUV.x <= 0.99f && shadowMapUV.x >= 0.01f){
		if (shadowMapUV.y <= 0.99f && shadowMapUV.y >= 0.01f){
			if (dot(float3(0.0f, 1.0f, 0.0f), normal) >= 0.1f){
				shadow_val = tex2D(g_ShadowMapSampler, shadowMapUV).xy;
			}
		}
	}
	float depth = ShadowPos.z / ShadowPos.w;

	if (!IsVSM){
		if (shadow_val.x >= depth){
			return 1.0f;
		}
		else{
			return 0.5f;
		}
	}
	else{
		if (depth > shadow_val.x + 0.006f){
			// VSM(分散シャドウマップ)
			// 分散を算出
			float Variance = shadow_val.y - (shadow_val.x * shadow_val.x);
			Variance = max(Variance, 0.0006f);
			float d = depth - shadow_val.x;
			Variance = Variance / (Variance + d * d);
			return pow(Variance, 5.0f);
		}
		else{
			return 1.0f;
		}
	}
}