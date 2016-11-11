#include "stdafx.h"
#include "Chocoball.h"
#include "CBManager.h"
#include "ShadowRender.h"
#include "InstancingRender.h"
#include "ShadowSamplingRender_I.h"
#include "EM_SamplingRender_I.h"

int CCBManager::m_CBManagerNum = 0;

void CCBManager::Initialize()
{
	m_interval = 0.2f;
	m_timer = 0.0f;
	m_numCreate = 0;
	/*for (int i = 0; i < CHOCO_NUM; i++)
	{
	int rate = 100.0f / (rand() % 100+1);
	if (rand() % 2){
	rate *= -1.0f;
	}
	D3DXVECTOR3 pos(GetStartPosition());
	pos.z += fabsf(rate);
	pos.y += rate;
	D3DXVECTOR3 Epos(GetEndPosition());
	Epos.z += fabsf(rate);
	Epos.y += rate;
	m_Choco[i].Initialize(pos, Epos);
	}*/
	SetAlive(true);

	m_IsFirst = true;

	//SInstancingVertex vertices[] = {
	//	{ -1.0f, -1.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f, },
	//	{ -1.0f, 1.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f, },
	//	{ 1.0f, -1.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f, },
	//	{ 1.0f, -1.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f, },
	//	{ -1.0f, 1.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f, },
	//	{ 1.0f, 1.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f, },
	//};
	//
	//(*graphicsDevice()).CreateVertexBuffer(6 * sizeof(SInstancingVertex), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);

#ifdef NOT_INSTANCING
#else
	// ���f���f�[�^�͈�ł������߁A�X�̃`���R���œǂݍ��܂��A�����œǂݍ���
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/ball.x");
	CGameObject::Initialize();
		InitInstancing(m_CBManagerNum * CHOCO_NUM,true);
		SINSTANCE(CShadowRender)->Entry(this);	// �`���R�{�[���̓C���X�^���V���O�`��̂��ߎ��g��o�^����
		m_pModel->m_alpha = 1.0f;
		m_pModel->m_luminance = 0.0f;
		m_pModel->m_Refractive = g_RefractivesTable[REFRACTIVES::CHOCOLATE];
#endif

}

void CCBManager::ActivateShadowRender(){
#ifdef NOT_INSTANCING
#else
	CGameObject::ActivateShadowRender();
	static_cast<CShadowSamplingRender_I*>(m_pShadowRender)->CreateMatrixBuffer(m_CBManagerNum * CHOCO_NUM);
#endif
}

void CCBManager::Update()
{
	//�`���R�{�[���𐶐����Ă����B
	const float deltaTime = 1.0f / 60.0f;
	m_timer += deltaTime;
	if (m_interval < m_timer){
		int createCount = 0;
		while ( m_numCreate < CHOCO_NUM ){
			if (createCount == 10){
				break;
			}
			float rate = 100.0f / (rand() % 100 + 1);
			rate /= 50.0f;
			if (rand() % 2){
				rate *= -1.0f;
			}
			D3DXVECTOR3 pos(GetStartPosition());
			pos.x += rate;
			pos.z += fabsf(rate);
			pos.y += rate;
			D3DXVECTOR3 Epos(GetEndPosition());
			Epos.x += rate;
			Epos.z += fabsf(rate);
			Epos.y += rate;
			m_Choco[m_numCreate].Initialize(pos, Epos);
#ifdef NOT_INSTANCING
			SINSTANCE(CShadowRender)->Entry(&m_Choco[m_numCreate]);
#endif
			createCount++;
			m_numCreate++;
		}
	}
	for (int i = 0; i < m_numCreate; i++)
	{
		m_Choco[i].Update();
	}
}

void CCBManager::Draw()
{
#ifdef NOT_INSTANCING
	bool isBeginDraw = false;

	for (int i = 0; i < m_numCreate; i++){
		if (m_Choco[i].GetAlive()){
			if (!isBeginDraw){
				m_Choco[i].BeginDraw();
				isBeginDraw = true;
			}
			m_Choco[i].Draw();
		}
	}
	if (isBeginDraw){
		m_Choco[m_numCreate - 1].EndDraw();
	}
#else
		// ObjectManager����Draw�֐����Ă΂ꂽ��A�C���X�^���V���O�p�̃����_�[�ɍs��f�[�^��~�ς��Ă���
		m_pRender->SetModelData(m_pModel);
		SetUpTechnique();
		for (int i = 0; i < m_numCreate; i++)
		{
			if (m_Choco[i].GetAlive()){
				static_cast<CInstancingRender*>(m_pRender)->AddWorldMatrix(m_Choco[i].GetModel()->GetWorldMatrix());
				static_cast<CInstancingRender*>(m_pRender)->AddRotationMatrix(m_Choco[i].GetModel()->GetRotation());
			}
		}
#endif
}

void CCBManager::DrawShadow(CCamera* camera){
#ifdef NOT_INSTANCING
#else
	m_pShadowRender->SetModelData(m_pModel);
	m_pShadowRender->SetShadowCamera(camera);
	for (int i = 0; i < m_numCreate; i++)
	{
		if (m_Choco[i].GetAlive()){
			static_cast<CShadowSamplingRender_I*>(m_pShadowRender)->AddWorldMatrix(m_Choco[i].GetModel()->GetWorldMatrix());
		}
	}
#endif
}

void CCBManager::Draw_EM(CCamera* camera){
#ifdef NOT_INSTANCING
	for (int i = 0; i < m_numCreate; i++){
		if (m_Choco[i].GetAlive()){
			m_Choco[i].Draw_EM(camera);
		}
	}
#else
	static_cast<CEM_SamplingRender_I*>(m_pEMSamplingRender)->SetCamera(camera);
	m_pEMSamplingRender->SetModelData(m_pModel);
	EM_SetUpTechnique();
	for (int i = 0; i < m_numCreate; i++)
	{
		if (m_Choco[i].GetAlive()){
			static_cast<CEM_SamplingRender_I*>(m_pEMSamplingRender)->AddWorldMatrix(m_Choco[i].GetModel()->GetWorldMatrix());
			static_cast<CEM_SamplingRender_I*>(m_pEMSamplingRender)->AddRotationMatrix(m_Choco[i].GetModel()->GetRotation());
		}
	}
#endif
}

bool CCBManager::IsHit(D3DXVECTOR3 pos,D3DXVECTOR3 size)
{
	for (int i = 0; i < m_numCreate; i++){
		//�v���C���[�����锠�̍ő�l�ƍŏ��l�����߂�B
		D3DXVECTOR3 MaxSize;//�ő�l
		D3DXVECTOR3 MinSize;//�ŏ��l
		const static float Sphereradius = 0.65f;//�`���R�{�[���̔��a
		size *= 0.5f;
		MaxSize.x = pos.x + size.x + Sphereradius;
		MaxSize.y = pos.y + size.y + Sphereradius;
		MaxSize.z = pos.z + size.z + Sphereradius;

		MinSize.x = pos.x - size.x - Sphereradius;
		MinSize.y = pos.y - size.y - Sphereradius;
		MinSize.z = pos.z - size.z - Sphereradius;
		D3DXVECTOR3 chocoPos = m_Choco[i].GetPos();
		//�v���C���[�Ƀ`���R�{�[���̔��a���𑫂��������ɓ����Ă���e�Ƃ̓����蔻��𒲂ׂ�B
		if (MinSize.x < chocoPos.x&&
			MinSize.y < chocoPos.y&&
			MinSize.z < chocoPos.z&&
			chocoPos.x < MaxSize.x&&
			chocoPos.y < MaxSize.y&&
			chocoPos.z < MaxSize.z)
		{
			return TRUE;
		}
	
	}
	return FALSE;
}

void CCBManager::FindCource(){
	m_CourceDef.SetStageID(m_StageID);
	m_CourceDef.Initialize();
	m_InitPosOfCourceNo = m_CourceDef.FindCource(m_pos).blockNo;
}

void CCBManager::NonActivate(){
#ifdef NOT_INSTANCING
	for (auto& choco : m_Choco){
		choco.OnDestroy();
		choco.SetAlive(false);
		SINSTANCE(CShadowRender)->DeleteObjectImidieit(&choco);
	}
#else
	SINSTANCE(CShadowRender)->DeleteObject(this);
#endif
	SetAlive(false);
}