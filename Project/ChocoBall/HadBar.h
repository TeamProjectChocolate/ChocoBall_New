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
	// ���������ɃQ�[�W�̐F�����߂�B
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
	float m_TargetValue;	// ��{���̒l(�X�V�����ł̖ڕW�l)�B
	float m_Value;		// ��{���̒l(�X�V�����ŕϓ�������l�ŁA���ۂɉ�ʂɕ\������Ă��鐔�l)�B
	float m_MaxValue;	// ��{���̍ő�l�B
	float m_InitBarScale_X;	// �o�[�摜�̍ŏ��̃X�P�[��(x)�B
	float m_InitBarPosition_X;	// �o�[�摜�̍ŏ��̃|�W�V����(x)�B
	float m_WorkValue; // m_Value��m_TargetValue�Ɍ����ď��X�ɐi�߂Ă����Ƃ��̒l�B
	float m_Dir = 1.0f;	// �X�V����(+1��-1)�B
	static float m_Time;	// m_Value��m_TargetValue�ɓ��B����܂ł̎���(�b)�B
};

static const char* BarFileNameArray[] = {
	"Image/hp.png",	// �ԃQ�[�W�B
	"Image/hp_back.png",	// ���F�Q�[�W�B
	"Image/hp_Green.png",	// �΃Q�[�W�B
};

class CHadBar :
	public CGameObject
{
public:
	CHadBar();
	~CHadBar();
	//void Initialize()override;
	/// ���������̓o�[�̍ő�l�B
	/// ����O�����͌��݂̒l�B
	void Initialize(CGameObject* Object,const vector<BarColor>&, float max, float value);
	void Update()override;
	void Draw()override;
	// �ǂ̏��Ԃłǂ̐F��\�����邩�����߂��z���n���ACBarElement�̃C���X�^���X�𐶐�����֐��B
	void ActiveBarColor(const vector<BarColor>& BarColorArray, float max, float value);
	void SetValue(float value);
	//void AddValue(int value) {
	//	m_NowBar->AddValue(value);
	//}
private:
	// �����Q�[�W������ۂ̃C�x���g�B
	void BreakEvent();
private:
	short m_MaxBarNum;	// ���Q�[�W���d�˂邩�B
	CBarFrame m_Frame;	// �o�[�̘g�B
	vector<CBarElement*> m_BarElement;	// �o�[�B
	CBarElement* m_NowBar;	// ���ݍX�V���̃o�[�B
	short m_NowBarNum;	// ���ݍX�V���̃o�[��\���Y�����B
	float m_Varue;		// �S�o�[���킹�Ă̒l�B
	float m_MaxValue;		// �S�o�[���킹�Ă̍ő�l�B
	CBarElement* m_NowSettingBar;	// ���ݐ�s���͒��̃o�[�B
	short m_NowSettingNum;		// ���ݐ�s���͒��̃o�[��\���Y�����B
	CGameObject* m_pObject = nullptr;	// ���̃o�[�����Q�[���I�u�W�F�N�g�B
};