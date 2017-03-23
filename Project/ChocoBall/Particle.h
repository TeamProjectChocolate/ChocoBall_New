#pragma once

#include "GameObject.h"
#include "CourceCamera.h"
#include "Primitive.h"
#include "Random.h"
#include "GraphicsDevice.h"

class CRandom;
class CCourceCamera;
struct SParticleEmitParameter;

class CParticle:public CGameObject
{
public:
	CParticle();
	~CParticle();
	void SetUpTechnique()override{
		if (m_alphaBlendMode == 0){
			m_pRender->SetUpTechnique("BasicTec");
		}
		else{
			m_pRender->SetUpTechnique("ColorTexPrimAdd");
		}
	}
	void EM_SetUpTechnique()override{
		if (m_alphaBlendMode == 0){
			m_pEMSamplingRender->SetUpTechnique("BasicTec");
		}
		else{
			m_pEMSamplingRender->SetUpTechnique("ColorTexPrimAdd");
		}
	}


	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Draw_EM(CCamera*)override;
	void SetupMatrices();
	void InitParticle(CRandom&, CCamera&, const SParticleEmitParameter*, const D3DXVECTOR3&,D3DXVECTOR3);
	// パーティクルに力を加える関数
	void ApplyFource(const D3DXVECTOR3& applyFource){
		this->m_applyFource = applyFource;
	}
	void SetFileName(LPCSTR name){
		CH_ASSERT(strlen(name) <= MAX_FILENAME);
		strcpy(m_pFileName, name);
	}
public:
// 静的メンバ関数
	// パーティクルクラスで使用するプリミティブ生成。
	static void CreatePrimitive();
private:
	D3DXMATRIX mWorldViewProj;
	CCamera* m_camera;
	CRandom* m_random;
	float m_life;
	float m_timer;
	PARTICLE::DATA m_ParticleData;
	float m_rotateZ;
	D3DXVECTOR3 m_addVelocityRandomMargin;	// 速度の積分のときのランダム幅
	bool m_isFade;		// 死ぬときにフェードアウトするかのフラグ
	float m_fadeTime;		// フェードの時間
	PARTICLE::EMIT_STATE m_state;	// 状態
	float m_initAlpha;	// 初期アルファ
	bool m_isBillboard;	// ビルボードかどうかのフラグ
	D3DXVECTOR3 m_applyFource;	// 外部から加わる力
	float m_brightness;		// 輝度。ブルームが有効なら強くすると光があふれる
	int m_alphaBlendMode;		// 半透明合成、1加算合成
	float m_deltaTime;

	// 板ポリのサイズ。
	float m_width;
	float m_hight;
	D3DXVECTOR4 m_uv;	// 板ポリのUV。
private:
	// 静的メンバ変数。
	static unique_ptr<CPrimitive> m_Primitive;	// このクラスで使用するプリミティブは全く同じ頂点情報を使用するため1つでいい。
};

