#pragma once

class CGameObject;
class CSkinModelData;

#define MAX_FILENAME 255

#define NUM_DIFFUSE_LIGHT 4		// 使用するディフューズライトの数


enum PRIORTY{ CONFIG = 0, EMITTER, PLAYER, BULLET, ENEMY, OBJECT3D, OBJECT3D_ALPHA, PARTICLE,PARTICLE_ALPHA, OBJECT2D, OBJECT2D_ALPHA, MAX_PRIORTY };

enum GAMEEND_ID{ CONTINUE = 0,CLEAR, OVER };

// シェーダファイル格納用構造体
typedef struct EFFECT_DATA{
	CHAR pFileName[MAX_FILENAME + 1];
	LPD3DXEFFECT Effect;	// インタフェースポインタ
}EFFECT_DATA;

// 3Dモデル情報格納用構造体
typedef struct IMAGE3D{
	CHAR pFileName[MAX_FILENAME + 1];
	CSkinModelData* pModel; 
}IMAGE3D;

// 2D画像の大きさ格納用構造体
typedef struct TEXTURE_SIZE{
	float x;
	float y;
};

// 2Dモデル情報格納用構造体
typedef struct IMAGE2D{
	CHAR pFileName[MAX_FILENAME + 1];
	LPDIRECT3DTEXTURE9 pTex;
	TEXTURE_SIZE RealSize;
	TEXTURE_SIZE UnRealSize;
}IMAGE2D;


// オブジェクト座標格納構造体
typedef struct TRANSEFORM{
	D3DXVECTOR3 position;
	D3DXQUATERNION angle;
	D3DXVECTOR3 scale;
}TRANSFORM;

// アニメーション用行列
typedef struct D3DXFRAME_DERIVED : public D3DXFRAME {
	D3DXMATRIXA16	CombinedTransformationMatrix;	//合成済み行列。
};

// アニメーション3Dモデル情報格納用構造体
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

enum RENDER_STATE{ None = 0, _2D, _3D,_3D_Simple,_3D_ShadowSample,_3D_ShadowSample_I, Bloom, Dof, Instancing ,Max};

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540