#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "ObjectManager.h"
#include "Chocoball.h"
#include "GameObject.h"
#include "Player.h"
#include "CourceDef.h"
#include "ShadowRender.h"
#include "Enemy_Boss.h"


#define CHOCO_NUM 200	//���ɗ����`���R�̐�

struct SInstancingVertex{
	FLOAT x, y, z, w;
	DWORD color;
	FLOAT u, v;
	int Index;
public:
	void SetIndex(const int& index){
		this->Index = index;
	}
};
//Chocoball���Ǘ�����N���X
class CCBManager : public CGameObject{
public:
	CCBManager(){
		m_CBManagerNum++;
		strcpy(m_pRenderName, "Choco");
	};
	~CCBManager(){
#ifdef NOT_INSTANCING
#else
		SINSTANCE(CShadowRender)->DeleteObjectImidieit(this);
		m_CBManagerNum--;
#endif
	};
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawShadow(CCamera*)override;
	void Draw_EM(CCamera*)override;
	void SetRenderState()override{
		m_RenderingState = RENDER_STATE::Instancing;
	}
	void SetShadowRenderState()override{
		m_ShadowRenderingState = RENDER_STATE::_3D_ShadowSample_I;
	}
	void EM_SetRenderState()override{
		m_EMRenderingState = RENDER_STATE::EM_Sampling_I;
	}

	void ActivateShadowRender()override;

	void SetUpTechnique()override{
#ifdef NOT_EM
		m_pRender->SetUpTechnique("IBoneless_Tex_Lim");
#else
		m_pRender->SetUpTechnique("IBoneless_Tex_Fresnel");
#endif
	}
	void EM_SetUpTechnique()override{
		m_pEMSamplingRender->SetUpTechnique("IBoneless_Tex_Lim");
	}

	void FindCource();
	void NonActivate();

	//�ŏ��̃|�W�V�������擾�ł��܂��B
	D3DXVECTOR3 GetStartPosition()
	{
		return m_pos;
	}


	//�ŏ��̃|�W�V�������Z�b�g���܂��B
	void SetStartPosition(D3DXVECTOR3 pos)
	{
		m_pos = pos;
	}

	//�I�_�̃|�W�V�������Z�b�g���܂��B
	void SetEndPosition(D3DXVECTOR3 Epos)
	{
		m_posG = Epos;
	}

	//�I�_�̃|�W�V�������擾�ł��܂��B
	D3DXVECTOR3 GetEndPosition()
	{
		return m_posG;
	}
	//�`���R�{�[���Ƃ̏Փ˔���B
	void IsHit();
	
	void SetStageID(STAGE_ID id){
		m_StageID = id;
	}
	int GetCourceNo(){
		return m_InitPosOfCourceNo;
	}

	void SetPintoWorld(const D3DXMATRIX& mat)override{
#ifdef NOT_DOF
		for (short idx = 0; idx < CHOCO_NUM; idx++) {
			m_Choco[idx]->SetPintoWorld(mat);
		}
#else
		m_pModel->SetPintoWorld(mat);
#endif
	}
	void SetPintoPos(const D3DXVECTOR3& pos)override{
#ifdef NOT_DOF
		for (short idx = 0; idx < CHOCO_NUM; idx++) {
			m_Choco[idx]->SetPintoPos(pos);
		}
#else
		m_pModel->SetPintoPos(pos);
#endif
	}

	void SetIsBossDamage(bool flg) {
		m_IsBossDamage = flg;
	}

	// �`���R�{�[�������ԍ��Ŕj�􂳂���B
	void OnBurst(float);
private:
	void CreateChocoBall();
private:
	D3DXVECTOR3			m_pos;			//���������ꏊ�̃|�W�V�����B
	D3DXVECTOR3			m_posG;			//����Ă�����(�S�[��)�̃|�W�V�����B
	vector<unique_ptr<CChocoBall>>	m_Choco;
	float				m_interval;		//�C���^�[�o���B
	float				m_timer;		//�^�C�}�[�B
	int					m_InitPosOfCourceNo;// �`���R�{�[�����������ꂽ�ꏊ�̃R�[�X�i���o�[
	CCourceDef m_CourceDef;
	STAGE_ID m_StageID;
	bool m_IsFirst;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	static int m_CBManagerNum;

	CPlayer* m_pPlayer = nullptr;
	CEnemy_Boss* m_pBoss = nullptr;
	bool m_IsBossDamage = true;	// �{�X�Ƀ_���[�W��^�����邩�̃t���O�B
 };