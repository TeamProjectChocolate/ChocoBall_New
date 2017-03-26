#include "stdafx.h"
#include "HadBar.h"
#include "Enemy_Boss.h"

// CBarFrame�B
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


// CBarElement�B
float CBarElement::m_Time = 0.2f;
void CBarElement::Initialize(BarColor color, float max, float value) {
	UseModel<C2DImage>();
	// �o�^����Ă���Q�[�W�̉摜���󂯎�����l�Ō���B
	strcpy(m_pFileName, BarFileNameArray[color]);
	m_pModel->SetFileName(m_pFileName);

	m_MaxValue = max;
	m_TargetValue = value;
	m_Value = value;

	CGameObject::Initialize();
	// Transform�ݒ�B
	m_transform.position += D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	SetRotation(0.0f);
	m_transform.scale = D3DXVECTOR3(911.0f, 32.0f, 0.0f);
	m_InitBarScale_X = m_transform.scale.x;
	m_InitBarPosition_X = m_transform.position.x;
	// �����܂�̏����B
	SetAlive(true);
	m_pModel->m_alpha = 1.0f;
}

void CBarElement::Update() {
	// �ڕW�l�Ɍ����ăo�[�̒l�����X�ɍX�V�B
	BarValueUpdate();
	// ���݂̒l�Ńo�[�̒������X�V����B
	BarScaling();
	CGameObject::Update();
}

void CBarElement::BarValueUpdate() {
	if (m_Value >= m_TargetValue && m_Dir > 0) {
		// �X�V������+�A���ڕW�l�ɓ��B�����B
		m_Value = m_TargetValue;
	}
	else if (m_Value <= m_TargetValue && m_Dir < 0) {
		// �X�V������-�A���ڕW�l�ɓ��B�����B
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


// CHadBar�B
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
	// �o�[�̃t���[���𐶐��B
	m_Frame.SetPos(m_transform.position);
	m_Frame.SetQuaternion(m_transform.angle);
	m_Frame.Initialize();

	// �w�肳�ꂽ�F�̏��Ɛ��ɏ]���ăo�[�𐶐��B
	ActiveBarColor(BarColorArray,max,value);
	// �ŏ��̃o�[������B
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
			// �X�V���̃o�[��ύX�B
			m_NowBar = m_BarElement[m_NowBarNum].get();
		}
	}
}

void CHadBar::SetValue(float value) {
	float work = static_cast<int>(value) % (static_cast<int>(m_MaxValue) / m_MaxBarNum);	// ��{�̃o�[�ɃZ�b�g����l���Z�o�B

	bool isBreak = false;	// 1�Q�[�W�ȏ����Ă��邩�B

	float Difference = m_Varue - value;	// ��u�O�̒l�Ƃ̍������Z�o�B
	if(Difference >= 0.0001f){
		// ��u�O�̒l�ƈႤ�l���ݒ肳�ꂽ�B
		while (true) {
			float nowBarValue = m_NowSettingBar->GetTargetValue();
			if (nowBarValue <= Difference) {
				// 1�Q�[�W�ȏ������B
				// �u���C�N���̃C�x���g�B
				this->BreakEvent();
				// �Ƃ肠�������̃o�[�̒l��0�ɂ���B
				m_NowSettingBar->SetValue(0.0f);
				m_NowSettingNum++;
				if (m_NowSettingNum < m_MaxBarNum) {
					// ���̃o�[������΂�����Ƀ^�[�Q�b�g��ύX�B
					m_NowSettingBar = m_BarElement[m_NowSettingNum].get();
					// �ݒ肵���l��������������A�V����������ݒ�B
					Difference -= nowBarValue;
					if (nowBarValue < Difference) {
						// 1�Q�[�W�ȏ゠�ӂ��ꍇ�͏����𑱍s�B
						continue;
					}
					else {
						// ���傤��1�Q�[�W�g�����B
						break;
					}
				}
				else {
					// ���̃o�[���Ȃ���Ώ������I���B
					m_NowSettingNum--;
					break;
				}
			}
			else {
				// ���ӂ�Ȃ��ꍇ�̓o�[�ɓ���ď������I���B
				m_NowSettingBar->SetValue(work);
				break;
			}
		}
		// �S�̂Ƃ��Ă̒l���X�V�B
		m_Varue = value;
	}
}

void CHadBar::Draw() {
	m_Frame.Draw();
	// ���ɂ���o�[����`�悷��B
	for (int idx = m_BarElement.size() - 1; idx >= 0;idx--) {
		m_BarElement[idx]->Draw();
	}
}

void CHadBar::ActiveBarColor(const vector<BarColor>& BarColorArray,float max,float value) {
	m_MaxBarNum = BarColorArray.size();
	for (auto BarColor : BarColorArray) {
		unique_ptr<CBarElement> bar(new CBarElement);
		// �e�q�֌W�B
		bar->SetPos(m_transform.position);
		bar->SetQuaternion(m_transform.angle);
		// �o�[���d�˂�ꍇ�͒l���d�˂�o�[�̐���������������B
		bar->Initialize(BarColor, max / m_MaxBarNum, value / m_MaxBarNum);
		bar->Update();
		m_BarElement.push_back(move(bar));
	}
}

void CHadBar::BreakEvent() {
	static_cast<CEnemy_Boss*>(m_pObject)->BreakEventCallBack();
}