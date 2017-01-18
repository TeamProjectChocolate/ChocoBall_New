
#define DIFFUSE_LIGHT_NUM 4		// �f�B�t���[�Y���C�g�̐�
struct SLight{
	float3	diffuseLightDir[DIFFUSE_LIGHT_NUM];		//�f�B�t���[�Y���C�g�̌����B
	float4  diffuseLightColor[DIFFUSE_LIGHT_NUM];	//�f�B�t���[�Y���C�g�̃J���[�B
	float3	limLightDir;							//�������C�g�̕����B
	float4	limLightColor;							//�������C�g�̐F�B
	float3  ambient;								//�A���r�G���g���C�g�B
};

SLight g_light;		//!<���C�g
float3 g_EyePosition;	// ���_
float3 g_EyeDir;

float g_Refractive;

float4 CalcDiffuseLight(float3 normal){
	float4 lig = 0.0f;
	// �f�B�t���[�Y���C�g���v�Z
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
		// �X�y�L�����������v�Z
		// ���˃x�N�g�����v�Z
		float3 L = -1.0f * (g_light.diffuseLightDir[i]);
		lig += g_light.diffuseLightColor[i] * pow(max(0.0f, dot(L, R)), 10) * g_light.diffuseLightColor[i].w;	// �X�y�L�������x
	}
	return lig;
}

/*!
*@brief	�������C�g���v�Z�B
*/
float3 CalcLimLight(float3 normal)
{
	float lim = 0.0f;
	float baselim = 1.0f - abs(dot(normal, g_EyeDir));
	lim += baselim * max(0.0f, -dot(g_EyeDir, g_light.limLightDir));
	lim = pow(lim, 1.5f);
	return g_light.limLightColor.xyz * lim;
}

// ��������t���l���֘A

textureCUBE g_CubeTex;
samplerCUBE cubeTexSampler =
sampler_state{
	Texture = <g_CubeTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

// ���ܗ��v�Z�B
float CalcRefractive(
	float Refractive1,	//���݌����i��ł��镨�̔��˗��B
	float Refractive2	//�������߂��悤�Ƃ��Ă��镨���̔��˗��B
	) {
	return Refractive1 / Refractive2;	// ���ܗ��Z�o�B
}


//float Fresnel() {
//}

// Sclick�̋ߎ����B
float Schlick(float f0, float u) {
	return f0 + (1.0f - f0) * pow(min(0.0f,1.0f - u), 5);
}

//float Phong() {
//
//}

// ���˃s�N�Z���Ƌ��܃s�N�Z�����t���l�����˗��ō����B
float4 lerp(float4 refract, float4 reflect, float fresnel)
{
	return float4(refract + (reflect - refract)*fresnel);
}

// �P�[�X�P
//// �t���l�����˗��v�Z�B
float CalcFresnel(
	float3 normal,		//���[���h���W�n�ł̖@���B
	float4 pos,			//���[���h���W�n�ł̒��_���W�B
	float4 EyeDir,		//�J�����̌����B 
	float Refractive1,	//���݌����i��ł��镨�̔��˗��B
	float Refractive2	//�������߂��悤�Ƃ��Ă��镨���̔��˗��B
	){
	float Refractive = CalcRefractive(Refractive1, Refractive2);
	//float3 EyeDir = normalize(g_EyeDir);
		//normalize(pos - g_EyePosition);	//�����x�N�g���Z�o�B
	float InRad = abs(acos(dot(EyeDir, normal)));	// ���ˊp�Z�o�B
	float RefractiveRad = InRad * Refractive;	// ���܊p�Z�o�B
	float Rs = (sin(InRad - RefractiveRad) / sin(InRad + RefractiveRad)) * (sin(InRad - RefractiveRad) / sin(InRad + RefractiveRad));
	float Rp = (tan(InRad - RefractiveRad) / tan(InRad + RefractiveRad)) * (tan(InRad - RefractiveRad) / tan(InRad + RefractiveRad));

	return Rs + Rp / 2.0f;		// ���˗���ԋp�B
}

// �P�[�X�Q
//float CalcFresnel(
//	float3 normal,		//���[���h���W�n�ł̖@���B
//	float4 pos,			//���[���h���W�n�ł̒��_���W�B
//	float4 EyeDir,		//�J�����̌����B 
//	float Refractive1,	//���݌����i��ł��镨�̋��ܗ��B
//	float Refractive2	//�������߂��悤�Ƃ��Ă��镨���̋��ܗ��B
//) {
////	float3 EyeDir = normalize(/*pos - g_EyePosition*//*g_EyePosition - pos*/g_EyeDir);	//�����x�N�g���Z�o�B
//	float3 LightDir = normalize(g_light.diffuseLightDir[0]);
//	// �n�[�t�x�N�g�����Z�o�B
//	float3 HalfVec = normalize(LightDir + EyeDir);
//	// �����̑��΋��ܗ��̔䂩��A�t���l�����ˌW��F(0��)�����߂�B
//	float f0 = pow((Refractive1) / (Refractive2), 2);
//	float f = Schlick(f0, dot(LightDir, HalfVec));
//	return f;		// ���˗���ԋp�B
//}
