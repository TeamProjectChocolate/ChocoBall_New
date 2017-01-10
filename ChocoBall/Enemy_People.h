#pragma once
#include "EnemyBase.h"

class CEnemy_People :
	public EnemyBase
{
public:
	CEnemy_People();
	~CEnemy_People();
	void Initialize()override;
	void Update()override;
	virtual void Move() = 0;
	void HitReaction()override;
	void RollingEnemy();
	void ConfigLight()override;
private:
	CIsIntersect m_IsIntersect;		//CIsIntersectのインスタンス
	float m_deadTimer;
};

