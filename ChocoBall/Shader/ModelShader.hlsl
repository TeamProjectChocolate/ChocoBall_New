
#include "LightFunction.h"
#include "BloomFunction.h"
#include "ShadowFunction.h"

float4x4 World;	// ���[���h�ϊ��s��錾
float4x4 View;	// �r���[�ϊ��s��錾
float4x4 Proj;	// �ˉe�ϊ��s��錾
float4x4 Rota;	// ��]�s��B�@�������f���Ɠ����ʂ�����]�����邽�߂ɕK�v
float4x4 LightViewProj;		// ���C�g�r���[�v���W�F�N�V�����s��

// �[�x�������ݗp
float4 g_PintoPoint;	// �ǂ̈ʒu�Ƀs���g�����킹�邩(�s��ϊ���̒l)
float4x4 g_PintoWorld; // �s���g�p�̃��[���h�s��
float2 g_DepthFarNear;

float Alpha;		// �����x

// �X�L���s��
#define MAX_MATRICES 26
float4x3 g_WorldMatrixArray[MAX_MATRICES]:WORLDMATRIXARRAY;
float g_numBone;		// ���̐�

float4x4 g_CameraRotaInverse;	// �J�����̉�]�s��̋t�s��

float g_Bias;	// �Z���t�V���h�E���Ɏg�p����B

texture g_Texture;			// �e�N�X�`��
sampler g_TextureSampler = 
sampler_state{
	Texture = <g_Texture>;
	MipFilter = LINEAR;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture g_ZMask;	// Z�}�X�N
sampler g_ZMaskSampler =
sampler_state{
	Texture = <g_ZMask>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

texture g_normalMap;	// �@���}�b�v
sampler g_normalMapSampler = 
sampler_state{
	Texture = <g_normalMap>;
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

// ���_�����͗p�\����
struct VS_INPUT{
	float4	pos		: POSITION;
	float4  BlendWeights:BLENDWEIGHT;
	float4  BlendIndices:BLENDINDICES;
	float4	color	: COLOR0;
	float3  normal  : NORMAL0;	// ���[���h���W�ɂ�����@��?
	float2  uv		: TEXCOORD0;
	float3  tangent : TANGENT0;	// �ڃx�N�g��(�|���S�����甭�����@���ƒ��p�Ɍ����x�N�g���A�|���S���ɉ���)
};

struct VS_INPUT_INSTANCING{
	VS_INPUT base;
	float4 mWorld1	: TEXCOORD1;		//���[���h�s���1�s��
	float4 mWorld2	: TEXCOORD2;		//���[���h�s���2�s��
	float4 mWorld3	: TEXCOORD3;		//���[���h�s���3�s��
	float4 mWorld4	: TEXCOORD4;		//���[���h�s���4�s��
	float4 mRota1:TEXCOORD5;
	float4 mRota2:TEXCOORD6;
	float4 mRota3:TEXCOORD7;
	float4 mRota4:TEXCOORD8;
};

// ���_���o�͗p�\����
struct VS_OUTPUT{
	float4	pos		: POSITION;
	float4	color	: COLOR0;
	float2  uv		: TEXCOORD0;
	float3  normal	: TEXCOORD1;
	float4  WorldPos	: TEXCOORD2;	// ���[���h��Ԃł̒��_���W
	float4  ShadowPos	: TEXCOORD3;
	float3  tangent :  TEXCOORD4;	// �ڃx�N�g��
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

// ���_�V�F�[�_�[
VS_OUTPUT VS_Main(VS_INPUT In,uniform bool UseBorn){
	VS_OUTPUT Out = (VS_OUTPUT)0;
	float4 pos = float4(0.0f,0.0f,0.0f,1.0f);

	if (UseBorn){
		// �u�����h����{�[���̃C���f�b�N�X
		int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);

		// �u�����h���[�g
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

	// �s���g�����킹�钸�_�����W�ϊ��B
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

	pos = mul(In.base.pos, worldMat);			//���f���̃��[�J����Ԃ��烏�[���h��Ԃɕϊ��B
	Out.WorldPos = pos;
	Out.ShadowPos = mul(pos, LightViewProj);
	pos = mul(pos, View);			//���[���h��Ԃ���r���[��Ԃɕϊ��B
	pos = mul(pos, Proj);	//�r���[��Ԃ���ˉe��Ԃɕϊ��B
	Out.pos = pos;
	Out.color = In.base.color;
	Out.uv = In.base.uv;
	Out.normal = mul(In.base.normal, rotationMat);	//�@�����񂷁B
	Out.tangent = mul(In.base.tangent, rotationMat);		// �ڃx�N�g������
	Out.pos2 = Out.pos;
	//}
	//else{

	//}

	// �s���g�����킹�钸�_�����W�ϊ��B
	Out.PintoPoint = CalcPinto();

	return Out;
}


struct Flags{
	bool Tex;		//�e�N�X�`���̗L���B
	bool Shadow;	//�e��`�悷�邩�B
	bool VSM;		//�e��VSM���B
	bool NMap;		//�@���}�b�v���g�p���邩�B
	bool ZMask;		//Z�}�X�N���g�p���邩�B
	bool Fresnel;	//�t���l���������邩�B
	bool Bloom;		//�P�x�𖄂ߍ���Ō��点�邩�B
	bool Lim;		//�������C�g���g�p���邩�B
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
	{ false, false, false, false, false, false, false, false },	// ���C�g�݂̂𓖂Ă��`��B
	{ true, false, false, false, false, false, false, false },	// �e�N�X�`���ǉ��B
	{ false, true, false, false, false, false, false, false },	// �V���h�E�B
	{ false, true, true, false, false, false, false, false },	// �\�t�g�V���h�E�B
	{ false, false, false, true, false, false, false, false },// �@���}�b�v�B
	{ false, false, false, false, true, false, false, false },// Z�}�X�N�B
	{ false, false, false, false, false, true, false, false },// �t���l�����ˁB
	{ false, false, false, false, false, false, true, false },// �u���[���B
	{ false, false, false, false, false, false, false, true },// �������C�g�B
	{ true, true, true, false, true, false, false, false },// �e�N�X�`���A�\�t�g�V���h�E�AZ�}�X�N�B
	{ true, false, false, false, false, false, false, true },// �e�N�X�`���A�������C�g�B
	{ true, false, false, false, false, true, false, false },//�e�N�X�`���A�t���l�����ˁB
	{ true, false, false, false, false, false, true, false },//�e�N�X�`���A�u���[���B
	{ true, true, true, false, false, true, false, false },//�e�N�X�`���A�\�t�g�V���h�E�A�t���l�����ˁB
	{ true, true, true, false, false, false, false, false },//�e�N�X�`���A�\�t�g�V���h�E�B
	{ true, true, false, false, false, false, false, false },//�e�N�X�`���A�V���h�E�B
};

// �s�N�Z���V�F�[�_
PS_OUTPUT PS_Main(VS_OUTPUT In, uniform Flags flags){
	PS_OUTPUT o = (PS_OUTPUT)0;

	if (flags.ZMask){
		float4 screenPos = In.WorldPos;
		screenPos = mul(screenPos, View);			// �r���[���W�ɕϊ�
		screenPos = mul(screenPos, Proj);			// �v���W�F�N�V�������W�ɕϊ�
		screenPos.xyz /= screenPos.w;
		screenPos.xy *= float2(0.5f, -0.5f);			//-0.5�`0.5�͈̔͂ɂ���
		screenPos.xy += 0.5f;						//0.0�`1.0�͈̔͂���B
		float4 zmask = tex2D(g_ZMaskSampler, screenPos.xy);
		clip(screenPos.z - zmask.z);	//�s�N�Z����j�����邩�e�X�g�B
	}

	float3 normal;		// �@���}�b�v�ɏ������܂�Ă���@��
	if (flags.NMap){
		normal = tex2D(g_normalMapSampler, In.uv);	// �����œ�����l��0.0����1.0(�{����-1.0����1.0�̈Ӗ��łȂ���΂Ȃ�Ȃ�)
		// -1.0�`1.0�͈̔͂ɒ�������
		normal = (normal * 2.0f) - 1.0f;

		float3 biNormal;	// �]�x�N�g��(�|���S���ɉ����x�N�g���A�O������Ԃł͎����O�K�v�Ȃ��߁A�@���Ɛڃx�N�g���Ə]�x�N�g�����g�p����)
		biNormal = normalize(cross(In.tangent, In.normal));	// �ڃx�N�g���ƃ|���S������o��@���̊O�ς����߁A�]�x�N�g�������߂�

		float4x4 TangentSpaceMatrix;	// �|���S���̃��[�J�����W(�|���S���𒆐S�Ƃ����O���̌���)���i�[�����s��
		TangentSpaceMatrix[0] = float4(In.tangent, 0.0f);	// �ڃx�N�g��
		TangentSpaceMatrix[1] = float4(biNormal, 0.0f);		// 
		TangentSpaceMatrix[2] = float4(In.normal, 0.0f);	// 
		TangentSpaceMatrix[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);	// 

		// �|���S���̊�ꎲ(���[���h���W���猩���|���S���̎��̌���)�Ɩ@���}�b�v���瓾���l���g���ă��[���h���W�ł̖@�������߂�
		normal = TangentSpaceMatrix[0] * normal.x + TangentSpaceMatrix[1] * normal.y + TangentSpaceMatrix[2] * normal.z;
	}
	else
	{
		normal = In.normal;
	}

	// �f�B�t���[�Y���C�g�̌v�Z
	float4 light = CalcDiffuseLight(normal);

	// �������C�g�̌v�Z
	if (flags.Lim){
		light.xyz += CalcLimLight(normal);
		// �ȈՃ������C�g
		//float fresnel = 0.0f;
		//// �@�����J�������W�n�ɕϊ�����
		//float3 normalInCamera = mul(normal, g_CameraRotaInverse);
		//fresnel = 1.0f - abs(dot(normalInCamera, float3(0.0f, 0.0f, 1.0f)));
		////fresnel *= fresnel;
		//fresnel = pow(fresnel, 1.0f);
		//light.xyz += fresnel;
	}

	// �X�y�L�������C�g���v�Z
	light += CalcSpeculerLight(normal, In.WorldPos);

	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);

	if (flags.Tex){
		color += tex2D(g_TextureSampler, In.uv);	// �e�N�X�`����\��t����
	}
	else{
		color.xyz = In.color.xyz;
	}

	if (flags.Shadow){
		// �����͎c���ăf�B�t���[�Y���C�g�ƃX�y�L�������C�g�ɉe�t�B���^��������
		light.xyz *= vsm(In.ShadowPos, normal, flags.VSM);
	}

	// �A���r�G���g���C�g�����Z
	light.xyz += g_light.ambient;

	if (flags.Bloom){
		// ���ɋP�x�𖄂ߍ���
		color.a = CalcLuminance(color.xyz);
	}
	else{
		color.w = Alpha;
	}

	if (flags.Fresnel){
		float alpha = color.a;
		// �����̔��˃x�N�g���Z�o�B
		float3 vReflect = reflect(mul(g_EyeDir, World), normal);
		float4 work1 = texCUBE(cubeTexSampler, vReflect);
		work1.a = 1.0f;
		// �����̋��܃x�N�g���Z�o�B
		float3 vRefract = refract(mul(g_EyeDir, World), normal, g_Refractive);
		float4 work2 = texCUBE(cubeTexSampler, vRefract);
		work2.a = 1.0f;
		// �t���l�����˗��v�Z�B
		float fresnel = CalcFresnel(normal, In.WorldPos, 1.000293f/*�n���̑�C�̋��ܗ��B*/, g_Refractive);
		// ���߂����˗��Ńu�����f�B���O�B
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

	color.xyz *= light;	// �e�N�X�`���̃J���[�ƃ��C�g����Z

	// �[�x�̏�������
	float4 OutDepth;
	{
		// �s���g�����킹��ʒu���v�Z
		float pinto = (In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
		float offset = 0.5f - pinto;

		// ���_�̍��W��FarNear�̂ǂ̈ʒu�ɂ��邩�v�Z
		float linerDepth = (In.pos2.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
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
		OutDepth = float4(Depth, Depth, Depth, 1.0f);
	}

	PS_OUTPUT Out = (PS_OUTPUT)0;
	Out.color = color;
	Out.depth = OutDepth;
	return Out;
}

float4 ZMaskPsShader(VS_OUTPUT In) : COLOR0 {
	float4 screenPos = In.WorldPos;
	screenPos = mul(screenPos, View);			// �r���[���W�ɕϊ�
	screenPos = mul(screenPos, Proj);			// �v���W�F�N�V�������W�ɕϊ�
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
		VertexShader = compile vs_3_0 VS_Main(false);	// ���_�V�F�[�_
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX]);		// �s�N�Z���V�F�[�_
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

//�C���X�^���V���O�`��p�̃e�N�j�b�N(�t���l��)
technique IBoneless_Tex_Fresnel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VS_Main_Instancing(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_FRESNEL]);
	}
}

//�C���X�^���V���O�`��p�̃e�N�j�b�N(�������C�g)�B
technique IBoneless_Tex_Lim{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main_Instancing(false);
		PixelShader = compile ps_3_0 PS_Main(g_FlagsType[TEX_LIM]);
	}
};

// ZMask�p�̃e�N�X�`�������e�N�j�b�N�B
technique ZMask{
	pass p0{
		VertexShader = compile vs_3_0 VS_Main(false);
		PixelShader = compile ps_3_0 ZMaskPsShader();
	}
}