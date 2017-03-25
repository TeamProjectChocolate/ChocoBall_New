#pragma once
#include "GameObject.h"
#include "PixTag.h"

enum BarColor {  Red = 0, Yellow,Green};

class CBarFrame : public CGameObject {
public:
	CBarFrame() {
		strcpy(m_pFileName, "Image/hpsp_bar.png");
	};
	~CBarFrame() {

	};
	void Initialize()override;
};

class CBarElement : public CGameObject {
public:
	CBarElement() {
	};
	~CBarElement() {};
	//void Initialize()override;
	// 初期化時にゲージの色を決める。
	void Initialize(BarColor,float,float);
	void Update()override;
	void Draw()override;
	void SetValue(float value) {
		m_TargetValue = value;
		m_WorkValue = (m_TargetValue - m_Value) / (60.0f * m_Time);
		m_Dir = m_WorkValue / fabsf(m_WorkValue);
	}
	float GetValue() {
		return m_Value;
	}
	float GetTargetValue() {
		return m_TargetValue;
	}
	//void AddValue(int value) {
	//	m_Value += value;
	//}
private:
	void BarValueUpdate();
	void BarScaling();
private:
	float m_TargetValue;	// 一本分の値(更新処理での目標値)。
	float m_Value;		// 一本分の値(更新処理で変動させる値で、実際に画面に表示されている数値)。
	float m_MaxValue;	// 一本分の最大値。
	float m_InitBarScale_X;	// バー画像の最初のスケール(x)。
	float m_InitBarPosition_X;	// バー画像の最初のポジション(x)。
	float m_WorkValue; // m_Valueをm_TargetValueに向けて徐々に進めていくときの値。
	float m_Dir = 1.0f;	// 更新方向(+1か-1)。
	static float m_Time;	// m_Valueがm_TargetValueに到達するまでの時間(秒)。
};

static const char* BarFileNameArray[] = {
	"Image/hp.png",	// 赤ゲージ。
	"Image/hp_back.png",	// 黄色ゲージ。
	"Image/hp_Green.png",	// 緑ゲージ。
};

class CHadBar :
	public CGameObject
{
public:
	CHadBar();
	~CHadBar();
	//void Initialize()override;
	/// ※第二引数はバーの最大値。
	/// ※第三引数は現在の値。
	void Initialize(CGameObject* Object,const vector<BarColor>&, float max, float value);
	void Update()override;
	void Draw()override;
	// どの順番でどの色を表示するかを決めた配列を渡し、CBarElementのインスタンスを生成する関数。
	void ActiveBarColor(const vector<BarColor>& BarColorArray, float max, float value);
	void SetValue(float value);
	//void AddValue(int value) {
	//	m_NowBar->AddValue(value);
	//}
private:
	// ワンゲージ削った際のイベント。
	void BreakEvent();
private:
	short m_MaxBarNum;	// 何ゲージ分重ねるか。
	CBarFrame m_Frame;	// バーの枠。
	vector<CBarElement*> m_BarElement;	// バー。
	CBarElement* m_NowBar;	// 現在更新中のバー。
	short m_NowBarNum;	// 現在更新中のバーを表す添え字。
	float m_Varue;		// 全バー合わせての値。
	float m_MaxValue;		// 全バー合わせての最大値。
	CBarElement* m_NowSettingBar;	// 現在先行入力中のバー。
	short m_NowSettingNum;		// 現在先行入力中のバーを表す添え字。
	CGameObject* m_pObject = nullptr;	// このバーを持つゲームオブジェクト。
};