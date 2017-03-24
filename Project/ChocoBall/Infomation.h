#pragma once

class CSkinModelData;

#define MAX_FILENAME 255

#define NUM_DIFFUSE_LIGHT 4		// �g�p����f�B�t���[�Y���C�g�̐�

#define GRAVITY 0.6f// �n���̏d�͉����x�B

// �I�u�W�F�N�g�Ǘ��D��x�B
namespace OBJECT{
	enum PRIORTY { CONFIG = 0, EMITTER, PLAYER, ENEMY, OBJECT3D, OBJECT3D_ALPHA, PARTICLE, PARTICLE_ALPHA, OBJECT2D, OBJECT2D_ALPHA, MAX_PRIORTY };
}

// �Q�[����ԁB
namespace GAMEEND{
	enum ID { CONTINUE = 0, CLEAR, OVER };
}

// �L�����N�^�[�X�e�[�g�B
namespace MOVE{
	enum STATE { Wait = 0, Walk, Dash, Vibration, Flow, Fly, RockOn, Move };
}

// �V�F�[�_�[�Ǘ��p�B
namespace EFFECT{
	// �V�F�[�_�t�@�C���i�[�p�\����
	typedef struct DATA {
		CHAR pFileName[MAX_FILENAME + 1];
		LPD3DXEFFECT Effect;	// �C���^�t�F�[�X�|�C���^
	}DATA;
}

// ���f���Ǘ��p�B
namespace MODEL{
	// 3D���f�����i�[�p�\����
	typedef struct IMAGE3D {
		CHAR pFileName[MAX_FILENAME + 1];
		CSkinModelData* pModel;
	}IMAGE3D;

	// 2D�摜�̑傫���i�[�p�\����
	typedef struct TEXTURE_SIZE {
		float x;
		float y;
	};

	// 2D���f�����i�[�p�\����
	typedef struct IMAGE2D {
		CHAR pFileName[MAX_FILENAME + 1];
		LPDIRECT3DTEXTURE9 pTex = nullptr;
		TEXTURE_SIZE RealSize;
		TEXTURE_SIZE UnRealSize;
	}IMAGE2D;
}

namespace Vector3 {
	const D3DXVECTOR3 Zero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 One = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	const D3DXVECTOR3 One_Minus = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);

	const D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	const D3DXVECTOR3 Down = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	const D3DXVECTOR3 Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 Left = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	const D3DXVECTOR3 Back = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	const D3DXVECTOR3 AxisX = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	const D3DXVECTOR3 AxisY = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 AxisZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

namespace Quaternion {
	const D3DXQUATERNION Identity = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
}

namespace SH_ENGINE{
	// ���W�i�[�\���́B
	typedef struct TRANSFORM{
		D3DXVECTOR3 position;
		D3DXQUATERNION angle;
		D3DXVECTOR3 scale;
		// �������֐��B
		void Identity() {
			position = Vector3::Zero;
			angle = Quaternion::Identity;
			scale = Vector3::One;
			//*this = identity;
		};
	//private:
		//// �������l�B
		//static const TRANSFORM identity;
	}TRANSFORM;
	//const TRANSFORM TRANSFORM::identity = { D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f) };
}

// �A�j���[�V�����֘A�B
namespace ANIMATION{
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

	// �Đ��^�C�v�B
	enum PLAYING_STATE { ONCE = 0, REPEAT };
}

// �p�[�e�B�N���֘A�B
namespace PARTICLE{
	enum EMIT_STATE { RUN = 0, FADEOUT, DEAD };

	typedef struct DATA {
		D3DXVECTOR3 position;
		D3DXVECTOR3 velocity;
		D3DXVECTOR3 gravity;
	};
}

namespace PRIMITIVE {
	struct SShapeVertex_PT {
		D3DXVECTOR4	pos;
		D3DXVECTOR2	uv;
	};
}

// �����_�[�֘A�B
namespace RENDER{
	enum TYPE { None = 0, _2D, _3D, _3D_Simple, _3D_ShadowSample, _3D_ShadowSample_I, Bloom, Dof, EM, EM_Sampling, EM_Sampling_I, Instancing, Instancing_Alpha, Collision, Max };
}

// ��ʃT�C�Y�B
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

// �t���l���֘A�B
namespace FRESNEL{
	// �t���l�����˂Ŏg�p����A���̂̃^�C�v�B
	enum REFRACTIVES { EARTH = 0, CHOCOLATE, GOLD, GLASS, NONE = 999 };
	// �e���̂̋��ܗ��B
	static float g_RefractivesTable[] = {
		1.000293f,
		1.457f,
		0.34f,
		1.51f,
	};
}


// �O���t�B�b�N�e�N�j�b�N�̃X�C�b�`�B
//#define NOT_VSM // ��`�Ń\�t�g�V���h�E���I�t�B
//#define NOT_BLOOM	// ��`�Ńu���[�����I�t�B
#define NOT_DOF	// ��`�Ŕ�ʊE�[�x���I�t�B
//#define NOT_EM		// ��`�Ŋ��}�b�v�I�t�B

// �f�o�b�O�p�B
//#define REREASE_TEST	// ��`�Ŋe��R�}���h�Ȃǂ��I���ɂȂ�B
//#define TEST_EM	// ��`�Ŋ��}�b�v���e�X�g�p�̋����ɂ���B
//#define NOT_INSTANCING	// ��`�ŃC���X�^���V���O�`��I�t�B