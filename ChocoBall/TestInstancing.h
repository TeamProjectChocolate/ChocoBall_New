#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "Effect.h"

#define MAX_INSTANCE 5000

class CTestInstancing
{
public:
	CTestInstancing();
	~CTestInstancing();
	void CreateMatrixBuffer(int);	// �l������ő吔���̃��[���h�s��i�[�p�o�b�t�@���`����֐�
	void Initialize();
	void Update();
	void Draw();
	void CopyMatrixToVertexBuffer();
private:
	C3DImage model;
	LPD3DXEFFECT m_pEffect;
	// �C���X�^���V���O�`��p�ϐ�
	IDirect3DVertexBuffer9* m_WorldMatrixBuffer = nullptr;			//���[���h�s��̃o�b�t�@�B
	IDirect3DVertexDeclaration9* m_VertexDecl = nullptr;			//���_��`�B
	vector<D3DXMATRIX> m_worldMatrix;	// ���[���h�s��̔z��
	vector<D3DXMATRIX*> pWorlds;
	// �V�F�[�_�[�ɒl���Z�b�g���邽�߂̃n���h���Q
	D3DXHANDLE m_hEyePosition;
	D3DXHANDLE m_hWorldMatrixArray;
	D3DXHANDLE m_hluminance;
	D3DXHANDLE m_hnumBone;
	D3DXHANDLE m_hAlpha;
	D3DXHANDLE m_hRota;
	D3DXHANDLE m_hWorld;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hShadowMap;
	D3DXVECTOR3 margin[MAX_INSTANCE];
	float  m_IntervalTime;
	float m_TimeCounter;
	D3DXVECTOR3 rnd[MAX_INSTANCE];
	D3DXVECTOR3 TargetRnd[MAX_INSTANCE];
	bool flg[MAX_INSTANCE];
};

