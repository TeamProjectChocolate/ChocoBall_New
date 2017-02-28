#pragma once
#include "stdafx.h"
#include "MapTable.h"
#include "Field.h"
#include "ZBufferSphere.h"

// �}�b�v�ɔz�u����I�u�W�F�N�g�Ǘ��N���X�B
// �����݂̓{�X��p�}�b�v���\�z���邱�Ƃɂ̂ݎg�p���Ă���B

class CMapObjectManager : public CGameObject{
public:
	CMapObjectManager();
	~CMapObjectManager();
	void Initialize()override;
	void Build();
	void Update()override;
	void Draw()override;
	void DrawShadow(CCamera*)override;
	void Draw_EM(CCamera*)override;
	void Is_DrawShadow_Use_Horizon()override;
	void SetStageID(STAGE_ID id) {
		m_StageID = id;
	}
	void SetPintoWorld(const D3DXMATRIX& mat)override;
	void SetPintoPos(const D3DXVECTOR3& pos)override;
private:
	STAGE_ID m_StageID;
	vector<CField*> m_MapObjects;
	CZBufferSphere* m_czbuffersphere;
	//��������bulletPhysics�̍��̂��g�p���邽�߂ɕK�v�ȕϐ��B
	vector<btBoxShape*>	m_groundShape;	//�n�ʂ̃R���W�����`��B
	vector<btRigidBody*>		m_rigidBody;	//���́B
	btDefaultMotionState* m_myMotionState;
};
