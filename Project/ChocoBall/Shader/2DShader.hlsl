
float4x4 World;	// ワールド変換行列宣言
float Alpha;	// 透明度
int Split_X;	// 画像分割数(横)
int Split_Y;	// 画像分割数(縦)
int NowCol;		// 現コマ数(横)
int NowRow;		// 現コマ数(縦)
float Ratio_X;	// 元の画像サイズから生成された画像サイズを割った比率(横)
float Ratio_Y;	// 元の画像サイズから生成された画像サイズを割った比率(縦)
float g_brightness;

// 深度書き込み用
float4 g_PintoPoint;	// どの位置にピントを合わせるか(行列変換後の値)
float4x4 g_PintoWorld; // ピント用のワールド行列
float2 g_DepthFarNear;
float4x4 g_View;
float4x4 g_Proj;

texture g_Texture;			// テクスチャ
sampler g_TextureSampler =
sampler_state{
	Texture = g_Texture;
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

// 頂点情報入力用構造体
struct VS_INPUT{
	float4	pos		: POSITION;
	float4	color	: COLOR0;
	float2  uv		: TEXCOORD0;	// 頂点に貼るべきテクスチャのアドレス？
};

// 頂点情報出力用構造体
struct VS_OUTPUT{
	float4	pos		: POSITION;
	float4	color	: COLOR0;
	float2  uv		: TEXCOORD0;
	float4 wpos:TEXCOORD1;
	float4 PintoPoint:TEXCOORD2;
};

struct PS_OUTPUT{
	float4 color:COLOR0;
	float4 depth:COLOR1;
};

// 頂点シェーダ
VS_OUTPUT vs_main(VS_INPUT In /*頂点情報(ローカル座標*/)
{

	//ここでいろいろ自由に触ったり、座標変換を行ったりする。

	VS_OUTPUT Screen = (VS_OUTPUT)0;
	float4 pos;
	pos = mul(In.pos, World);	// ワールド座標に変換
	Screen.pos = pos;
	Screen.color = In.color;
	float2 uv = In.uv;
	uv.x *= 1.0f / Split_X;
	uv.x *= Ratio_X;
	uv.x += ((float)NowCol / Split_X) * Ratio_X;
	uv.y *= 1.0f / Split_Y;
	uv.y *= Ratio_Y;
	uv.y += ((float)NowRow / Split_Y) * Ratio_Y;
	Screen.uv = uv;

	float3 PintoPoint = g_PintoPoint.xyz;
	PintoPoint = mul(PintoPoint, g_PintoWorld);
	Screen.PintoPoint = mul(float4(PintoPoint, 1.0f), g_View);
	Screen.PintoPoint = mul(Screen.PintoPoint, g_Proj);

	return Screen;	// 頂点情報(スクリーン座標)←スクリーン座標を返さなければエラーとなってしまう。
};

// ピクセルシェーダ
PS_OUTPUT ps_main(VS_OUTPUT In){
	In.color = (float4)0.0f;
	float4 color = tex2D(g_TextureSampler, In.uv);
	color.w *= Alpha;

	// 深度の書き込み
	float4 OutDepth;
	{
		// ピントを合わせる位置を計算
		float pinto = (In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
		float offset = 0.5f - pinto;

		// 頂点の座標がFarNearのどの位置にあるか計算
		float linerDepth = (In.wpos.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
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

		float4 TexColor = tex2D(g_TextureSampler, In.uv);
		OutDepth = float4(Depth, Depth, Depth, TexColor.w);
	}
	PS_OUTPUT Out = (PS_OUTPUT)0;
	Out.color = color;
	Out.depth = OutDepth;

	return Out;
};

PS_OUTPUT ps_mainAdd(VS_OUTPUT In){
	In.color = (float4)0.0f;
	float4 color = tex2D(g_TextureSampler, In.uv);
	color.xyz *= color.a * Alpha;

	// 深度の書き込み
	float4 OutDepth;
	{
		// ピントを合わせる位置を計算
		float pinto = (In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
		float offset = 0.5f - pinto;

		// 頂点の座標がFarNearのどの位置にあるか計算
		float linerDepth = (In.wpos.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
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

		float4 TexColor = tex2D(g_TextureSampler, In.uv);
		OutDepth = float4(Depth, Depth, Depth, TexColor.w);
	}
	PS_OUTPUT Out = (PS_OUTPUT)0;
	Out.color = float4(color.xyz, color.w * 1.0f / g_brightness);
	Out.depth = OutDepth;

	return Out;
};

technique BasicTec{
	pass p0{
		VertexShader = compile vs_2_0 vs_main();	// 頂点シェーダ
		PixelShader = compile ps_2_0 ps_main();		// ピクセルシェーダ
	}
}

// 加算合成用テクニック
technique ColorTexPrimAdd{
	pass p0{
		VertexShader = compile vs_2_0 vs_main();	// 頂点シェーダ
		PixelShader = compile ps_2_0 ps_mainAdd();		// ピクセルシェーダ
	}
}