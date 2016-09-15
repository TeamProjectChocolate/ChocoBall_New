#pragma once
#include "stdafx.h"
#include "Bullet.h"


class CEnemyBullet
{
public:
	CEnemyBullet();
	~CEnemyBullet();
	void Initialize();
	void Update();
	void Draw();
	inline void OnDestroy(){
		SINSTANCE(CObjectManager)->DeleteGameObject(m_enemybullet);
		m_enemybullet = nullptr;
	}
	void Build();
	void BulletPlayerCollision();
	void SetDir(D3DXVECTOR4 dir){
		m_enemybullet->SetDir(dir);
	}
	void SetPos(D3DXVECTOR3 pos){
		m_enemybullet->SetPos(pos);
	}
	D3DXVECTOR3 GetPos(){
		return m_enemybullet->GetPos();
	}
	void SetStartPos(const D3DXVECTOR3& pos){
		m_StartPos = pos;
	}
	const D3DXVECTOR3& GetStartPos(){
		return m_StartPos;
	}
	float GetRange(){
		return m_Range;
	}
	void SetBulletSpeed(float s){
		m_enemybullet->SetBulletSpeed(s);
	}
	
private:
	Bullet* m_enemybullet;
	D3DXVECTOR3 m_StartPos;
	float m_Range;
};

