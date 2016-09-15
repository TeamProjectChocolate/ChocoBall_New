#include "stdafx.h"
#include "Turn.h"

CTurn::~CTurn()
{
}

void CTurn::Initialize()
{
	m_currentAngleY = 0.0f;
	m_targetAngleY = 0.0f;
	m_turnSpeed = 0.0f;
}

//回転処理
float CTurn::Update(bool isTurn, float m_targetAngleY)
{
	if (isTurn) {
		float angleDiff = m_targetAngleY - m_currentAngleY;	//目的角度までどれだけ回せがいいのかを計算する。
		float angleDiffAbs = fabsf(angleDiff);				//回す角度の絶対値を計算。
		if (angleDiffAbs > 0.0001f) {						//回す角度の大きさが非常に小さい場合は回さない。
			float turnDir = angleDiff / angleDiffAbs;		//回転させる方向を計算する。
			if (angleDiffAbs > fPI) {						//回転させる角度が180度を越えているかを判定する。
				//180度を越える回転のため遠回り。
				m_currentAngleY += 2.0f * fPI *  turnDir;	//現在の角度を-90度なら270度、180度なら-180度にする。
				turnDir *= -1.0f;							//回す方向を反転。
			}
			m_turnSpeed = g_cTurnSpeed * turnDir;
		}
	}
	m_currentAngleY += m_turnSpeed;
	if (fabsf(m_targetAngleY - m_currentAngleY) < fabsf(m_turnSpeed) + 0.01f) {
		//ターン終わり。
		m_turnSpeed = 0.0f;
		m_currentAngleY = m_targetAngleY;
	}
	return m_currentAngleY;
}
