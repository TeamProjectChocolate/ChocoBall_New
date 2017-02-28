#pragma once
#include "stdafx.h"
#include "Infomation.h"
#include "Assert.h"


class CLight
{
public:
	// �e�평����(�S�~�f�[�^�����ꍞ�܂Ȃ��悤��)
	CLight(){
		for (short idx = 0; idx < NUM_DIFFUSE_LIGHT; idx++) {
			this->SetDiffuseLightColor(idx, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f));
			this->SetDiffuseLightDirection(idx, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
		}
		this->SetAmbientLight(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->SetLimLightDirection(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
		this->SetLimLightColor(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f));
	};
	~CLight(){};

	//�f�B�t���[�Y���C�g�̌�����ݒ肷��֐�
	//������: int�^ �ǂ̃��C�g�̌�����ݒ肷�邩
	//	�@�@: D3DXVECTOR3�^ �ݒ肷������̒l(���K�����ꂽ�x�N�g���A�ő�l1�A�ŏ��l-1)
	//�Ԃ�l: �Ȃ�
	void SetDiffuseLightDirection(int lightNum, D3DXVECTOR3 dir){
		CH_ASSERT(lightNum < NUM_DIFFUSE_LIGHT);
		D3DXVec3Normalize(&dir,&dir);
		m_diffuseLightDirection[lightNum] = dir;
	}

	//�f�B�t���[�Y���C�g�̌������擾����֐�
	//������: �Ȃ�
	//�Ԃ�l: D3DXVECTOR3*�^ �e�f�B�t���[�Y���C�g�̌������i�[�����z��̐擪�A�h���X
	D3DXVECTOR3* GetDiffuseLightDirection(){
		return m_diffuseLightDirection;
	}

	//�f�B�t���[�Y���C�g�̃J���[��ݒ肷��֐�
	//������: int�^ �ǂ̃��C�g�̃J���[��ݒ肷�邩
	//	�@�@: D3DXVECTOR4�^ �ݒ肷��J���[�̒l
	//�Ԃ�l: �Ȃ�
	void SetDiffuseLightColor(int lightNum, D3DXVECTOR4 color){
		CH_ASSERT(lightNum < NUM_DIFFUSE_LIGHT);
		m_diffuseLightColor[lightNum] = color;
	}

	//�f�B�t���[�Y���C�g�̃J���[���擾����֐�
	//������: �Ȃ�
	//�Ԃ�l: D3DXVECTOR4*�^ �e�f�B�t���[�Y���C�g�̃J���[���i�[�����z��̐擪�A�h���X
	D3DXVECTOR4* GetDiffuseLightColor(){
		return m_diffuseLightColor;
	}

	//������ݒ肷��֐�
	//������: D3DXVECTOR3�^ �����H
	//�Ԃ�l: �Ȃ�
	void SetAmbientLight(D3DXVECTOR3 ambient){
		m_ambientLight = ambient;
	}

	//�������擾����֐�
	//������: �Ȃ�
	//�Ԃ�l: D3DXVECTOR3&�^ ����
	D3DXVECTOR3& GetAmbientLight(){
		return m_ambientLight;
	}

	/*!
	*@brief	�������C�g�̕�����ݒ�B
	*/
	void SetLimLightDirection(const D3DXVECTOR3& dir)
	{
		m_limLightDir = dir;
	}
	/*!
	*@brief	�������C�g�̕������擾�B
	*/
	const D3DXVECTOR3& GetLimLightDirection() const
	{
		return m_limLightDir;
	}
	/*!
	*@brief	�������C�g�̐F��ݒ�B
	*/
	void SetLimLightColor(const D3DXVECTOR4& color)
	{
		m_limLightColor = color;
	}
	/*!
	*@brief	�������C�g�̐F���擾�B
	*/
	const D3DXVECTOR4& GetLimLightColor() const
	{
		return m_limLightColor;
	}

	void SetLight(LPD3DXEFFECT);
private:
	D3DXVECTOR3 m_diffuseLightDirection[NUM_DIFFUSE_LIGHT];		// �f�B�t���[�Y���C�g�̌���
	D3DXVECTOR4 m_diffuseLightColor[NUM_DIFFUSE_LIGHT];			// �f�B�t���[�Y���C�g�̃J���[
	D3DXVECTOR3	m_limLightDir;									//!<�������C�g�̕����B
	D3DXVECTOR4	m_limLightColor;								//!<�������C�g�̐F�B
	D3DXVECTOR3 m_ambientLight;									// ����
};

