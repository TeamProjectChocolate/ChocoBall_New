#pragma once
#include "EnemyBase.h"

// 人型雑魚エネミークラス。
class CEnemy_People :
	public EnemyBase
{
public:
	CEnemy_People();
	~CEnemy_People();
	void Initialize()override;
	void Update()override;
	virtual void Move() = 0;
	void HitReaction(D3DXVECTOR3)override;
	void RollingEnemy();
	void ConfigLight()override;
	void SetUpTechnique()override {
		m_pRender->SetUpTechnique("Skin_Tex");
	}
	void EM_SetUpTechnique()override {
		m_pEMSamplingRender->SetUpTechnique("Skin_Tex");
	}

private:
	CIsIntersect m_IsIntersect;		//CIsIntersectのインスタンス
	float m_deadTimer;
};

