#include "stdafx.h"
#include "HadBar.h"
#include "Enemy_Boss.h"

// CBarFrame。
void CBarFrame::Initialize() {
	UseModel<C2DImage>();
	m_pModel->SetFileName(m_pFileName);
	CGameObject::Initialize();
	m_transform.position += D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(960.0f, 55.0f, 0.0f);
	SetAlive(true);
	m_pModel->m_alpha = 1.0f;
}


// CBarElement。
float CBarElement::m_Time = 0.2f;
void CBarElement::Initialize(BarColor color, float max, float value) {
	UseModel<C2DImage>();
	// 登録されているゲージの画像を受け取った値で決定。
	strcpy(m_pFileName, BarFileNameArray[color]);
	m_pModel->SetFileName(m_pFileName);

	m_MaxValue = max;
	m_TargetValue = value;
	m_Value = value;

	CGameObject::Initialize();
	// Transform設定。
	m_transform.position += D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(911.0f, 32.0f, 0.0f);
	m_InitBarScale_X = m_transform.scale.x;
	m_InitBarPosition_X = m_transform.position.x;
	// お決まりの処理。
	SetAlive(true);
	m_pModel->m_alpha = 1.0f;
}

void CBarElement::Update() {
	// 目標値に向けてバーの値を徐々に更新。
	BarValueUpdate();
	// 現在の値でバーの長さを更新する。
	BarScaling();
	CGameObject::Update();
}

void CBarElement::BarValueUpdate() {
	if (m_Value >= m_TargetValue && m_Dir > 0) {
		// 更新方向が+、かつ目標値に到達した。
		m_Value = m_TargetValue;
	}
	else if (m_Value <= m_TargetValue && m_Dir < 0) {
		// 更新方向が-、かつ目標値に到達した。
		m_Value = m_TargetValue;
	}
	else {
		m_Value += m_WorkValue;
	}
}

void CBarElement::BarScaling() {
	float weight = static_cast<float>(m_Value) / m_MaxValue;
	if (fabsf(weight) <= 0.001f) {
		weight = 0.0f;
	}
	m_transform.scale.x = m_InitBarScale_X * weight;

	float offset = m_InitBarScale_X - m_transform.scale.x;
	m_transform.position.x = m_InitBarPosition_X - (offset / 2);
}

void CBarElement::Draw() {
	CGameObject::Draw();
}


// CHadBar。
CHadBar::CHadBar()
{
}

CHadBar::~CHadBar()
{
}

void CHadBar::Initialize(CGameObject* Object,const vector<BarColor>& BarColorArray,float max,float value) {
	SetAlive(true);
	m_transform.position = D3DXVECTOR3(480.0f, 27.0f, 1.0f);
	SetRotation(0.0f);
	// バーのフレームを生成。
	m_Frame.SetPos(m_transform.position);
	m_Frame.SetQuaternion(m_transform.angle);
	m_Frame.Initialize();

	// 指定された色の順と数に従ってバーを生成。
	ActiveBarColor(BarColorArray,max,value);
	// 最初のバーを決定。
	m_NowBarNum = m_NowSettingNum = 0;
	m_NowBar = m_NowSettingBar = m_BarElement[m_NowBarNum].get();

	m_MaxValue = max;
	m_Varue = value;
	m_pObject = Object;
}

void CHadBar::Update() {
	m_Frame.Update();
	m_NowBar->Update();
	if (m_NowBar->GetValue() <= 0.0f) {
		m_NowBarNum++;
		if (m_NowBarNum < 0) {
			m_NowBarNum = 0;
		}
		if (m_NowBarNum < m_MaxBarNum) {
			// 更新中のバーを変更。
			m_NowBar = m_BarElement[m_NowBarNum].get();
		}
	}
}

void CHadBar::SetValue(float value) {
	float work = static_cast<int>(value) % (static_cast<int>(m_MaxValue) / m_MaxBarNum);	// 一本のバーにセットする値を算出。

	bool isBreak = false;	// 1ゲージ以上削っているか。

	float Difference = m_Varue - value;	// 一瞬前の値との差分を算出。
	if(Difference >= 0.0001f){
		// 一瞬前の値と違う値が設定された。
		while (true) {
			float nowBarValue = m_NowSettingBar->GetTargetValue();
			if (nowBarValue <= Difference) {
				// 1ゲージ以上削った。
				// ブレイク時のイベント。
				this->BreakEvent();
				// とりあえず今のバーの値を0にする。
				m_NowSettingBar->SetValue(0.0f);
				m_NowSettingNum++;
				if (m_NowSettingNum < m_MaxBarNum) {
					// 次のバーがあればそちらにターゲットを変更。
					m_NowSettingBar = m_BarElement[m_NowSettingNum].get();
					// 設定した値を差分から引き、新しい差分を設定。
					Difference -= nowBarValue;
					if (nowBarValue < Difference) {
						// 1ゲージ以上あふれる場合は処理を続行。
						continue;
					}
					else {
						// ちょうど1ゲージ使った。
						break;
					}
				}
				else {
					// 次のバーがなければ処理を終了。
					m_NowSettingNum--;
					break;
				}
			}
			else {
				// あふれない場合はバーに入れて処理を終了。
				m_NowSettingBar->SetValue(work);
				break;
			}
		}
		// 全体としての値を更新。
		m_Varue = value;
	}
}

void CHadBar::Draw() {
	m_Frame.Draw();
	// 下にあるバーから描画する。
	for (int idx = m_BarElement.size() - 1; idx >= 0;idx--) {
		m_BarElement[idx]->Draw();
	}
}

void CHadBar::ActiveBarColor(const vector<BarColor>& BarColorArray,float max,float value) {
	m_MaxBarNum = BarColorArray.size();
	for (auto BarColor : BarColorArray) {
		unique_ptr<CBarElement> bar(new CBarElement);
		// 親子関係。
		bar->SetPos(m_transform.position);
		bar->SetQuaternion(m_transform.angle);
		// バーを重ねる場合は値を重ねるバーの数分だけ分割する。
		bar->Initialize(BarColor, max / m_MaxBarNum, value / m_MaxBarNum);
		bar->Update();
		m_BarElement.push_back(move(bar));
	}
}

void CHadBar::BreakEvent() {
	static_cast<CEnemy_Boss*>(m_pObject)->BreakEventCallBack();
}