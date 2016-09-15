#include "stdafx.h"
#include "LevelBuilder.h"
#include "EnemyManager.h"
#include "Field.h"
#include "CollisionType.h"
#include <vector>
#include "CBManager.h"
#include "EnemyLR.h"
#include "StageTable.h"
#include "FallFloor.h"
#include "MoveFloor.h"
#include "ShadowRender.h"
#include "UpFloor.h"
#include "FireJet.h"
#include "SmokeJet.h"


CLevelBuilder::CLevelBuilder()
{
	memset(m_ghostObject, 0, sizeof(m_ghostObject));//ghostobjectの配列を0からメモリ分初期化
	m_IsStage = STAGE_ID::STAGE_NONE;
	m_ChocoWallNum = 0;
	m_FireJetNum = 0;
	m_SmokeJetNum = 0;
}

CLevelBuilder::~CLevelBuilder()
{
	CObjectManager* objMgr = SINSTANCE(CObjectManager);
	for (int i = 0; i < MaxCollision; i++){
		if (m_ghostObject[i]){
			objMgr->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->RemoveCollisionObject(m_ghostObject[i]);
		}
	}
}

void CLevelBuilder::Build(CAudio* pAudio)
{
	int StageID = static_cast<int>(m_IsStage);
	int tableSize = InfoTableSizeArray[StageID];
	SEnemyAndGimmickInfo* pInfo = infoTableArray[StageID];
	CEnemyManager* enemyMgr = SINSTANCE(CObjectManager)->FindGameObject<CEnemyManager>(_T("EnemyManager"));
	for (int i = 0; i < tableSize; i++){

		SEnemyAndGimmickInfo info = pInfo[i];
		if (info.enemyType == EnemyType::EnemyType_LR){
			//敵を生成。
			CEnemyLR* enemy = new CEnemyLR;
			enemy->SetStageID(m_IsStage);
			enemy->Initialize();
			info.pos.x = pInfo[i].pos.x * -1;
			info.pos.z = pInfo[i].pos.z * -1;
			enemy->SetInitPosition(info.pos);
			enemy->Build();
			enemyMgr->AddEnemy(enemy);
			SINSTANCE(CShadowRender)->Entry(enemy);
		}
		else if (info.enemyType == EnemyType::EnemyType_FB){
			//敵を生成。
			CEnemyFB* enemyfb = new CEnemyFB;
			enemyfb->SetStageID(m_IsStage);
			enemyfb->Initialize();
			info.pos.x = pInfo[i].pos.x * -1;
			info.pos.z = pInfo[i].pos.z * -1;
			enemyfb->SetInitPosition(info.pos);	
			enemyfb->Build();
			enemyMgr->AddEnemy(enemyfb);
			SINSTANCE(CShadowRender)->Entry(enemyfb);
		}
		else if (info.enemyType == EnemyType::EnemyType_JUMP){
			//敵を生成。
			CEnemyjamp* enemyjamp = new CEnemyjamp;	
			enemyjamp->SetStageID(m_IsStage);
			enemyjamp->Initialize();
			info.pos.x = pInfo[i].pos.x * -1;
			info.pos.z = pInfo[i].pos.z * -1;
			enemyjamp->SetInitPosition(info.pos);
			enemyjamp->Build();
			enemyMgr->AddEnemy(enemyjamp);
			SINSTANCE(CShadowRender)->Entry(enemyjamp);
		}
		else if (info.enemyType == EnemyType::EnemyType_BULLET){
			//敵を生成。
			CEnemy* enemy = new CEnemy;
			enemy->SetStageID(m_IsStage);
			enemy->Initialize();
			info.pos.x = pInfo[i].pos.x * -1;
			info.pos.z = pInfo[i].pos.z * -1;
			enemy->SetInitPosition(info.pos);
			enemy->Build();
			enemyMgr->AddEnemy(enemy);
			SINSTANCE(CShadowRender)->Entry(enemy);
		}
		else if (info.gimmickType == GimmickType_Chocoball){
			//チョコボールを生成。
			CCBManager* mgr = SINSTANCE(CObjectManager)->GenerationObject<CCBManager>(_T("Mgr"), PRIORTY::OBJECT3D, false);
			m_chocoballMgrList.push_back(mgr);
			D3DXVECTOR3 startPos(-pInfo[i].pos.x, pInfo[i].pos.y, -pInfo[i].pos.z);
			D3DXQUATERNION rot(pInfo[i].rot.x, pInfo[i].rot.y, pInfo[i].rot.z, pInfo[i].rot.w);
			D3DXMATRIX mRot;
			D3DXMatrixRotationQuaternion(&mRot, &rot);
			D3DXVECTOR3 back;
			back.x = -mRot.m[2][0];
			back.y = mRot.m[2][1];
			back.z = -mRot.m[2][2];
			mgr->SetStartPosition(startPos);
			mgr->SetEndPosition(startPos + back);
			mgr->SetStageID(m_IsStage);
			mgr->FindCource();
			mgr->SetAlive(false);
		}
		else if (info.gimmickType == GimmickType_Wall){
			//チョコ壁の生成

			string str = "B_Block";
			char num[100];
			_itoa(m_ChocoWallNum, num, 10);
			str += num;
			CBuildBlock* buildBlock = SINSTANCE(CObjectManager)->GenerationObject<CBuildBlock>(_T(str.c_str()),PRIORTY::OBJECT3D,false);
			buildBlock->Initialize(
				D3DXVECTOR3(-pInfo[i].pos.x, pInfo[i].pos.y, -pInfo[i].pos.z),
				pInfo[i].rot
			);
			m_ChocoWallNum++;
		}
		else if (info.gimmickType == GimmickType_FallFloor){
			//落ちる床だよ。つかって
			FallingFloor* fallfloor = SINSTANCE(CObjectManager)->GenerationObject<FallingFloor>(_T("FallFloor"), PRIORTY::OBJECT3D, false);
			fallfloor->SetAudio(pAudio);
			fallfloor->Initialize(
				D3DXVECTOR3(-pInfo[i].pos.x, pInfo[i].pos.y, -pInfo[i].pos.z),
				pInfo[i].rot,
				D3DXVECTOR3(pInfo[i].scale.x, pInfo[i].scale.y, pInfo[i].scale.z)
			);
			fallfloor->SetMaxMove(pInfo[i].MaxMove);
			//SINSTANCE(CShadowRender)->Entry(fallfloor);
		}
		else if (info.gimmickType == GimmickType_MoveFloor){
			// 動く床
			MoveFloor* movefloor = SINSTANCE(CObjectManager)->GenerationObject<MoveFloor>(_T("movefloor"), PRIORTY::OBJECT3D, false);
			movefloor->SetAudio(pAudio);
			movefloor->Initialize(
				D3DXVECTOR3(-pInfo[i].pos.x, pInfo[i].pos.y, -pInfo[i].pos.z),
				pInfo[i].rot,
				D3DXVECTOR3(pInfo[i].scale.x, pInfo[i].scale.y, pInfo[i].scale.z)
			);
			D3DXQUATERNION rot(pInfo[i].rot.x, pInfo[i].rot.y, pInfo[i].rot.z, pInfo[i].rot.w);
			D3DXMATRIX mRot;
			D3DXMatrixRotationQuaternion(&mRot, &rot);
			D3DXVECTOR3 back;
			back.x = -mRot.m[2][0];
			back.y = -mRot.m[2][1];
			back.z = -mRot.m[2][2];
			movefloor->SetDirection((movefloor->GetPos() + back) - movefloor->GetPos());
			movefloor->SetMaxMove(pInfo[i].MaxMove);
			//SINSTANCE(CShadowRender)->Entry(movefloor);
		}
		else if (info.gimmickType == GimmickType_UpFloor){
			// 上昇床
			CUpFloor* upfloor = SINSTANCE(CObjectManager)->GenerationObject<CUpFloor>(_T("movefloor"), PRIORTY::OBJECT3D, false);
			upfloor->SetAudio(pAudio);
			D3DXQUATERNION rot(-pInfo[i].rot.x, pInfo[i].rot.y, -pInfo[i].rot.z,pInfo[i].rot.w);
			upfloor->Initialize(
				D3DXVECTOR3(-pInfo[i].pos.x, pInfo[i].pos.y, -pInfo[i].pos.z),
				rot,
				D3DXVECTOR3(pInfo[i].scale.x, pInfo[i].scale.y, pInfo[i].scale.z)
				);
			upfloor->SetMaxMove(pInfo[i].MaxMove);
			//SINSTANCE(CShadowRender)->Entry(upfloor);
		}
		else if (info.gimmickType == GimmickType_FireJet){
			// 炎を噴出するギミック
			string str = "firejet";
			char num[100];
			_itoa(m_FireJetNum, num, 10);
			str += num;
			CFireJet* fire = SINSTANCE(CObjectManager)->GenerationObject<CFireJet>(_T(str.c_str()), PRIORTY::OBJECT3D_ALPHA, false);
			fire->SetEmitterName(_T(str.c_str()));
			fire->SetStageID(m_IsStage);
			fire->SetAudio(pAudio);
			fire->Initialize();
			fire->SetPos(D3DXVECTOR3(-pInfo[i].pos.x, pInfo[i].pos.y, -pInfo[i].pos.z));
			D3DXQUATERNION rot(pInfo[i].rot.x, pInfo[i].rot.y, pInfo[i].rot.z, pInfo[i].rot.w);
			D3DXMATRIX mRot;
			D3DXMatrixRotationQuaternion(&mRot, &rot);
			D3DXVECTOR3 back;
			back.x = -mRot.m[2][0];
			back.y = mRot.m[2][1];
			back.z = -mRot.m[2][2];
			D3DXVECTOR3 dir = (fire->GetPos() + back) - fire->GetPos();
			D3DXVec3Normalize(&dir, &dir);
			fire->SetDirection(dir);
			m_FireJetNum++;
		}
		else if (info.gimmickType == GimmickType_SmokeJet){
			// 煙を噴出するギミック
			string str = "smokejet";
			char num[100];
			_itoa(m_FireJetNum, num, 10);
			str += num;
			CSmokeJet* smoke = SINSTANCE(CObjectManager)->GenerationObject<CSmokeJet>(_T(str.c_str()), PRIORTY::OBJECT3D_ALPHA, false);
			smoke->SetEmitterName(_T(str.c_str()));
			smoke->SetStageID(m_IsStage);
			smoke->Initialize();
			smoke->SetPos(D3DXVECTOR3(-pInfo[i].pos.x, pInfo[i].pos.y, -pInfo[i].pos.z));
			D3DXQUATERNION rot(pInfo[i].rot.x, pInfo[i].rot.y, pInfo[i].rot.z, pInfo[i].rot.w);
			D3DXMATRIX mRot;
			D3DXMatrixRotationQuaternion(&mRot, &rot);
			D3DXVECTOR3 back;
			back.x = -mRot.m[2][0];
			back.y = mRot.m[2][1];
			back.z = -mRot.m[2][2];
			D3DXVECTOR3 dir = (smoke->GetPos() + back) - smoke->GetPos();
			D3DXVec3Normalize(&dir, &dir);
			smoke->SetDirection(dir);
			m_SmokeJetNum++;
		}
	}

	//この引数に渡すのはボックスのhalfsizeなので、0.5倍する。
	int arraySize2 = GimmickInfoTableSizeArray[StageID];	//配列の要素数を返す。
	SCollisionInfo* pInfo2 = GimmickinfoTableArray[StageID];
	for (int i = 0; i < arraySize2; i++) {
		SCollisionInfo& collision = pInfo2[i];
		m_GhostShape[i] = new btBoxShape(btVector3(collision.scale.x*0.5f, collision.scale.y*0.5f, collision.scale.z*0.5f));
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(-collision.pos.x, collision.pos.y, -collision.pos.z));
		groundTransform.setRotation(btQuaternion(collision.rotation.x, collision.rotation.y, collision.rotation.z, collision.rotation.w));

		m_ghostObject[i] = new btGhostObject();
		m_ghostObject[i]->activate();
		m_ghostObject[i]->setCollisionShape(m_GhostShape[i]);
		m_ghostObject[i]->setWorldTransform(groundTransform);
		m_ghostObject[i]->setUserIndex(CollisionType_ChocoballTrigger);
		m_ghostObject[i]->setUserPointer(m_chocoballMgrList[i]);

		//ワールドに追加。
		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddCollisionObject(m_ghostObject[i]);
	}
	m_chocoballMgrList.clear();
}
