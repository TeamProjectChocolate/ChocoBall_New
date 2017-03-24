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
	SetAlive(true);

	m_IsFirst = true;

#ifdef NOT_INSTANCING
#else
	// ���f���f�[�^�͈�ł������߁A�X�̃`���R���œǂݍ��܂��A�����œǂݍ���
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/ball.x");
	CGameObject::Initialize();
	InitInstancing(m_CBManagerNum * CHOCO_NUM, true);
	SINSTANCE(CShadowRender)->Entry(this);	// �`���R�{�[���̓C���X�^���V���O�`��̂��ߎ��g��o�^����
	m_pModel->m_alpha = 1.0f;
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = FRESNEL::g_RefractivesTable[FRESNEL::REFRACTIVES::EARTH];
#endif
	// �����蔻����s���I�u�W�F�N�g��o�^�B
	m_pPlayer = SINSTANCE(CObjectManager)->FindGameObject <CPlayer>(_T("Player"));
	m_pBoss = SINSTANCE(CObjectManager)->FindGameObject <CEnemy_Boss>(_T("BossEnemy"));
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
	CreateChocoBall();

	IsHit();

	for (auto itr = m_Choco.begin();itr != m_Choco.end();)
	{
		if ((*itr)->GetAlive()) {
			(*itr)->Update();
			itr++;
		}
		else {
			// �`���R�{�[���̎������s���Ă���΍폜�B
			SINSTANCE(CObjectManager)->DeleteGameObject(*itr);
			itr = m_Choco.erase(itr);
		}
	}

	// �폜�����B
	int no = m_pPlayer->GetNowCourceNo();
	if (no != -1) {
		if (no - m_InitPosOfCourceNo >= 5) {
			this->NonActivate();
		}
	}
}

void CCBManager::CreateChocoBall() {
	if (m_IsFirst) {
		//�`���R�{�[���𐶐����Ă����B
		const float deltaTime = 1.0f / 60.0f;
		m_timer += deltaTime;
		if (m_interval < m_timer) {
			int createCount = 0;
			while (true) {
				if (m_numCreate >= CHOCO_NUM) {
					// �`���R�{�[���̐����ڕW���ɓ��B�B
					m_IsFirst = false;
					break;
				}
				if (createCount == 10) {
					break;
				}
				float rate = 100.0f / (rand() % 100 + 1);
				rate /= 50.0f;
				if (rand() % 2) {
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
				CChocoBall* choco = SINSTANCE(CObjectManager)->GenerationObject<CChocoBall>(_T("Choco"),OBJECT::PRIORTY::OBJECT3D, false);
				choco->SetStageID(m_StageID);
				choco->Initialize(pos, Epos);
				m_Choco.push_back(choco);
#ifdef NOT_INSTANCING
				SINSTANCE(CShadowRender)->Entry(ptr.get());
#endif
				createCount++;
				m_numCreate++;
			}
		}
	}
}

void CCBManager::Draw()
{
#ifdef NOT_INSTANCING
	bool isBeginDraw = false;

	for (int i = 0; i < m_Choco.size(); i++){
		if (m_Choco[i]->GetAlive()){
			if (!isBeginDraw){
				m_Choco[i]->BeginDraw();
				isBeginDraw = true;
			}
			m_Choco[i]->Draw();
		}
	}
	if (isBeginDraw){
		m_Choco[m_Choco.size() - 1]->EndDraw();
	}
#else
		// ObjectManager����Draw�֐����Ă΂ꂽ��A�C���X�^���V���O�p�̃����_�[�ɍs��f�[�^��~�ς��Ă���
		m_pRender->SetModelData(m_pModel);
		SetUpTechnique();
		for (int i = 0; i < m_Choco.size(); i++)
		{
			static_cast<CInstancingRender*>(m_pRender)->AddWorldMatrix(m_Choco[i]->GetModel()->GetWorldMatrix());
			static_cast<CInstancingRender*>(m_pRender)->AddRotationMatrix(m_Choco[i]->GetModel()->GetRotation());
		}
#endif
}

void CCBManager::DrawShadow(CCamera* camera){
#ifdef NOT_INSTANCING
#else
	m_pShadowRender->SetModelData(m_pModel);
	m_pShadowRender->SetShadowCamera(camera);
	for (int i = 0; i < m_Choco.size(); i++)
	{
		static_cast<CShadowSamplingRender_I*>(m_pShadowRender)->AddWorldMatrix(m_Choco[i]->GetModel()->GetWorldMatrix());
	}
#endif
}

void CCBManager::Draw_EM(CCamera* camera){
#ifdef NOT_INSTANCING
	for (int i = 0; i < m_Choco.size(); i++) {
		m_Choco[i]->Draw_EM(camera);
	}
#else
	static_cast<CEM_SamplingRender_I*>(m_pEMSamplingRender)->SetCamera(camera);
	m_pEMSamplingRender->SetModelData(m_pModel);
	EM_SetUpTechnique();
	for (int i = 0; i < m_Choco.size(); i++)
	{
		static_cast<CEM_SamplingRender_I*>(m_pEMSamplingRender)->AddWorldMatrix(m_Choco[i]->GetModel()->GetWorldMatrix());
		static_cast<CEM_SamplingRender_I*>(m_pEMSamplingRender)->AddRotationMatrix(m_Choco[i]->GetModel()->GetRotation());
	}
#endif
}

void CCBManager::OnTriggerStay(const btCollisionObject* pCollision) {
	if (pCollision->getUserIndex() == static_cast<int>(Collision::Type::Player)) {
		// �`���R�{�[�������J�n�B
		this->Initialize();
		this->SetAlive(true);
		// �`���R�{�[���g���K�[�̃R���W�������R���W�������[���h���珜�O�B
		m_CollisionObject->RemoveWorld();
	}
}

void CCBManager::IsHit()
{
	bool IsPlayerHit = false;
	bool isBossHit = false;
	for (int i = 0; i < m_Choco.size(); i++) {
		if(!IsPlayerHit){
			if (m_IsBossDamage) {
				// �v���C���[�p����B
				D3DXVECTOR3 MaxSize;//�ő�l
				D3DXVECTOR3 MinSize;//�ŏ��l
				const static float Sphereradius = 0.25f;//�`���R�{�[���̔��a
				D3DXVECTOR3 size = m_pPlayer->GetSize();
				size *= 0.5f;
				D3DXVECTOR3 pos = m_pPlayer->GetPos();
				MaxSize.x = pos.x + size.x + Sphereradius;
				MaxSize.y = pos.y + size.y + Sphereradius;
				MaxSize.z = pos.z + size.z + Sphereradius;

				MinSize.x = pos.x - size.x - Sphereradius;
				MinSize.y = pos.y - size.y - Sphereradius;
				MinSize.z = pos.z - size.z - Sphereradius;
				D3DXVECTOR3 chocoPos = m_Choco[i]->GetPos();
				//�{�b�N�X�R���C�_�[�Ƀ`���R�{�[���̔��a���𑫂�����ŁA�Փ˂��Ă���`���R�{�[���Ƃ̓����蔻��𒲂ׂ�B
				if (MinSize.x < chocoPos.x&&
					MinSize.y < chocoPos.y&&
					MinSize.z < chocoPos.z&&
					chocoPos.x < MaxSize.x&&
					chocoPos.y < MaxSize.y&&
					chocoPos.z < MaxSize.z)
				{
					m_pPlayer->ChocoHit();
					IsPlayerHit = true;
				}
			}
		}
		if (m_pBoss) {
			if (!isBossHit) {
				// �{�X�p�̓�����
				if (m_IsBossDamage) {
					D3DXVECTOR3 MaxSize;//�ő�l
					D3DXVECTOR3 MinSize;//�ŏ��l
					const static float Sphereradius = 0.25f;//�`���R�{�[���̔��a
					D3DXVECTOR3 size = m_pBoss->GetSize();
					size *= 0.5f;
					D3DXVECTOR3 pos = m_pBoss->GetPos();
					MaxSize.x = pos.x + size.x + Sphereradius;
					MaxSize.y = pos.y + size.y + Sphereradius;
					MaxSize.z = pos.z + size.z + Sphereradius;

					MinSize.x = pos.x - size.x - Sphereradius;
					MinSize.y = pos.y - size.y - Sphereradius;
					MinSize.z = pos.z - size.z - Sphereradius;
					D3DXVECTOR3 chocoPos = m_Choco[i]->GetPos();
					//�{�b�N�X�R���C�_�[�Ƀ`���R�{�[���̔��a���𑫂�����ŁA�Փ˂��Ă���`���R�{�[���Ƃ̓����蔻��𒲂ׂ�B
					if (MinSize.x < chocoPos.x&&
						MinSize.y < chocoPos.y&&
						MinSize.z < chocoPos.z&&
						chocoPos.x < MaxSize.x&&
						chocoPos.y < MaxSize.y&&
						chocoPos.z < MaxSize.z)
					{
						m_pBoss->ChocoHit(this);
						isBossHit = true;
					}
				}
			}
		}
	}
}

void CCBManager::OnBurst(float DeathTime) {
	for (int i = 0; i < m_Choco.size(); i++)
	{
		// ���S����܂ł̎��Ԃ������_���ł΂炷�B
		float TimeOffset = 0.07f;	// ���S���Ԃ����炷�Ԋu�̒P�ʁB
		int rnd = rand() % 8;
		m_Choco[i]->SetDeathTime(DeathTime + (TimeOffset * static_cast<float>(rnd)));
		m_Choco[i]->SetIsBurst(true);
	}
}

void CCBManager::FindCource(){
	m_CourceDef.SetStageID(m_StageID);
	m_CourceDef.Initialize();
	m_InitPosOfCourceNo = m_CourceDef.FindCource(m_pos).blockNo;
}

void CCBManager::NonActivate(){
#ifdef NOT_INSTANCING
	for (auto choco : m_Choco){
		choco->SetAlive(false);
		SINSTANCE(CShadowRender)->DeleteObjectImidieit(choco.get());
	}
#else
	for (auto choco : m_Choco) {
		// ���݂���`���R�{�[�����폜�B
		SINSTANCE(CObjectManager)->DeleteGameObject(choco);
	}
	m_Choco.clear();
	// �������폜����B
	SINSTANCE(CShadowRender)->DeleteObject(this);
#endif
	SetAlive(false);
}