
float4x4 World;	// ���[���h�ϊ��s��錾
float Alpha;	// �����x
int Split_X;	// �摜������(��)
int Split_Y;	// �摜������(�c)
int NowCol;		// ���R�}��(��)
int NowRow;		// ���R�}��(�c)
float Ratio_X;	// ���̉摜�T�C�Y���琶�����ꂽ�摜�T�C�Y���������䗦(��)
float Ratio_Y;	// ���̉摜�T�C�Y���琶�����ꂽ�摜�T�C�Y���������䗦(�c)
float g_brightness;

// �[�x�������ݗp
float4 g_PintoPoint;	// �ǂ̈ʒu�Ƀs���g�����킹�邩(�s��ϊ���̒l)
float4x4 g_PintoWorld; // �s���g�p�̃��[���h�s��
float2 g_DepthFarNear;
float4x4 g_View;
float4x4 g_Proj;

texture g_Texture;			// �e�N�X�`��
sampler g_TextureSampler =
sampler_state{
	Texture = g_Texture;
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

// ���_�����͗p�\����
struct VS_INPUT{
	float4	pos		: POSITION;
	float4	color	: COLOR0;
	float2  uv		: TEXCOORD0;	// ���_�ɓ\��ׂ��e�N�X�`���̃A�h���X�H
};

// ���_���o�͗p�\����
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

// ���_�V�F�[�_
VS_OUTPUT vs_main(VS_INPUT In /*���_���(���[�J�����W*/)
{

	//�����ł��낢�뎩�R�ɐG������A���W�ϊ����s�����肷��B

	VS_OUTPUT Screen = (VS_OUTPUT)0;
	float4 pos;
	pos = mul(In.pos, World);	// ���[���h���W�ɕϊ�
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

	return Screen;	// ���_���(�X�N���[�����W)���X�N���[�����W��Ԃ��Ȃ���΃G���[�ƂȂ��Ă��܂��B
};

// �s�N�Z���V�F�[�_
PS_OUTPUT ps_main(VS_OUTPUT In){
	In.color = (float4)0.0f;
	float4 color = tex2D(g_TextureSampler, In.uv);
	color.w *= Alpha;

	// �[�x�̏�������
	float4 OutDepth;
	{
		// �s���g�����킹��ʒu���v�Z
		float pinto = (In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
		float offset = 0.5f - pinto;

		// ���_�̍��W��FarNear�̂ǂ̈ʒu�ɂ��邩�v�Z
		float linerDepth = (In.wpos.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
		// �s���g�����킹���ꏊ�𒆐S(0.5f)�Ƃ��A�s���g�������悤�ɒ���
		float Depth = clamp(linerDepth + offset, 0.0f, 1.0f);

		// �N���ɉf��͈͂��L���邽�߂ɐ[�x�l�𒲐����鏈��
		if (Depth > 0.6f){
			Depth = 2.5f * (1.0f - Depth);
		}
		else if (Depth <= 0.6f && Depth >= 0.4f){
			Depth = 1.0f;
		}
		else{
			Depth = 2.5f * Depth;
		}
		// ��O���͋}���ɂڂ���(�s���g��0.1���̏������l�̍ہA��O�����ڂ��Ȃ��Ȃ邽��)
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

	// �[�x�̏�������
	float4 OutDepth;
	{
		// �s���g�����킹��ʒu���v�Z
		float pinto = (In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
		float offset = 0.5f - pinto;

		// ���_�̍��W��FarNear�̂ǂ̈ʒu�ɂ��邩�v�Z
		float linerDepth = (In.wpos.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
		// �s���g�����킹���ꏊ�𒆐S(0.5f)�Ƃ��A�s���g�������悤�ɒ���
		float Depth = clamp(linerDepth + offset, 0.0f, 1.0f);

		// �N���ɉf��͈͂��L���邽�߂ɐ[�x�l�𒲐����鏈��
		if (Depth > 0.6f){
			Depth = 2.5f * (1.0f - Depth);
		}
		else if (Depth <= 0.6f && Depth >= 0.4f){
			Depth = 1.0f;
		}
		else{
			Depth = 2.5f * Depth;
		}
		// ��O���͋}���ɂڂ���(�s���g��0.1���̏������l�̍ہA��O�����ڂ��Ȃ��Ȃ邽��)
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
		VertexShader = compile vs_2_0 vs_main();	// ���_�V�F�[�_
		PixelShader = compile ps_2_0 ps_main();		// �s�N�Z���V�F�[�_
	}
}

// ���Z�����p�e�N�j�b�N
technique ColorTexPrimAdd{
	pass p0{
		VertexShader = compile vs_2_0 vs_main();	// ���_�V�F�[�_
		PixelShader = compile ps_2_0 ps_mainAdd();		// �s�N�Z���V�F�[�_
	}
}