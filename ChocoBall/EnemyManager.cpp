#include "stdafx.h"
#include "EnemyManager.h"
#include "ShadowRender.h"
#include "tkStopwatch.h"

void CEnemyManager::Initialize()
{
	SetAlive(true);
	this->ConfigLight();
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

void CEnemyManager::ConfigLight() {
	if (!m_pLight) {
		m_pLight = new CActreLight;
	}
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(0, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(1, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(2, D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(3, D3DXVECTOR3(0.0f, -1.0f, 0.0f));

	m_pLight->SetDiffuseLightColor(0, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLight->SetDiffuseLightColor(1, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLight->SetDiffuseLightColor(2, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLight->SetDiffuseLightColor(3, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

	m_pLight->SetLimLightDirection(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	m_pLight->SetLimLightColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

	m_pLight->SetAmbientLight(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

	CGameObject::ConfigLight();
}