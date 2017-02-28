#pragma once
#include "stdafx.h"
#include "Stage.h"
#include "Field.h"
#include "CourceCamera.h"
#include "TestObject2D.h"
#include "EnemyManager.h"


class CStageManager
{
private:
	SINGLETON_DECL(CStageManager)
public:
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

	//次のステージへ移行する関数
	void SetNextStage();

	//ステージ切り替え関数
	//引き数: LPCSTR型 どのステージに切り替えるか
	//返り値: なし
	//呼び出し例: SINSTANCE(CStageManager)->ChangeStage(_T("インスタンスの名前"));
	//※関数が重いため多用しないこと
	void ChangeStage(STAGE_ID);

	void SetIsContinue(bool flg){
		m_IsContinue = flg;
	}
	void SetAudio(CAudio* pAudio){
		m_pAudio = pAudio;
	}
	void SetContinueStage(STAGE_ID id){
		m_ContinueStage = id;
	}
public:
	static int m_ClearNum;
private:
	bool m_IsContinue;		// ステージを続きから遊ぶ場合はtrue,そうでなければfalse
	STAGE_ID m_ContinueStage;	// ゲームオーバーしたときに現在のステージを保持する
	STAGE_ID m_NowStage;
	STAGE_ID m_NextStage;

	CStage* m_pStage = nullptr;

	CAudio* m_pAudio;
	//※デストラクタにて呼び出される
	void DeleteAll();
};
