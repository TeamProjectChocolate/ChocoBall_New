#pragma once
#include "Enemy_Boss.h"

class CState
{
public:
	CState() {};
	CState(CEnemy_Boss* obj){
		m_pObject = obj;
	}
	virtual ~CState() {}
	virtual void Entry() {};
	// 戻り値は処理が終了したか。
	virtual bool Update() { return true; };
	virtual void Exit(CEnemy_Boss::BOSS_STATE) {};
	// このステートでダメージを与えられるか。
	virtual bool IsPossibleDamage() {
		return true;
	}
protected:
	// ローカルステートを引数の値に対応したステートに変更する。
	void ChangeLocalState(CEnemy_Boss::BOSS_STATE next) {
		if (m_pCurrentLocalState) {
			m_pCurrentLocalState->Exit(next);
		}
		m_pCurrentLocalState = m_pObject->GetState(next);
		m_pCurrentLocalState->Entry();
		m_NowLocalState = next;
	}
protected:
	CEnemy_Boss* m_pObject = nullptr;	// ボスのポインタ(ステート内部でボスを移動させる際などに使用)。
	CEnemy_Boss::BOSS_STATE m_NowLocalState;
	CState* m_pCurrentLocalState = nullptr;	// 現在のローカルステート。
};

