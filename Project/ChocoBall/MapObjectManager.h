#pragma once
#include "stdafx.h"
#include "MapTable.h"
#include "Field.h"
#include "ZBufferSphere.h"

// マップに配置するオブジェクト管理クラス。
// ※現在はボス戦用マップを構築することにのみ使用している。

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
	//ここからbulletPhysicsの剛体を使用するために必要な変数。
	vector<btBoxShape*>	m_groundShape;	//地面のコリジョン形状。
	vector<btRigidBody*>		m_rigidBody;	//剛体。
	btDefaultMotionState* m_myMotionState;
};
