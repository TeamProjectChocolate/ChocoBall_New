#pragma once

class CGameObject;
class CSkinModelData;

#define MAX_FILENAME 255

#define NUM_DIFFUSE_LIGHT 4		// �g�p����f�B�t���[�Y���C�g�̐�

#define GRAVITY 0.6f// �n���̏d�͉����x�B

enum PRIORTY{ CONFIG = 0, EMITTER, PLAYER, ENEMY, OBJECT3D, OBJECT3D_ALPHA, PARTICLE,PARTICLE_ALPHA, OBJECT2D, OBJECT2D_ALPHA, MAX_PRIORTY };

enum GAMEEND_ID{ CONTINUE = 0,CLEAR, OVER };

enum MOVE_STATE{ Wait = 0, Walk, Dash, Vibration, Flow, Fly ,RockOn,Move};

// �V�F�[�_�t�@�C���i�[�p�\����
typedef struct EFFECT_DATA{
	CHAR pFileName[MAX_FILENAME + 1];
	LPD3DXEFFECT Effect;	// �C���^�t�F�[�X�|�C���^
}EFFECT_DATA;

// 3D���f�����i�[�p�\����
typedef struct IMAGE3D{
	CHAR pFileName[MAX_FILENAME + 1];
	CSkinModelData* pModel; 
}IMAGE3D;

// 2D�摜�̑傫���i�[�p�\����
typedef struct TEXTURE_SIZE{
	float x;
	float y;
};

// 2D���f�����i�[�p�\����
typedef struct IMAGE2D{
	CHAR pFileName[MAX_FILENAME + 1];
	LPDIRECT3DTEXTURE9 pTex;
	TEXTURE_SIZE RealSize;
	TEXTURE_SIZE UnRealSize;
}IMAGE2D;


// �I�u�W�F�N�g���W�i�[�\����
typedef struct TRANSEFORM{
	D3DXVECTOR3 position;
	D3DXQUATERNION angle;
	D3DXVECTOR3 scale;
}TRANSFORM;

// �A�j���[�V�����p�s��
typedef struct D3DXFRAME_DERIVED : public D3DXFRAME {
	D3DXMATRIXA16	CombinedTransformationMatrix;	//�����ςݍs��B
};

// �A�j���[�V����3D���f�����i�[�p�\����
typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER {
	LPDIRECT3DTEXTURE9* ppTextures;
	LPD3DXMESH pOrigMesh;
	LPD3DXATTRIBUTERANGE pAttributeTable;
	DWORD NumAttributeGroups;
	DWORD NumInfl;
	LPD3DXBUFFER pBoneCombinationBuf;
	D3DXMATRIX** ppBoneMatrixPtrs;
	D3DXMATRIX* pBoneOffsetMatrices;
	DWORD NumPaletteEntries;
	bool UseSoftwareVP;
	DWORD iAttributeSW;
};

enum EMIT_STATE{RUN = 0,FADEOUT,DEAD};

typedef struct PARTICLE_DATA{
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 gravity;
};

struct SShapeVertex_PT {
	float		pos[4];
	float		uv[2];
};

enum RENDER_STATE{ None = 0, _2D, _3D, _3D_Simple, _3D_ShadowSample, _3D_ShadowSample_I, Bloom, Dof, EM,EM_Sampling ,EM_Sampling_I, Instancing,Instancing_Alpha ,Max};

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

enum PLAYING_STATE{ONCE = 0,REPEAT};

// �t���l�����˂Ŏg�p����A���̂̃^�C�v�B
enum REFRACTIVES{EARTH = 0,CHOCOLATE,GOLD,GLASS,NONE = 999};
// �e���̂̋��ܗ��B
static float g_RefractivesTable[] = {
	1.000293f,
	1.457f,
	0.34f,
	1.51f,
};

// �O���t�B�b�N�e�N�j�b�N�̃X�C�b�`�B
//#define NOT_VSM // ��`�Ń\�t�g�V���h�E���I�t�B
//#define NOT_BLOOM	// ��`�Ńu���[�����I�t�B
#define NOT_DOF	// ��`�Ŕ�ʊE�[�x���I�t�B
//#define NOT_EM		// ��`�Ŋ��}�b�v�I�t�B
//#define NOT_INSTANCING	// ��`�ŃC���X�^���V���O�`��I�t�B