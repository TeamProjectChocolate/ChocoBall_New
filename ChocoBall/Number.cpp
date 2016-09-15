#include "stdafx.h"
#include "Number.h"
#include "C2DImage.h"

CNumber::CNumber(){
	m_value = 0;
}
void CNumber::Initialize(){
	UseModel<C2DImage>();
	m_pModel->SetFileName("image\\Number.png");
	CGameObject::Initialize();
	m_transform.position = D3DXVECTOR3(230.0f, 50.0f, 0.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(50, 80, 1);
	m_pModel->m_alpha = 1.0f;
	SetAlive(true);
	m_pModel->SetSplit(D3DXVECTOR2(10.0f,1.0f));
	m_pModel->SetNow(D3DXVECTOR2(0.0f, 0.0f));
	m_value = 0;
};
CNumber::~CNumber()
{
}
void CNumber::Update(){
	
}

void CNumber::Draw(){
	string value = to_string(m_value);
	if (m_pModel->GetImage_2D()->pTex != NULL){

		m_pModel->SetNow(D3DXVECTOR2(0.0f,0.0f));
		D3DXVECTOR2 Now = m_pModel->GetNow();
		for (int idx = value.length() - 1, offset = 0; idx >= 0; idx--, offset++){
			D3DXVECTOR3 OldPos = m_transform.position;	// 元のポジションを確保
			float width = m_pModel->GetImage_2D()->UnRealSize.x / NUMBER_NUM;
			char now[] = {'0','\0'};
			now[0] = value[idx];	
			int num = atoi(now);
			Now.x = static_cast<float>(num);
			m_pModel->SetNow(Now);
			m_transform.position = D3DXVECTOR3(OldPos.x - (width * offset), OldPos.y, OldPos.z);	// 桁が上がればポジションをずらす
			CGameObject::Update();
			CGameObject::Draw();
			m_transform.position = OldPos;	// 退避しておいたポジションを元に戻す
		}
	}
}
