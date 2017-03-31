#include "stdafx.h"
#include "RushAttackState.h"
#include "BuildBlock.h"
#include "CBManager.h"

CRushAttackState::~CRushAttackState()
{
}

void CRushAttackState::Entry() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::FastsSwim);
	m_pObject->SetPlayingState(ANIMATION::PLAYING_STATE::REPEAT);

	m_TimeCounter = 0.0f;
	m_SumTimeCounter = 0.0f;
	m_DamageTimeCounter = 0.0f;
	m_IntervalTime = 1.0f;
	m_AttackTime = 5.0f;
	m_DamageIntervalTime = 1.5f;
	m_CreateCBNum = 2;
	m_IsFirst = true;
	m_Count = 0;
}

bool CRushAttackState::Update() {
	const float deltaTime = 1.0f / 60.0f;
	m_TimeCounter += deltaTime;
	m_SumTimeCounter += deltaTime;
	if (m_SumTimeCounter >= m_AttackTime) {
		// 攻撃終了。
		// 攻撃終了からカウント。
		m_DamageTimeCounter += deltaTime;
		if (m_DamageTimeCounter >= m_DamageIntervalTime) {
			// 一定時間待った。
			if (m_IsFirst) {
				// ボスの頭上にチョコボール生成(自爆)。
				m_pObject->GetAudio()->PlayCue("Chocoball", true, nullptr);//チョコ落下Audio
				CCBManager* CBM = SINSTANCE(CObjectManager)->GenerationObject<CCBManager>(_T("BossDamageChocoBall"), OBJECT::PRIORTY::OBJECT3D, false);
				CBM->Initialize();
				CBM->SetStartPosition(m_pObject->GetPos() + D3DXVECTOR3(0.0f, 3.0f, 0.0f));
				CBM->SetEndPosition(m_pObject->GetPos());
				CBM->SetAlive(true);
				m_IsFirst = false;
				return false;
			}
		}
	}
	else {
		if (m_TimeCounter >= m_IntervalTime) {
			// 攻撃処理。
			D3DXVECTOR3 WallPos = m_pObject->GetPos();;
			// チョコ壁を生成していく。
			{
				string txt = "B_Block";
				char num[100];
				_itoa(CBuildBlock::GetBBManagerNum(), num, 10);
				txt += num;
				CBuildBlock* BB = SINSTANCE(CObjectManager)->GenerationObject<CBuildBlock>(txt.c_str(), OBJECT::PRIORTY::OBJECT3D, false);
				// 高さを調整。
				WallPos.y -= BLOCK_H * BUILD_H;
				float offsetY = 2.65f;
				WallPos.y += offsetY;

				float Volum = BLOCK_W * (BUILD_W * 0.5f);
				float OffsetX = Volum - (BLOCK_W  * 0.5f);	

				// まずは出現位置が中心になるようにする。
				WallPos -= m_pObject->GetTransform().right * Volum;

				// 位置を何パターンかに分ける。
				// いい感じにばらけさせる。
				int randPtern = 3;
				int rnd = (rand() % randPtern) - (randPtern / 2);
				WallPos += m_pObject->GetTransform().right * (Volum * rnd);
				// ボスの鼻先くらいから出現するよう調整。
				float offsetZ = 8.0f;
				BB->Initialize(
					WallPos + (m_pObject->GetDirection() * offsetZ),
					m_pObject->GetQuaternion()
				);
				// チョコ壁を飛ばす。
				BB->ThrowBlock(m_pObject->GetDirection(), 0.1f, 1.75f);
			}
			// チョコボールを生成していく。
			if (m_CreateCBNum > 0 && m_Count % 2 == 0) {
				m_pObject->GetAudio()->PlayCue("Chocoball", true, nullptr);//チョコ落下Audio
				CCBManager* CBM = SINSTANCE(CObjectManager)->GenerationObject<CCBManager>(_T("BossDamageChocoBall"), OBJECT::PRIORTY::OBJECT3D, false);
				CBM->Initialize();
				D3DXVECTOR3 BallPos = WallPos;	// 一旦壁を生成した位置を格納。
				// 生成位置がチョコ壁の左右になるよう調整。
				float WallSize = BLOCK_W * BUILD_W;
				float Volum = 0.5f;
				int rnd = rand() % 3;
				BallPos += m_pObject->GetTransform().right * ((WallSize * Volum) * rnd);
				// ボスの鼻先くらいで生成するよう調整。
				float offset = 8.0f;
				BallPos += (m_pObject->GetDirection() * offset);
				CBM->SetStartPosition(BallPos);
				CBM->SetEndPosition(BallPos + m_pObject->GetDirection() * offset);
				CBM->SetAlive(true);
				// ボスにダメージが入らないよう設定。
				CBM->SetIsBossDamage(false);
				m_CreateCBNum--;
			}
			m_Count++;
			m_TimeCounter = 0.0f;
		}
	}
	return false;
}

void CRushAttackState::Exit(CEnemy_Boss::BOSS_STATE next) {
}