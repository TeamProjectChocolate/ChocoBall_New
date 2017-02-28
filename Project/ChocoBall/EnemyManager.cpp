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
	for (auto enemy : m_Enemys){
		if (enemy->GetAlive()){
			enemy->Update();
		}
		else
		{
			DeleteEnemy(enemy);
		}
	}
}

void CEnemyManager::Draw()
{
	for (auto enemy : m_Enemys)
	{
		enemy->Draw();
	}
}

void CEnemyManager::Draw_EM(CCamera* camera){
	for (auto enemy : m_Enemys)
	{
		enemy->Draw_EM(camera);
	}
}

void CEnemyManager::DeleteAll(){
	for (auto enemy : m_DeleteObjects)
	{
		SAFE_DELETE(enemy);
	}
	m_DeleteObjects.clear();
	for (auto enemy : m_Enemys)
	{
		SAFE_DELETE(enemy);
	}
	m_Enemys.clear();
}
