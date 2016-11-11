#pragma once
#include "Enemy.h"
#include "EnemyLR.h"
#include "EnemyFB.h"
#include "Enemyjamp.h"
#include "GameObject.h"
#include "ShadowRender.h"

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

	void ConfigLight()override;
	void AddEnemy(EnemyBase* enemy)
	{
		enemy->SetLight(m_pLight);
		m_Enemys.push_back(enemy);
	}
	void DeleteEnemy(EnemyBase* enemy)
	{
		enemy->OnDestroy();
		m_DeleteObjects.push_back(enemy);
	}

	void ExcuteDeleteObjects(){
		int size = m_DeleteObjects.size();
		for (int idx = 0; idx < size; idx++){
			for (auto itr = m_Enemys.begin(); itr != m_Enemys.end();){
				if (m_DeleteObjects[idx] == *itr){
					SAFE_DELETE(*itr);
					itr = m_Enemys.erase(itr);
					break;
				}
				else{
					itr++;
				}
			}
		}
		m_DeleteObjects.clear();
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
	vector<EnemyBase*> m_DeleteObjects;
};
