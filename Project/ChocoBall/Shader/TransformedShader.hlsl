/*!
* @brief	�g�����X�t�H�[���ς݃v���~�e�B�u��`��B
*/


struct VS_INPUT{
	float4		pos		: POSITION;
	float2		uv		: TEXCOORD0;
};

struct VS_OUTPUT{
	float4		pos		: POSITION;
	float2		uv		: TEXCOORD0;
};

float2 g_offset;				//�I�t�Z�b�g

texture g_tex;
sampler TextureSampler =
sampler_state
{
	Texture = <g_tex>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.pos = In.pos;
	Out.uv = In.uv;
	return Out;
}

float4 PSMain(VS_OUTPUT In) : COLOR0
{
	return tex2D(TextureSampler, In.uv + g_offset);
}

technique TransformedPrim
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}

// ��������u���[
struct VS_OUTPUT_BLUR{
	float4		pos		: POSITION;
	float2 		tex0	: TEXCOORD0;
	float2 		tex1 	: TEXCOORD1;
	float2 		tex2 	: TEXCOORD2;
};

float2 g_TexSize;			//�e�N�X�`���T�C�Y�B

/*!
* @brief	X�u���[�̒��_�V�F�[�_�[�B
*/
VS_OUTPUT_BLUR VSMainBlurX(VS_INPUT In)
{
	VS_OUTPUT_BLUR Out;
	Out.pos = In.pos;

	float2 tex = In.uv;
	Out.tex0 = tex;
	Out.tex1 = float2(0.5f / g_TexSize.x, 0.0f);
	Out.tex2 = float2(1.0f / g_TexSize.x, 0.0f);
	return Out;
}

/*!
* @brief	Y�u���[�̒��_�V�F�[�_�[�B
*/
VS_OUTPUT_BLUR VSMainBlurY(VS_INPUT In)
{
	VS_OUTPUT_BLUR Out = (VS_OUTPUT_BLUR)0;
	Out.pos = In.pos;
	float2 tex = In.uv;

	Out.tex0 = tex;
	Out.tex1 = float2(0.0f, 0.5f / g_TexSize.y);
	Out.tex2 = float2(0.0f, 1.0f / g_TexSize.y);

	return Out;
}
/*!
* @brief	�u���[�̃s�N�Z���V�F�[�_�[�B
*/
float4 PSMainBlur(VS_OUTPUT_BLUR In) : COLOR0
{
	float4 Color;

	//5�_�u���[�������Ă݂�B
	Color = tex2D(TextureSampler, In.tex0);
	Color += tex2D(TextureSampler, In.tex0 + In.tex1);
	Color += tex2D(TextureSampler, In.tex0 - In.tex1);
	Color += tex2D(TextureSampler, In.tex0 + In.tex2);
	Color += tex2D(TextureSampler, In.tex0 - In.tex2);
	Color /= 5.0f;

	return Color;
}

/*!
* @brief	���u���[�B
*/
technique TransformedPrimBlurX
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSMainBlurX();
		PixelShader = compile ps_2_0 PSMainBlur();
	}
}

/*!
* @brief	�c�u���[�B
*/
technique TransformedPrimBlurY
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSMainBlurY();
		PixelShader = compile ps_2_0 PSMainBlur();
	}
}