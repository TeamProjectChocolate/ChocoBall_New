float4x4 World;
float4x4 View;
float4x4 Proj;

// スキン行列
#define MAX_MATRICES 26
float4x3 g_WorldMatrixArray[MAX_MATRICES]:WORLDMATRIXARRAY;
float g_numBone;		// 骨の数

float g_PlayerHorizon;	// シャドウマップに書き込むかの境界線。

struct VS_INPUT{
	float4	pos		: POSITION;
	float4  BlendWeights:BLENDWEIGHT;
	float4  BlendIndices:BLENDINDICES;
};

// 一定以上の高さにあるものの影を描画したくない時に使用。
// Horizon成分は描画するものと描画しないものの境界線。
struct VS_INPUT_HORIZON {
	VS_INPUT base;
	float Horizon:TEXCOORD1;
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
	float Horizon : TEXCOORD1;
	float4 wPos:TEXCOORD2;
	//float4 worldPos : TEXCOORD1;
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

	//Out.worldPos = Out.pos;
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

VS_OUTPUT VS_ShadowMain_Horizon(VS_INPUT_HORIZON In, uniform bool isBone) {
	float4x4 LightViewProj;	// ライトから見たときのビュープロジェクション行列
	LightViewProj = mul(View, Proj);
	VS_OUTPUT Out = (VS_OUTPUT)0;
	if (isBone) {
		float4 pos = 0.0f;
		// ブレンドするボーンのインデックス
		int4 IndexVector = D3DCOLORtoUBYTE4(In.base.BlendIndices);

		// ブレンドレート
		float BlendWeightsArray[4] = (float[4])In.base.BlendWeights;
		int IndexArray[4] = (int[4])IndexVector;
		float LastWeight = 0.0f;
		for (int iBone = 0; iBone < g_numBone - 1; iBone++) {
			LastWeight = LastWeight + BlendWeightsArray[iBone];
			pos.xyz += mul(In.base.pos, g_WorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
		}
		LastWeight = 1.0f - LastWeight;
		pos.xyz += (mul(In.base.pos, g_WorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight);

		Out.pos = float4(pos.xyz, 1.0f);
	}
	else {
		Out.pos = mul(In.base.pos, World);
	}
	
	// 境界線用処理。
	{
		if (In.Horizon <= g_PlayerHorizon) {
			Out.Horizon = In.Horizon;
		}
		else {
			Out.Horizon = g_PlayerHorizon;
		}
		Out.wPos = Out.pos; 
	}
	Out.pos = mul(Out.pos, LightViewProj);
	Out.depth = Out.pos;
	return Out;
}

float4 PS_ShadowMain(VS_OUTPUT In,uniform bool isHorizon)	: COLOR{
	return 0.0f;
	//if (isNotDraw) {
	//	if (g_Horizon - In.worldPos.y < 0.0f) {
	//		clip(-1.0f);
	//	}
	//}
	if (isHorizon) {
		clip(In.Horizon - In.wPos.y);
	}
	float z = In.depth.z / In.depth.w;
	float dx = ddx(z);
	float dy = ddy(z);
	float Depth_Depth = (z * z) + (0.25f * (dx * dx + dy * dy));
	return float4(z,Depth_Depth,0.0f,1.0f);
}

technique BonelessShadowMapping{
	pass p0{
		VertexShader = compile vs_3_0 VS_ShadowMain(false);
		PixelShader = compile ps_3_0 PS_ShadowMain(false);
	}
};

// 一定以上の高さのものはシャドウマップに書き込まない。
technique BonelessShadowMapping_Horizon{
	pass p0 {
		VertexShader = compile vs_3_0 VS_ShadowMain_Horizon(false);
		PixelShader = compile ps_3_0 PS_ShadowMain(true);
	}
};

technique BoneShadowMapping{
	pass p0{
		VertexShader = compile vs_3_0 VS_ShadowMain(true);
		PixelShader = compile ps_3_0 PS_ShadowMain(false);
	}
};

technique BonelessShadowMapping_I{
	pass p0{
		VertexShader = compile vs_3_0 VS_ShadowMain_I(false);
		PixelShader = compile ps_3_0 PS_ShadowMain(false);
	}
}

technique BoneShadowMapping_I{
	pass p0{
		VertexShader = compile vs_3_0 VS_ShadowMain_I(true);
		PixelShader = compile ps_3_0 PS_ShadowMain(false);
	}
}
