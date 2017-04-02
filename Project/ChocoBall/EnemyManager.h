#pragma once
#include "EnemyBase.h"
#include "GameObject.h"
#include "ShadowRender.h"
#include "ObjectManager.h"

class CEnemyManager:public CGameObject
{
public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Draw_EM(CCamera*)override;
	void DeleteAll();
	CEnemyManager(){}
	virtual ~CEnemyManager(){
		this->DeleteAll();
	}

	void AddEnemy(EnemyBase* enemy)
	{
		m_Enemys.push_back(enemy);
	}
	void DeleteEnemy(EnemyBase* enemy) {
		SINSTANCE(CObjectManager)->DeleteGameObject(enemy);
	}
	const vector<EnemyBase*>& GetEnemys(){
		return m_Enemys;
	}

	int GetNumEnemy()
	{
		return m_Enemys.size();	//エネミーの数を返す
	}

	void SetStageID(STAGE_ID id){
		m_StageID = id;
	}
	void SetPintoPos(const D3DXVECTOR3& pos)override{
		for (auto enemy : m_Enemys){
			enemy->SetPintoPos(pos);
		}
	}
	void SetPintoWorld(const D3DXMATRIX& mat)override{
		for (auto enemy : m_Enemys){
			enemy->SetPintoWorld(mat);
		}
	}
private:
	vector<EnemyBase*> m_Enemys;
	STAGE_ID m_StageID;
};
