#pragma once
#include "Render.h"
class CInstancingRender_D :
	public CRender
{
public:
	CInstancingRender_D();
	~CInstancingRender_D();
	void CreateMatrixBuffer(int);	// �l������ő吔���̃��[���h�s��i�[�p�o�b�t�@���`����֐�
	void AddWorldMatrix(const D3DXMATRIX&);
	void Initialize()override;
	void Draw()override{};
	void Draw(const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&)override;
private:
	/*!
	*@brief	���[���h�s��𒸓_�o�b�t�@�ɃR�s�[�B
	*/
	void CopyMatrixToVertexBuffer();
	void DrawFrameDepth(LPD3DXFRAME, LPD3DXEFFECT, const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&);
	void DrawMeshContainerDepth(LPD3DXMESHCONTAINER, LPD3DXFRAME, LPD3DXEFFECT, const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&);
	void AnimationDrawDepth(D3DXMESHCONTAINER_DERIVED*, D3DXFRAME_DERIVED*, LPD3DXEFFECT, const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&);
	void NonAnimationDrawDepth(D3DXFRAME_DERIVED*, LPD3DXEFFECT, const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&);
private:
	bool m_IsFirst;
	// �C���X�^���V���O�`��p�ϐ�
	IDirect3DVertexBuffer9* m_WorldMatrixBuffer = nullptr;			//���[���h�s��̃o�b�t�@�B
	IDirect3DVertexDeclaration9* m_VertexDecl = nullptr;			//���_��`�B
	vector<D3DXMATRIX> m_worldMatrix;	// ���[���h�s��̔z��
};

