#pragma once
#include "GameObject.h"
#include "BulletPhysics.h"
#include "StageTable.h"
#include "C3DObjectRender.h"
#include "Infomation.h"

class CZBufferSphere;

class CField :
	public CGameObject
{
public:
	CField(){
		m_myMotionState = nullptr;
		m_myMotionState = nullptr;
	};
	~CField();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void SetUpTechnique()override{
#ifdef NOT_VSM
		m_pRender->SetUpTechnique("Boneless_Tex_Shadow_ZMask");
#else
		m_pRender->SetUpTechnique("Boneless_Tex_Shadow_VSM_ZMask");
#endif
	}
	void SetStageID(STAGE_ID id){
		m_StageID = id;
	}
	
private:
	//ここからbulletPhysicsの剛体を使用するために必要な変数。
	vector<btBoxShape*>	m_groundShape;	//地面のコリジョン形状。
	vector<btRigidBody*>		m_rigidBody;	//剛体。
	btDefaultMotionState* m_myMotionState;
	STAGE_ID m_StageID;
	CZBufferSphere* m_czbuffersphere;
};

