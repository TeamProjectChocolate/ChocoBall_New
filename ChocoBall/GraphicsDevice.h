#pragma once
#include "stdafx.h"

//Direct3D�f�o�C�X�E�I�u�W�F�N�g���Ǘ�����N���X
class CGraphicsDevice
{
public:
	CGraphicsDevice(){
		m_pD3d = nullptr;
		m_pDevice = nullptr;
	}
	~CGraphicsDevice(){
		FreeDX();
	}
	HRESULT InitD3d(HWND);	//Direct3D�I�u�W�F�N�g����уf�o�C�X������
	void ChangeDisplayMode(HWND, HMENU, bool*, bool*, RECT*);	// �f�B�X�v���C���[�h�̕ύX
	void FreeDX();		//Direct3D�f�o�C�X�E�I�u�W�F�N�g���

	bool GetState(void){
		if (!m_pD3d || !m_pDevice){
			return false;
		}
	}
	// �f�o�C�X�̋�����Ԃ��m�F����
	HRESULT GetCooperativeLevel(void){
		return m_pDevice->TestCooperativeLevel();
	}
	// �V���O���g��
	operator LPDIRECT3DDEVICE9() const { return this->m_pDevice; }
private:
	LPDIRECT3D9 m_pD3d;			//DIRECT3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pDevice;	//Direct3D�f�o�C�X
};

//�O���[�o���֐����`���Ăǂ����ɂ�����̂�Ԃ�(�����ł�����cGraphicsDevice�N���X�̃C���X�^���X��Ԃ�)
inline CGraphicsDevice& graphicsDevice()
{
	extern CGraphicsDevice g_graphicsDevice;
	return g_graphicsDevice;
}

struct CUSTOMVERTEX{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL)
