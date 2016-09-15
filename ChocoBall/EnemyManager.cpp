#include "stdafx.h"
#include "EnemyManager.h"
#include "ShadowRender.h"
#include "tkStopwatch.h"

void CEnemyManager::Initialize()
{
	//for (short i = 0; i < numEnemy; i++)
	//{
	//	Enemy[i]->SetStageID(m_StageID);
	//	Enemy[i]->Initialize();
	//	SINSTANCE(CShadowRender)->Entry(Enemy[i]);
	//}
	SetAlive(true);
}

void CEnemyManager::Update()
{
	for (short i = 0; i < numEnemy; i++)
	{
		Enemy[i]->Update();
		if (Enemy[i]->GetAlive()){
			Enemy[i]->Update();
		}
		else
		{
			DeleteEnemy(Enemy[i]);
			i--;
		}
		
	}
}

void CEnemyManager::Draw()
{
	for (short i = 0; i < numEnemy; i++)
	{
		Enemy[i]->Draw();
	}
}

void CEnemyManager::DrawDepth( const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){
	for (short i = 0; i < numEnemy; i++)
	{
		Enemy[i]->DrawDepth(FarNear,PintoPos,PintoWorld);
	}
}

void CEnemyManager::DeleteAll(){
	//for (int i = 0; i < ENEMY_MAX; i++){
	//	if (Enemy[i] != nullptr){
	//		SAFE_DELETE(Enemy[i]);
	//	}
	//}
	//numEnemy = 0;
}