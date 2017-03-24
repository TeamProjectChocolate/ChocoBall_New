#pragma once

class CSkinModelData;

#define MAX_FILENAME 255

#define NUM_DIFFUSE_LIGHT 4		// 使用するディフューズライトの数

#define GRAVITY 0.6f// 地球の重力加速度。

// オブジェクト管理優先度。
namespace OBJECT{
	enum PRIORTY { CONFIG = 0, EMITTER, PLAYER, ENEMY, OBJECT3D, OBJECT3D_ALPHA, PARTICLE, PARTICLE_ALPHA, OBJECT2D, OBJECT2D_ALPHA, MAX_PRIORTY };
}

// ゲーム状態。
namespace GAMEEND{
	enum ID { CONTINUE = 0, CLEAR, OVER };
}

// キャラクターステート。
namespace MOVE{
	enum STATE { Wait = 0, Walk, Dash, Vibration, Flow, Fly, RockOn, Move };
}

// シェーダー管理用。
namespace EFFECT{
	// シェーダファイル格納用構造体
	typedef struct DATA {
		CHAR pFileName[MAX_FILENAME + 1];
		LPD3DXEFFECT Effect;	// インタフェースポインタ
	}DATA;
}

// モデル管理用。
namespace MODEL{
	// 3Dモデル情報格納用構造体
	typedef struct IMAGE3D {
		CHAR pFileName[MAX_FILENAME + 1];
		CSkinModelData* pModel;
	}IMAGE3D;

	// 2D画像の大きさ格納用構造体
	typedef struct TEXTURE_SIZE {
		float x;
		float y;
	};

	// 2Dモデル情報格納用構造体
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
	// 座標格納構造体。
	typedef struct TRANSFORM{
		D3DXVECTOR3 position;
		D3DXQUATERNION angle;
		D3DXVECTOR3 scale;
		// 初期化関数。
		void Identity() {
			position = Vector3::Zero;
			angle = Quaternion::Identity;
			scale = Vector3::One;
			//*this = identity;
		};
	//private:
		//// 初期化値。
		//static const TRANSFORM identity;
	}TRANSFORM;
	//const TRANSFORM TRANSFORM::identity = { D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f),D3DXVECTOR3(1.0f, 1.0f, 1.0f) };
}

// アニメーション関連。
namespace ANIMATION{
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

	// 再生タイプ。
	enum PLAYING_STATE { ONCE = 0, REPEAT };
}

// パーティクル関連。
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

// レンダー関連。
namespace RENDER{
	enum TYPE { None = 0, _2D, _3D, _3D_Simple, _3D_ShadowSample, _3D_ShadowSample_I, Bloom, Dof, EM, EM_Sampling, EM_Sampling_I, Instancing, Instancing_Alpha, Collision, Max };
}

// 画面サイズ。
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

// フレネル関連。
namespace FRESNEL{
	// フレネル反射で使用する、物体のタイプ。
	enum REFRACTIVES { EARTH = 0, CHOCOLATE, GOLD, GLASS, NONE = 999 };
	// 各物体の屈折率。
	static float g_RefractivesTable[] = {
		1.000293f,
		1.457f,
		0.34f,
		1.51f,
	};
}


// グラフィックテクニックのスイッチ。
//#define NOT_VSM // 定義でソフトシャドウをオフ。
//#define NOT_BLOOM	// 定義でブルームをオフ。
#define NOT_DOF	// 定義で被写界深度をオフ。
//#define NOT_EM		// 定義で環境マップオフ。

// デバッグ用。
//#define REREASE_TEST	// 定義で各種コマンドなどがオンになる。
//#define TEST_EM	// 定義で環境マップをテスト用の挙動にする。
//#define NOT_INSTANCING	// 定義でインスタンシング描画オフ。