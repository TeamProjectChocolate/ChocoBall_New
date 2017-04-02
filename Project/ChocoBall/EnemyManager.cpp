#include "stdafx.h"
#include "EnemyManager.h"
#include "ShadowRender.h"
#include "tkStopwatch.h"

void CEnemyManager::Initialize()
{
	SetAlive(true);
}

void CEnemyManager::Update()
{
	for (auto itr = m_Enemys.begin(); itr != m_Enemys.end();) {
		if (!((*itr)->GetAlive())){
			DeleteEnemy(*itr);
			itr = m_Enemys.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void CEnemyManager::Draw()
{
}

void CEnemyManager::Draw_EM(CCamera* camera){
}

void CEnemyManager::DeleteAll(){
	for (auto enemy : m_Enemys)
	{
		SINSTANCE(CObjectManager)->DeleteGameObject(enemy);
	}
	m_Enemys.clear();
}
