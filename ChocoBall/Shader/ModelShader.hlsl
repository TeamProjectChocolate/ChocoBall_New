
//#define ENABLE_NORMAL_MAP		//��`����Ɩ@���}�b�v���L���ɂȂ�B
//#define ENABLE_VERTEX_COLOR	//��`����ƒ��_�J���[���L���ɂȂ�B

float4x4 World;	// ���[���h�ϊ��s��錾
float4x4 View;	// �r���[�ϊ��s��錾
float4x4 Proj;	// �ˉe�ϊ��s��錾
float4x4 Rota;	// ��]�s��B�@�������f���Ɠ����ʂ�����]�����邽�߂ɕK�v
float3 EyePosition;	// ���_
float4x4 LightViewProj;		// ���C�g�r���[�v���W�F�N�V�����s��

float2 g_FarNear;

// �[�x�������ݗp
float4 g_PintoPoint;	// �ǂ̈ʒu�Ƀs���g�����킹�邩(�s��ϊ���̒l)
float4x4 g_PintoWorld; // �s���g�p�̃��[���h�s��
float2 g_DepthFarNear;

float Alpha;		// �����x

#define DIFFUSE_LIGHT_NUM 4		// �f�B�t���[�Y���C�g�̐�
float4 diffuseLightDirection[DIFFUSE_LIGHT_NUM];	// �f�B�t���[�Y���C�g�̕���
float4 diffuseLightColor[DIFFUSE_LIGHT_NUM];	// �f�B�t���[�Y���C�g�̃J���[
float4 ambientLight;							// ����

// �X�L���s��
#define MAX_MATRICES 26
float4x3 g_WorldMatrixArray[MAX_MATRICES]:WORLDMATRIXARRAY;
float g_numBone;		// ���̐�

float4x4 g_CameraRotaInverse;	// �J�����̉�]�s��̋t�s��

float g_luminance;	// �P�x�̌��̗ʂ𒲐����邽�߂̕ϐ�


//int g_MaxInstance;
//#define MAX_INSTANCE g_MaxInstance
//float4x4 g_InstancingWorlds[MAX_INSTANCE]:WORLDMATRIXARRAY;	// �C���X�^���V���O�`��p�̃��[���h�s��z��

texture g_Texture;			// �e�N�X�`��
sampler g_TextureSampler = 
sampler_state{
	Texture = <g_Texture>;
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture g_ShadowMap;	// �V���h�E�}�b�v
sampler g_ShadowMapSampler =
sampler_state{
	Texture = <g_ShadowMap>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
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
#if !defined(ENABLE_VERTEX_COLOR)
	float4	color	: COLOR0;
#endif
	float3  normal  : NORMAL0;	// ���[���h���W�ɂ�����@��?
	float2  uv		: TEXCOORD0;
#if !defined(ENABLE_NORMAL_MAP)
	float3  tangent : TANGENT0;	// �ڃx�N�g��(�|���S�����甭�����@���ƒ��p�Ɍ����x�N�g���A�|���S���ɉ���)
#endif
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
#if !defined(ENABLE_VERTEX_COLOR)
	float4	color	: COLOR0;
#endif
	float2  uv		: TEXCOORD0;
	float3  normal	: TEXCOORD1;
	float4  WorldPos	: TEXCOORD2;	// ���[���h��Ԃł̒��_���W
	float4  ShadowPos	: TEXCOORD3;
#if !defined(ENABLE_NORMAL_MAP)
	float3  tangent :  TEXCOORD4;	// �ڃx�N�g��
#endif
	float4 pos2:TEXCOORD5;
	float4 PintoPoint:TEXCOORD6;
};

struct PS_OUTPUT{
	float4 color:COLOR0;
	float4 depth:COLOR1;
};
// ���_�V�F�[�_
VS_OUTPUT BasicTransform(VS_INPUT In /*���_���(���[�J�����W*/)
{
	//�����ł��낢�뎩�R�ɐG������A���W�ϊ����s�����肷��B

	VS_OUTPUT Screen = (VS_OUTPUT)0;
	float4 pos;
	pos = mul(In.pos, World);	// ���[���h���W�ɕϊ�
	Screen.WorldPos = pos;
	pos = mul(pos, View);			// �r���[���W�ɕϊ�
	pos = mul(pos, Proj);			// �v���W�F�N�V�������W�ɕϊ�
	Screen.pos = pos;
#if !defined(ENABLE_VERTEX_COLOR)
	Screen.color = In.color;
#endif
	Screen.uv = In.uv;
	Screen.normal = mul(In.normal, Rota);		// �@������
#if !defined(ENABLE_NORMAL_MAP)
	Screen.tangent = mul(In.tangent, Rota);		// �ڃx�N�g������
#endif
	Screen.pos2 = Screen.pos;

	float3 PintoPoint = g_PintoPoint.xyz;
	PintoPoint = mul(PintoPoint, g_PintoWorld);
	Screen.PintoPoint = mul(float4(PintoPoint, 1.0f), View);
	Screen.PintoPoint = mul(Screen.PintoPoint, Proj);

	return Screen;	// ���_���(�X�N���[�����W)���X�N���[�����W��Ԃ��Ȃ���΃G���[�ƂȂ��Ă��܂��B
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

	// �u�����h����{�[���̃C���f�b�N�X
	int4 IndexVector = D3DCOLORtoUBYTE4(In.BlendIndices);

	// �u�����h���[�g
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
	// �f�B�t���[�Y���C�g���v�Z
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
		// �X�y�L�����������v�Z
		// ���˃x�N�g�����v�Z
		float3 L = -diffuseLightDirection[i].xyz;
		lig += diffuseLightColor[i] * pow(max(0.0f, dot(L, R)), 10) * diffuseLightColor[i].w;	// �X�y�L�������x
	}
	return lig;
}

/*!
* @brief	�A���t�@�ɖ��ߍ��ދP�x���v�Z�B
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
	// �e�̕`��
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

	// VSM(���U�V���h�E�}�b�v)
	float Variance = shadow_val.y - (shadow_val.x * shadow_val.x);
	return max(shadow_val.x >= depth,Variance + (depth - shadow_val.x));
}

PS_OUTPUT ShadowPixel(VS_OUTPUT In, uniform bool hasNormalMap, uniform bool hasZMask ,uniform bool isIluminance){

	// ��{�`��
	float3 normal;		// �@���}�b�v�ɏ������܂�Ă���@��
#if !defined(ENABLE_NORMAL_MAP)
	if (hasNormalMap){
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
#endif
	{
		normal = In.normal;
	}
	if (hasZMask){
		float4 screenPos = In.WorldPos;
		screenPos = mul(screenPos, View);			// �r���[���W�ɕϊ�
		screenPos = mul(screenPos, Proj);			// �v���W�F�N�V�������W�ɕϊ�
		screenPos.xyz /= screenPos.w;
		screenPos.xy *= float2(0.5f, -0.5f);			//-0.5�`0.5�͈̔͂ɂ���
		screenPos.xy += 0.5f;						//0.0�`1.0�͈̔͂���B
		float4 zmask = tex2D(g_ZMaskSampler, screenPos.xy);
		clip(screenPos.z - zmask.z);
	}

	float4 color = tex2D(g_TextureSampler, In.uv);	// �e�N�X�`����\��t����
	// �f�B�t���[�Y���C�g�̌v�Z
	float4 light = CalcDiffuseLight(normal);
	// �X�y�L�������C�g���v�Z
	light += CalcSpeculerLight(normal, In.WorldPos);
	// �A���r�G���g���C�g�����Z
	light.xyz += ambientLight;

	color *= light;	// �e�N�X�`���̃J���[�ƃ��C�g����Z

	float shadow = vsm(In.ShadowPos, normal);
	color.xyz = (1.0f - shadow) * shadow + shadow * color.xyz;
	//color.xyz *= vsm(In.ShadowPos,normal);

	//if ((shadow_val.z >= depth) <= ((Variance + (depth - shadow_val.z)) / Variance))
	//if (depth > shadow_val.z){
	//	// �e�ɂȂ��Ă���
	//	color.xyz *= shadow_val.xyz;	// �e����������
	//}

	if (isIluminance){
		// ���ɋP�x�𖄂ߍ���
		color.a = CalcLuminance(color.xyz);
	}
	else{
		color.w = Alpha;
	}

	// �[�x�̏�������
	float4 OutDepth;
	{
		if (hasZMask){
			float4 work;
			work.xyz = In.pos2.xyz / In.pos2.w;
			work.xy *= float2(0.5f, -0.5f);			//-0.5�`0.5�͈̔͂ɂ���
			work.xy += 0.5f;						//0.0�`1.0�͈̔͂���B
			float4 zmask = tex2D(g_ZMaskSampler, work.xy);
			clip(work.z - zmask.z);	// ZMask�e�N�X�`������[�x���󂯎���āA�������܂ꂽ�[�x�������̃s�N�Z���Ȃ炻�̃s�N�Z����j������
		}

		// �s���g�����킹��ʒu���v�Z
		float pinto = /*0.5f*/(In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
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

// �s�N�Z���V�F�[�_
PS_OUTPUT TextureShader(VS_OUTPUT In, uniform bool hasNormalMap,uniform bool hasIluminance){
	float3 normal;		// �@���}�b�v�ɏ������܂�Ă���@��
#if !defined(ENABLE_NORMAL_MAP)
	if (hasNormalMap){
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
#endif
	{
		normal = In.normal;
	}
	// �f�B�t���[�Y���C�g�̌v�Z
	float4 light = CalcDiffuseLight(normal);
	// �X�y�L�������C�g���v�Z
	light += CalcSpeculerLight(normal,In.WorldPos);
	// �A���r�G���g���C�g�����Z
	light.xyz += ambientLight;

	float4 color = tex2D(g_TextureSampler, In.uv);	// �e�N�X�`����\��t����
	color *= light;	// �e�N�X�`���̃J���[�ƃ��C�g����Z
	if (hasIluminance){
		color.a = CalcLuminance(color.xyz);
	}
	else{
		color.w = Alpha;
	}

	// �[�x�̏�������
	float4 OutDepth;
	{
		// �s���g�����킹��ʒu���v�Z
		float pinto = /*0.5f*/(In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
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

PS_OUTPUT NoWorkingPixelShader(VS_OUTPUT In, uniform bool hasNormalMap){
	float3 normal;		// �@���}�b�v�ɏ������܂�Ă���@��
#if !defined(ENABLE_NORMAL_MAP)
	if (hasNormalMap){
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
#endif
	{
		normal = In.normal;
	}

	// �f�B�t���[�Y���C�g�̌v�Z
	float4 light = CalcDiffuseLight(normal);
	// �X�y�L�������C�g���v�Z
	light += CalcSpeculerLight(normal,In.WorldPos);
	// �A���r�G���g���C�g�����Z
	light.xyz += ambientLight;
#if !defined(ENABLE_VERTEX_COLOR)
	float4 color = In.color;	// �e�N�X�`����\��t����
#else
	float4 color = 1.0f;	// �e�N�X�`����\��t����
#endif
	color *= light;	// �e�N�X�`���̃J���[�ƃ��C�g����Z
	color.w = Alpha;


	// �[�x�̏�������
	float4 OutDepth;
	{
		// �s���g�����킹��ʒu���v�Z
		float pinto = /*0.5f*/(In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
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

PS_OUTPUT FresnelShader(VS_OUTPUT In, uniform bool hasNormalMap,uniform bool hasShadow,uniform bool hasluminance){
	float3 normal;		// �@���}�b�v�ɏ������܂�Ă���@��
#if !defined(ENABLE_NORMAL_MAP)
	if (hasNormalMap){
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
#endif
	{
		normal = In.normal;
	}

	// �f�B�t���[�Y���C�g�̌v�Z
	float4 light = CalcDiffuseLight(normal);
	// �X�y�L�������C�g���v�Z
	light += CalcSpeculerLight(normal, In.WorldPos);
	// �A���r�G���g���C�g�����Z
	light.xyz += ambientLight;

	// �t�@�[���C�e�B���O���v�Z
	// �@�����J�������W�n�ɕϊ�����
	float3 normalInCamera = mul(normal, g_CameraRotaInverse);
	float fresnel = 1.0f - abs(dot(normalInCamera, float3(0.0f, 0.0f, 1.0f)));
	fresnel *= fresnel;// pow(fresnel, 1.5f);
	float4 color = tex2D(g_TextureSampler, In.uv);	// �e�N�X�`����\��t����
	color *= light;	// �e�N�X�`���̃J���[�ƃ��C�g����Z
	color += fresnel;
	float4 shadow_val = float4(1.0f, 1.0f, 1.0f, 1.0f);

	if (hasShadow){
		//// �e�̕`��
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
		//	// �e�ɂȂ��Ă���
		//	color.xyz *= shadow_val.xyz;
		//}
		float shadow = vsm(In.ShadowPos, normal);
		color.xyz = (1.0f - shadow) * shadow + shadow * color.xyz;
		//color.xyz *= vsm(In.ShadowPos,normal);
	}
	
	if (hasluminance){
		// ���ɋP�x�𖄂ߍ���
		color.a = CalcLuminance(color.xyz);
	}
	else{
		color.w = Alpha;
	}

	// �[�x�̏�������
	float4 OutDepth;
	{
		// �s���g�����킹��ʒu���v�Z
		float pinto = /*0.5f*/(In.PintoPoint.z - g_DepthFarNear.y) / (g_DepthFarNear.x - g_DepthFarNear.y);
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

technique ShadowTec{
	pass p0{
		VertexShader = compile vs_3_0 ShadowVertex();
		PixelShader = compile ps_3_0 ShadowPixel(true, false,false);
	}
};

technique TextureTec{
	pass p0{
		VertexShader = compile vs_3_0 BasicTransform();	// ���_�V�F�[�_
		PixelShader = compile ps_3_0 TextureShader(true,false);		// �s�N�Z���V�F�[�_
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
		VertexShader = compile vs_3_0 BasicTransform();	// ���_�V�F�[�_
		PixelShader = compile ps_3_0 TextureShader(false,false);		// �s�N�Z���V�F�[�_
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

//�C���X�^���V���O�`��p�̃e�N�j�b�N(�t���l��)
technique NonAnimationInstancing_Fresnel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancing(false);
		PixelShader = compile ps_3_0 FresnelShader(false, false, false);
	}
}

//�C���X�^���V���O�`��p�̃e�N�j�b�N(�t���l��)
technique AnimationInstancing_Fresnel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancing(true);
		PixelShader = compile ps_3_0 FresnelShader(false, false, false);
	}
}

//�C���X�^���V���O�`��p�̃e�N�j�b�N(�t���l���A�e����)
technique NonAnimationInstancing_SFresnel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancing(false);
		PixelShader = compile ps_3_0 FresnelShader(false, true, false);
	}
}

//�C���X�^���V���O�`��p�̃e�N�j�b�N(�t���l���A�e����)
technique AnimationInstancing_SFresnel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancing(true);
		PixelShader = compile ps_3_0 FresnelShader(false, true, false);
	}
}