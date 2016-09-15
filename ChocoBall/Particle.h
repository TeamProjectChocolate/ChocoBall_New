#pragma once

#include "GameObject.h"
#include "CourceCamera.h"
#include "Primitive.h"
#include "Random.h"

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

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawDepth(const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&)override;
	void SetupMatrices();
	void InitParticle(CRandom&, CCamera&, const SParticleEmitParameter*, const D3DXVECTOR3&,D3DXVECTOR3);
	// パーティクルに力を加える関数
	void ApplyFource(const D3DXVECTOR3& applyFource){
		this->m_applyFource = applyFource;
	}
	bool GetIsDead(){
		return m_state == EMIT_STATE::DEAD;
	}
	void SetFileName(LPCSTR name){
		CH_ASSERT(strlen(name) <= MAX_FILENAME);
		strcpy(m_pFileName, name);
	}
	D3DXVECTOR3* GetPosRef(){
		return &(m_ParticleData.position);
	}
private:
	CPrimitive m_Primitive;
	D3DXMATRIX mWorldViewProj;
	CCamera* m_camera;
	CRandom* m_random;
	float m_life;
	float m_timer;
	PARTICLE_DATA m_ParticleData;
	float m_rotateZ;
	D3DXVECTOR3 m_addVelocityRandomMargin;	// 速度の積分のときのランダム幅
	bool m_isFade;		// 死ぬときにフェードアウトするかのフラグ
	float m_fadeTime;		// フェードの時間
	EMIT_STATE m_state;	// 状態
	float m_initAlpha;	// 初期アルファ
	bool m_isBillboard;	// ビルボードかどうかのフラグ
	D3DXVECTOR3 m_applyFource;	// 外部から加わる力
	float m_brightness;		// 輝度。ブルームが有効なら強くすると光があふれる
	int m_alphaBlendMode;		// 半透明合成、1加算合成
	float m_deltaTime;
};

