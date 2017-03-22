#include "stdafx.h"
#include "LevelBuilder.h"
#include "EnemyManager.h"
#include "EnemyLR.h"
#include "Enemy.h"
#include "EnemyFB.h"
#include "Enemyjamp.h"
#include "Field.h"
#include "CollisionType.h"
#include <vector>
#include "CBManager.h"
#include "StageTable.h"
#include "FallFloor.h"
#include "MoveFloor.h"
#include "ShadowRender.h"
#include "UpFloor.h"
#include "FireJet.h"
#include "SmokeJet.h"
#include "Enemy_Boss.h"
#include "DivisionWall.h"
#include "MapObjectManager.h"

CLevelBuilder::CLevelBuilder()
{
	m_IsStage = STAGE_ID::STAGE_NONE;
	m_ChocoWallNum = 0;
	m_FireJetNum = 0;
	m_SmokeJetNum = 0;
}

CLevelBuilder::~CLevelBuilder()
{
	m_DivisionWalls.clear();
}

void CLevelBuilder::Build(CAudio* pAudio)
{
	int StageID = static_cast<int>(m_IsStage);
	int tableSize = InfoTableSizeArray[StageID];
	SEnemyAndGimmickInfo* pInfo = infoTableArray[StageID];
	CEnemyManager* enemyMgr = SINSTANCE(CObjectManager)->FindGameObject<CEnemyManager>(_T("EnemyManager"));
	for (int i = 0; i < tableSize; i++){
		SEnemyAndGimmickInfo info = pInfo[i];
		if (tableSize == 1) {
			if (info.enemyType == EnemyType::EnemyType_None && info.gimmickType == GimmickType::GimmickType_None) {
				// �G�l�~�[���M�~�b�N���z�u����Ă��Ȃ��B
				break;
			}
		}

		if (info.enemyType == EnemyType::EnemyType_LR){
			//�G�𐶐��B
			CEnemyLR* enemy = new CEnemyLR;
			enemy->SetStageID(m_IsStage);
			enemy->Initialize();
			info.pos.x = pInfo[i].pos.x * -1;
			info.pos.z = pInfo[i].pos.z * -1;
			enemy->SetInitPosition(info.pos);
			enemy->Build();
			enemyMgr->AddEnemy(enemy);
		}
		else if (info.enemyType == EnemyType::EnemyType_FB){
			//�G�𐶐��B
			CEnemyFB* enemyfb = new CEnemyFB;
			enemyfb->SetStageID(m_IsStage);
			enemyfb->Initialize();
			info.pos.x = pInfo[i].pos.x * -1;
			info.pos.z = pInfo[i].pos.z * -1;
			enemyfb->SetInitPosition(info.pos);	
			enemyfb->Build();
			enemyMgr->AddEnemy(enemyfb);
		}
		else if (info.enemyType == EnemyType::EnemyType_JUMP){
			//�G�𐶐��B
			CEnemyjamp* enemyjamp = new CEnemyjamp;	
			enemyjamp->SetStageID(m_IsStage);
			enemyjamp->Initialize();
			info.pos.x = pInfo[i].pos.x * -1;
			info.pos.z = pInfo[i].pos.z * -1;
			enemyjamp->SetInitPosition(info.pos);
			enemyjamp->Build();
			enemyMgr->AddEnemy(enemyjamp);
		}
		else if (info.enemyType == EnemyType::EnemyType_BULLET){
			//�G�𐶐��B
			CEnemy* enemy = new CEnemy;
			enemy->SetStageID(m_IsStage);
			enemy->Initialize();
			info.pos.x = pInfo[i].pos.x * -1;
			info.pos.z = pInfo[i].pos.z * -1;
			enemy->SetInitPosition(info.pos);
			enemy->Build();
			enemyMgr->AddEnemy(enemy);
		}
		else if (info.enemyType == EnemyType::EnemyType_Boss) {
			//�G�𐶐��B
			// �{�X�͓��ʂɃG�l�~�[�}�l�[�W���[�ł͂Ȃ��I�u�W�F�N�g�}�l�[�W���[�ɓo�^�B
			CEnemy_Boss* boss = SINSTANCE(CObjectManager)->GenerationObject<CEnemy_Boss>(_T("BossEnemy"), OBJECT::PRIORTY::ENEMY, false);
			boss->Initialize();
			info.pos.x = pInfo[i].pos.x * -1;
			info.pos.z = pInfo[i].pos.z * -1;
			boss->SetInitPosition(info.pos);
			boss->SetAudio(pAudio);
			boss->SetDivisionWall(&m_DivisionWalls);
		}
		else if (info.gimmickType == GimmickType_Chocoball){
			//�`���R�{�[���𐶐��B
			CCBManager* mgr = SINSTANCE(CObjectManager)->GenerationObject<CCBManager>(_T("Mgr"), OBJECT::PRIORTY::OBJECT3D, false);
			m_chocoballMgrList.push_back(mgr);
			// �`���R�{�[�������ʒu���Z�b�g�B
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
			//�`���R�ǂ̐���

			string str = "B_Block";
			char num[100];
			_itoa(CBuildBlock::GetBBManagerNum(), num, 10);
			str += num;
			CBuildBlock* buildBlock = SINSTANCE(CObjectManager)->GenerationObject<CBuildBlock>(_T(str.c_str()), OBJECT::PRIORTY::OBJECT3D,false);
			buildBlock->Initialize(
				D3DXVECTOR3(-pInfo[i].pos.x, pInfo[i].pos.y, -pInfo[i].pos.z),
				pInfo[i].rot
			);
		}
		else if (info.gimmickType == GimmickType_FallFloor){
			//�����鏰����B������
			FallingFloor* fallfloor = SINSTANCE(CObjectManager)->GenerationObject<FallingFloor>(_T("FallFloor"), OBJECT::PRIORTY::OBJECT3D, false);
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
			// ������
			MoveFloor* movefloor = SINSTANCE(CObjectManager)->GenerationObject<MoveFloor>(_T("movefloor"), OBJECT::PRIORTY::OBJECT3D, false);
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
			// �㏸��
			CUpFloor* upfloor = SINSTANCE(CObjectManager)->GenerationObject<CUpFloor>(_T("movefloor"), OBJECT::PRIORTY::OBJECT3D, false);
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
			// ���𕬏o����M�~�b�N
			string str = "firejet";
			char num[100];
			_itoa(m_FireJetNum, num, 10);
			str += num;
			CFireJet* fire = SINSTANCE(CObjectManager)->GenerationObject<CFireJet>(_T(str.c_str()), OBJECT::PRIORTY::OBJECT3D_ALPHA, false);
			fire->SetEmitterName(_T(str.c_str()));
			fire->SetStageID(m_IsStage);
			fire->SetAudio(pAudio);
			fire->Initialize();
			fire->SetPos(D3DXVECTOR3(-pInfo[i].pos.x, pInfo[i].pos.y, -pInfo[i].pos.z));
			D3DXQUATERNION rot(-pInfo[i].rot.x, pInfo[i].rot.y, -pInfo[i].rot.z, pInfo[i].rot.w);
			D3DXVECTOR3 dir = pInfo[i].DirectionZ;
			dir.x *= -1.0f;
			//dir.y *= -1.0f;
			dir.z *= -1.0f;
			D3DXVec3Normalize(&dir, &dir);
			fire->SetDirection(dir);
			m_FireJetNum++;
		}
		else if (info.gimmickType == GimmickType_SmokeJet){
			// ���𕬏o����M�~�b�N
			string str = "smokejet";
			char num[100];
			_itoa(m_FireJetNum, num, 10);
			str += num;
			CSmokeJet* smoke = SINSTANCE(CObjectManager)->GenerationObject<CSmokeJet>(_T(str.c_str()), OBJECT::PRIORTY::OBJECT3D_ALPHA, false);
			smoke->SetEmitterName(_T(str.c_str()));
			smoke->SetStageID(m_IsStage);
			smoke->Initialize();
			smoke->SetPos(D3DXVECTOR3(-pInfo[i].pos.x, pInfo[i].pos.y, -pInfo[i].pos.z));
			D3DXQUATERNION rot(-pInfo[i].rot.x, pInfo[i].rot.y, -pInfo[i].rot.z, pInfo[i].rot.w);
			D3DXVECTOR3 dir = pInfo[i].DirectionZ;
			dir.x *= -1.0f;
			//dir.y *= -1.0f;
			dir.z *= -1.0f;
			D3DXVec3Normalize(&dir, &dir);
			smoke->SetDirection(dir);
			m_SmokeJetNum++;
		}
		else if (info.gimmickType == GimmickType_DivisionWall) {
			// �i�s�s�\�ǁB
			CDivisionWall* Wall = SINSTANCE(CObjectManager)->GenerationObject<CDivisionWall>(_T("DivisionWall"), OBJECT::PRIORTY::OBJECT3D_ALPHA, false);
			Wall->Initialize();
			D3DXQUATERNION rot(pInfo[i].rot.x, pInfo[i].rot.y, pInfo[i].rot.z, pInfo[i].rot.w);
			Wall->Build(D3DXVECTOR3(-pInfo[i].pos.x, pInfo[i].pos.y, -pInfo[i].pos.z),rot);
			m_DivisionWalls.push_back(Wall);
		}

	}

	//���̈����ɓn���̂̓{�b�N�X��halfsize�Ȃ̂ŁA0.5�{����B
	int arraySize2 = GimmickInfoTableSizeArray[StageID];	//�z��̗v�f����Ԃ��B
	SCollisionInfo* pInfo2 = GimmickinfoTableArray[StageID];
	for (int i = 0; i < arraySize2; i++) {
		SCollisionInfo& collision = pInfo2[i];
		if (arraySize2 == 1) {
			if (D3DXVec3Length(&collision.scale) <= 0.001f) {
				// �z�u����Ă��Ȃ��B
				break;
			}
		}
		// �`���R�{�[���g���K�[�̃|�W�V�������Z�b�g�B
		m_chocoballMgrList[i]->SetPos(D3DXVECTOR3(-collision.pos.x, collision.pos.y, -collision.pos.z));
		// �`���R�{�[���g���K�[�̉�]���Z�b�g�B
		m_chocoballMgrList[i]->SetQuaternion(D3DXQUATERNION(collision.rotation.x, collision.rotation.y, collision.rotation.z, collision.rotation.w));
		// �`���R�{�[���g���K�[�̃R���W�����𐶐��B
		m_chocoballMgrList[i]->ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(btVector3(collision.scale.x*0.5f, collision.scale.y*0.5f, collision.scale.z*0.5f)), Collision::Type::ChocoballTrigger,Collision::FilterGroup::Ghost, true, 0.0f, true,true);
		m_chocoballMgrList[i]->GetCollision()->BitMask_AllOff();
		m_chocoballMgrList[i]->GetCollision()->BitMask_On(Collision::FilterGroup::Player);
	}
	m_chocoballMgrList.clear();

	// �}�b�v�I�u�W�F�N�g�z�u�B
	{
		SINSTANCE(CObjectManager)->FindGameObject<CMapObjectManager>(_T("MapChipManager"))->Build();
	}
}
