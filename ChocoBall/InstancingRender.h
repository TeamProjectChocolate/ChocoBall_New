#pragma once
#include "Render.h"

class CInstancingRender:public CRender
{
public:
	CInstancingRender();
	~CInstancingRender();
	void CreateMatrixBuffer(unsigned int);	// �l������ő吔���̃��[���h�s��i�[�p�o�b�t�@���`����֐�
	void AddWorldMatrix(const D3DXMATRIX&);
	void AddRotationMatrix(const D3DXMATRIX&);
	void Initialize()override;
	void Draw()override;
	IDirect3DVertexBuffer9* GetWorldMatrixBuffer(){
		return m_WorldMatrixBuffer;
	}
private:
	/*!
	*@brief	���[���h�s��𒸓_�o�b�t�@�ɃR�s�[�B
	*/
	void CopyMatrixToVertexBuffer();
	void DrawFrame(LPD3DXFRAME);
	void DrawMeshContainer(LPD3DXMESHCONTAINER, LPD3DXFRAME);
	void AnimationDraw(D3DXMESHCONTAINER_DERIVED*, D3DXFRAME_DERIVED*);
	void NonAnimationDraw(D3DXFRAME_DERIVED*);
private:
	bool m_UseBorn;
	bool m_IsFirst;
	// �C���X�^���V���O�`��p�ϐ�
	IDirect3DVertexBuffer9* m_WorldMatrixBuffer = nullptr;			//���[���h�s��̃o�b�t�@�B
	IDirect3DVertexDeclaration9* m_VertexDecl = nullptr;			//���_��`�B
	vector<D3DXMATRIX> m_worldMatrix;	// ���[���h�s��̔z��
	IDirect3DVertexBuffer9* m_RotationMatrixBuffer = nullptr;
	vector<D3DXMATRIX> m_RotationMatrix;
	unsigned int m_MaxNum;	// �o�b�t�@�[�̃T�C�Y(���̐��𒴂���o�b�t�@�[�T�C�Y���K�v�ɂȂ����ꍇ�A�o�b�t�@�[����蒼��);
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
};

